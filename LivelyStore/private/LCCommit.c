
#include "LCCommit.h"

struct LCCommit {
  LCObjectInfo info;
  LCCommitRef parent;
  LCTreeRef tree;
  LCSHARef sha;
};

LCDataArrayRef LCCommitDataArray(void* object);
void LCCommitDealloc(void* object);

LCHashableObject hashableCommit = {
  .dataArrayCopy = LCCommitDataArray
};

LCType typeCommit = {
  .dealloc = LCCommitDealloc
};

LCCommitRef LCCommitCreate(LCCommitRef parent, LCTreeRef tree) {
  LCCommitRef newCommit = malloc(sizeof(struct LCCommit));
  if (newCommit != NULL) {
    newCommit->info.type = &typeCommit;
    newCommit->parent = LCRetain(parent);
    newCommit->tree = LCRetain(tree);
  }
  return newCommit;
};

LCTreeRef LCCommitTree(LCCommitRef commit) {
  return commit->tree;
}

LCCommitRef LCCommitParent(LCCommitRef commit) {
  return commit->parent;
}

LCSHARef LCCommitSHA(LCCommitRef commit) {
  if(commit->sha == NULL) {
    commit->sha = LCSHACreateFromHashableObject(commit);
  }
  return commit->sha;
}

LCDataArrayRef LCCommitDataArray(void* object) {
  LCCommitRef commit = (LCCommitRef)object;
  LCSHARef parentSHA = LCCommitSHA(commit->parent);
  LCSHARef treeSHA = LCTreeSHA(commit->tree);
  LCDataRef datas[2] = {LCSHASHAData(parentSHA), LCSHASHAData(treeSHA)};
  return LCDataArrayCreate(datas, 2);
}

void LCCommitDealloc(void* object) {
  LCCommitRef commit = (LCCommitRef)object;
  LCRelease(commit->parent);
  LCRelease(commit->tree);
}