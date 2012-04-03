
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

static LCCommitRef commitFindCommonParent(LCArrayRef commits1, LCArrayRef commits2, LCMutableDictionaryRef passed1, LCMutableDictionaryRef passed2) {
  if (LCArrayLength(commits1) == 0 && LCArrayLength(commits2) == 0) {
    return NULL;
  }
  LCCommitRef *commits1Ref = LCArrayObjects(commits1);
  LCCommitRef *commits2Ref = LCArrayObjects(commits2);
  for (LCInteger i=0; i<LCArrayLength(commits1); i++) {
    char hash[HASH_LENGTH];
    objectHash(commits1Ref[i], hash);
    LCStringRef hashString = LCStringCreate(hash);
    LCCommitRef result = LCMutableDictionaryValueForKey(passed2, hashString);
    if (result) {
      return result;
    }
    LCMutableDictionarySetValueForKey(passed1, hashString, commits1Ref[i]);
    objectRelease(hashString);
  }
  for (LCInteger i=0; i<LCArrayLength(commits2); i++) {
    char hash[HASH_LENGTH];
    objectHash(commits2Ref[i], hash);
    LCStringRef hashString = LCStringCreate(hash);
    LCCommitRef result = LCMutableDictionaryValueForKey(passed1, hashString);
    if (result) {
      return result;
    }
    LCMutableDictionarySetValueForKey(passed2, hashString, commits2Ref[i]);
    objectRelease(hashString);
  }
  for (LCInteger i=0; i<LCArrayLength(commits1); i++) {
    char hash[HASH_LENGTH];
    objectHash(commits1Ref[i], hash);
    LCStringRef hashString = LCStringCreate(hash);
    LCCommitRef result = LCMutableDictionaryValueForKey(passed2, hashString);
    if (result) {
      return result;
    }
    objectRelease(hashString);
  }
  
  LCMutableArrayRef commits1Parents = LCMutableArrayCreate(NULL, 0);
  for (LCInteger i=0; i<LCArrayLength(commits1); i++) {
    LCMutableArrayAddObjects(commits1Parents, LCCommitParents(commits1Ref[i]), LCCommitParentsLength(commits1Ref[i]));
  }
  LCMutableArrayRef commits2Parents = LCMutableArrayCreate(NULL, 0);
  for (LCInteger i=0; i<LCArrayLength(commits2); i++) {
    LCMutableArrayAddObjects(commits2Parents, LCCommitParents(commits2Ref[i]), LCCommitParentsLength(commits2Ref[i]));
  }
  LCCommitRef result = commitFindCommonParent(commits1Parents, commits2Parents, passed1, passed2);
  objectRelease(commits1Parents);
  objectRelease(commits2Parents);
  return result;
}

LCCommitRef LCCommitFindCommonParent(LCCommitRef commit1, LCCommitRef commit2) {
  LCMutableDictionaryRef passed1 = LCMutableDictionaryCreate(NULL, 0);
  LCMutableDictionaryRef passed2 = LCMutableDictionaryCreate(NULL, 0);
  LCArrayRef commits1 = LCArrayCreate(&commit1, 1);
  LCArrayRef commits2 = LCArrayCreate(&commit2, 1);
  LCCommitRef result = commitFindCommonParent(commits1, commits2, passed1, passed2);
  objectRelease(passed1);
  objectRelease(passed2);
  objectRelease(commits1);
  objectRelease(commits2);
  return result;
}

static void* pathArrayToPathString(LCInteger i, void* info, void* each) {
  LCKeyValueRef keyValue = (LCKeyValueRef)each;
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
