
#include "LCCommit.h"

struct LCCommit {
  LCObjectInfo info;
  LCSHARef parentSHA;
  LCSHARef treeSHA;
  LCSHARef sha;
};

LCBlobArrayRef LCCommitBlobArray(void* object);
void LCCommitDealloc(void* object);

LCHashableObject hashableCommit = {
  .blobArrayCopy = LCCommitBlobArray
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

LCBlobArrayRef LCCommitBlobArray(void* object) {
  LCCommitRef commit = (LCCommitRef)object;
  LCBlobRef blobs[2] = {LCSHASHABlob(commit->parentSHA), LCSHASHABlob(commit->treeSHA)};
  return LCBlobArrayCreate(blobs, 2);
}

void LCCommitDealloc(void* object) {
  LCCommitRef commit = (LCCommitRef)object;
  LCRelease(commit->parentSHA);
  LCRelease(commit->treeSHA);
}