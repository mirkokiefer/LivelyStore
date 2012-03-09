
#include "LCCommit.h"

struct LCCommit {
  LCObjectInfo info;
  LCDataStoreRef store;
  LCCommitRef parent;
  LCTreeRef tree;
  LCStringRef sha;
};

void LCCommitDealloc(void* object);
void commitDeserialize(LCCommitRef commit);

LCType typeCommit = {
  .dealloc = LCCommitDealloc
};

LCCommitRef LCCommitCreateFromSHA(LCDataStoreRef store, LCStringRef sha) {
  LCCommitRef newCommit = LCNewObject(&typeCommit, sizeof(struct LCCommit));
  newCommit->sha = LCRetain(sha);
  newCommit->store = LCRetain(store);
  newCommit->parent = NULL;
  newCommit->tree = NULL;
  return newCommit;
}

LCCommitRef LCCommitCreate(LCDataStoreRef store, LCCommitRef parent, LCTreeRef tree) {
  LCCommitRef newCommit = LCNewObject(&typeCommit, sizeof(struct LCCommit));
  newCommit->sha = NULL;
  newCommit->store = LCRetain(store);
  newCommit->parent = LCRetain(parent);
  newCommit->tree = LCRetain(tree);
  return newCommit;
};

LCTreeRef LCCommitTree(LCCommitRef commit) {
  if((commit->tree == NULL) && commit->sha) {
    commitDeserialize(commit);
  }
  return commit->tree;
}

LCCommitRef LCCommitParent(LCCommitRef commit) {
  if((commit->parent == NULL) && commit->sha) {
    commitDeserialize(commit);
  }
  return commit->parent;
}

LCStringRef LCCommitSHA(LCCommitRef commit) {
  if(commit->sha == NULL) {
    LCStringRef serialized = LCCommitCreateSerializedString(commit);
    commit->sha = LCStringCreateSHAString(serialized);
    LCDataStorePutCommitData(commit->store, commit->sha, serialized);
    LCRelease(serialized);
  }
  return commit->sha;
}

LCStringRef LCCommitCreateSerializedString(LCCommitRef commit) {
  char* parentSHA = "";
  if (commit->parent) {
    parentSHA = LCStringStringRef(LCCommitSHA(commit->parent));
  }
  char* treeSHA = "";
  if (commit->tree) {
    treeSHA = LCStringStringRef(LCTreeSHA(commit->tree));
  }
  char string[LC_SHA1_HEX_Length + 1 + LC_SHA1_HEX_Length];
  sprintf(string, "%s\n%s", parentSHA, treeSHA);
  return LCStringCreate(string);
}

void LCCommitDealloc(void* object) {
  LCCommitRef commit = (LCCommitRef)object;
  LCRelease(commit->parent);
  LCRelease(commit->tree);
  LCRelease(commit->sha);
}

void commitDeserialize(LCCommitRef commit) {
  LCStringRef data = LCDataStoreGetCommitData(commit->store, commit->sha);
  if ((data == NULL) || ((LCStringStringRef(data))[0] == '\n')) {
    return;
  }
  char parentSHA[LC_SHA1_HEX_Length];
  char treeSHA[LC_SHA1_HEX_Length];
  sscanf(LCStringStringRef(data), "%s\n%s", parentSHA, treeSHA);
  LCStringRef parentSHAObj = LCStringCreate(parentSHA);
  LCStringRef treeSHAObj = LCStringCreate(treeSHA);
  commit->parent = LCCommitCreateFromSHA(commit->store, parentSHAObj);
  commit->tree = LCTreeCreateFromSHA(commit->store, treeSHAObj);
  LCRelease(parentSHAObj);
  LCRelease(treeSHAObj);
}