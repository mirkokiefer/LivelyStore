
#include "LCCommit.h"

typedef struct commitData* commitDataRef;
static void* commitInitData();
void commitWalkChildren(LCObjectRef object, void *cookie, childCallback cb);
void commitStoreChildren(LCObjectRef object, char *key, LCObjectRef objects[], size_t length);

struct commitData {
  LCArrayRef parents;
  LCTreeRef tree;
};

static LCArrayRef commitParents(LCCommitRef commit);

void commitDealloc(LCCommitRef object);
void commitDeserialize(LCCommitRef commit);
LCCommitRef findCommit(LCCommitRef commits[], size_t length, char hash[HASH_LENGTH]);

struct LCType typeCommit = {
  .name = "LCCommit",
  .immutable = true,
  .dealloc = commitDealloc,
  .initData = commitInitData,
  .walkChildren = commitWalkChildren,
  .storeChildren = commitStoreChildren
};

LCTypeRef LCTypeCommit = &typeCommit;

static LCArrayRef commitParents(LCCommitRef commit) {
  commitDataRef data = objectData(commit);
  return data->parents;
}

static void* commitInitData() {
  commitDataRef data = malloc(sizeof(struct commitData));
  if (data) {
    data->parents = NULL;
    data->tree = NULL;
  }
  return data;
}

LCCommitRef LCCommitCreate(LCTreeRef tree, LCCommitRef parents[], size_t parentsLength) {
  commitDataRef data = commitInitData();
  data->parents = LCArrayCreate(parents, parentsLength);
  data->tree = objectRetain(tree);
  return objectCreate(LCTypeCommit, data);
};

LCTreeRef LCCommitTree(LCCommitRef commit) {
  commitDataRef data = objectData(commit);
  return data->tree;
}

LCCommitRef* LCCommitParents(LCCommitRef commit) {
  return (LCCommitRef*)LCArrayObjects(commitParents(commit));
}

size_t LCCommitParentsLength(LCCommitRef commit) {
  return LCArrayLength(commitParents(commit));
}

LCCommitRef LCCommitFindParent(LCCommitRef commit, char hash[HASH_LENGTH]) {
  return findCommit(&commit, 1, hash);
}

static void addHashObjectsToDict(LCObjectRef objects[], size_t length, LCMutableDictionaryRef dict) {
  for (LCInteger i=0; i<length; i++) {
    char hash[HASH_LENGTH];
    objectHash(objects[i], hash);
    LCStringRef hashString = LCStringCreate(hash);
    LCMutableDictionarySetValueForKey(dict, hashString, objects[i]);
    objectRelease(objects[i]);
  }
}

static LCCommitRef hashObjectInAllDicts(LCObjectRef objects[], size_t objectsLength, LCMutableDictionaryRef dicts[], size_t dictsLength) {
  for (LCInteger i=0; i<objectsLength; i++) {
    LCStringRef hash = objectCreateHashString(objects[i]);
    bool partOfAll = true;
    for (LCInteger j=0; j<dictsLength; j++) {
      LCObjectRef object = LCMutableDictionaryValueForKey(dicts[j], hash);
      if (!object) {
        partOfAll = false;
        break;
      }
    }
    objectRelease(hash);
    if (partOfAll) {
      return objects[i];
    }
  }
  return NULL;
}

static LCArrayRef commitsToParents(LCArrayRef commitArray) {
  LCCommitRef *commits = LCArrayObjects(commitArray);
  LCMutableArrayRef parents = LCMutableArrayCreate(NULL, 0);
  for (LCInteger i=0; i<LCArrayLength(commitArray); i++) {
    LCMutableArrayAddObjects(parents, LCCommitParents(commits[i]), LCCommitParentsLength(commits[i]));
  }
  return parents;
}

