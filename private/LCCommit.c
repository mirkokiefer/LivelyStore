
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
  .initData = commitInitData
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

void commitDealloc(LCCommitRef object) {
  commitDataRef data = objectData(object);
  objectRelease(data->parents);
  objectRelease(data->tree);
}

void commitWalkChildren(LCCommitRef commit, void *cookie, childCallback cb) {
  cb(cookie, "parents", LCCommitParents(commit), LCCommitParentsLength(commit), false);
  LCTreeRef tree = LCCommitTree(commit);
  cb(cookie, "tree", &tree, 1, false);
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
