
#include "LCCommit.h"

struct LCCommit {
  LCObjectInfo info;
  LCSHARef parentSHA;
  LCSHARef treeSHA;
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
    LCSHARef parentSHA = LCCommitSHA(parent);
    LCSHARef treeSHA = LCTreeSHA(tree);
    newCommit->parentSHA = LCRetain(parentSHA);
    newCommit->treeSHA = LCRetain(treeSHA);
  }
  return newCommit;
};

LCSHARef LCCommitSHA(LCCommitRef commit) {
  if(commit->sha == NULL) {
    commit->sha = LCSHACreateFromHashableObject(commit);
  }
  return commit->sha;
}

LCDataArrayRef LCCommitDataArray(void* object) {
  LCCommitRef commit = (LCCommitRef)object;
  LCDataRef datas[2] = {LCSHASHAData(commit->parentSHA), LCSHASHAData(commit->treeSHA)};
  return LCDataArrayCreate(datas, 2);
}

void LCCommitDealloc(void* object) {
  LCCommitRef commit = (LCCommitRef)object;
  LCRelease(commit->parentSHA);
  LCRelease(commit->treeSHA);
}