static LCCommitRef commitFindCommonParent(LCArrayRef commitArrays[], LCMutableDictionaryRef passed[], size_t length) {
  bool allLengthZero = true;
  for (LCInteger i=0; i<length; i++) {
    if(LCArrayLength(commitArrays[i]) > 0) {
      allLengthZero = false;
      LCCommitRef *commits = LCArrayObjects(commitArrays[i]);
      addHashObjectsToDict(commits, LCArrayLength(commitArrays[i]), passed[i]);
    }
  }
  for (LCInteger i=0; i<length; i++) {
    LCCommitRef *commits = LCArrayObjects(commitArrays[i]);
    LCCommitRef result = hashObjectInAllDicts(commits, LCArrayLength(commitArrays[i]), passed, length);
    if (result) {
      return result;
    }
  }
  if (allLengthZero) {
    return NULL;
  } else {
    LCArrayRef commitParentsArray[length];
    for (LCInteger i=0; i<length; i++) {
      commitParentsArray[i] = commitsToParents(commitArrays[i]);
    }
    LCCommitRef result = commitFindCommonParent(commitParentsArray, passed, length);
    for (LCInteger i=0; i<length; i++) {
      objectRelease(commitParentsArray[i]);
    }
    return result;
  }
}

LCCommitRef LCCommitFindCommonParent(LCCommitRef commits[], size_t length) {
  LCMutableDictionaryRef passed[length];
  LCArrayRef commitArrays[length];
  for (LCInteger i=0; i<length; i++) {
    passed[i] = LCMutableDictionaryCreate(NULL, 0);
    commitArrays[i] = LCArrayCreate(&commits[i], 1);
  }
  LCCommitRef result = commitFindCommonParent(commitArrays, passed, length);
  for (LCInteger i=0; i<length; i++) {
    objectRelease(passed[i]);
    objectRelease(commitArrays[i]);
  }
  return result;
}

static LCKeyValueRef pathArrayToPathString(LCInteger i, void* info, LCKeyValueRef keyValue) {
  LCArrayRef pathArray = LCKeyValueKey(keyValue);
  LCStringRef path = LCStringCreateFromStringsWithDelim(LCArrayObjects(pathArray), LCArrayLength(pathArray), "/");
  LCKeyValueRef pathValue = LCKeyValueCreate(path, LCKeyValueValue(keyValue));
  objectRelease(path);
  return pathValue;
}


LCArrayRef LCCommitDiff(LCCommitRef oldCommit, LCCommitRef newCommit) {
  LCArrayRef treeDiff = LCTreeChangedPathValues(LCCommitTree(oldCommit), LCCommitTree(newCommit));
  LCArrayRef diff = LCArrayCreateMutableArrayWithMap(treeDiff, NULL, pathArrayToPathString);
  objectRelease(treeDiff);
  return diff;
}

void commitDealloc(LCCommitRef object) {
  commitDataRef data = objectData(object);
  objectRelease(data->parents);
  objectRelease(data->tree);
}

void commitWalkChildren(LCCommitRef commit, void *cookie, childCallback cb) {
  cb(cookie, "parents", LCCommitParents(commit), LCCommitParentsLength(commit), false);
  LCTreeRef tree = LCCommitTree(commit);
  if(tree) {
    cb(cookie, "tree", &tree, 1, false);
  }
}

void commitStoreChildren(LCObjectRef object, char *key, LCObjectRef objects[], size_t length) {
  commitDataRef data = objectData(object);
  if (strcmp(key, "parents") == 0) {
    data->parents = LCArrayCreate(objects, length);
  } else if(strcmp(key, "tree") == 0) {
    data->tree = objectRetain(*objects);
  }
}

LCCommitRef findCommit(LCCommitRef commits[], size_t length, char hash[HASH_LENGTH]) {
  if (length == 0) {
    return NULL;
  }
  for (LCInteger i=0; i<length; i++) {
    char eachHash[HASH_LENGTH];
    objectHash(commits[i], eachHash);
    if(strcmp(hash, eachHash)==0) {
      return commits[i];
    }
  }
  LCMutableArrayRef parents = LCMutableArrayCreate(NULL, 0);
  for (LCInteger i=0; i<length; i++) {
    LCMutableArrayAddObjects(parents, LCCommitParents(commits[i]), LCCommitParentsLength(commits[i]));
  }
  LCCommitRef result = findCommit((LCCommitRef*)LCMutableArrayObjects(parents), LCMutableArrayLength(parents), hash);
  objectRelease(parents);
  return result;
}
