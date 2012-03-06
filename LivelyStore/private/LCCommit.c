
#include "LCCommit.h"

struct LCCommit {
  LCObjectInfo info;
  LCCommitRef parent;
  LCTreeRef tree;
  LCStringRef sha;
};

void LCCommitDealloc(void* object);

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

LCStringRef LCCommitSHA(LCCommitRef commit) {
  if(commit->sha == NULL) {
    LCStringRef serialized = LCCommitCreateSerializedString(commit);
    commit->sha = LCStringCreateSHAString(serialized);
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
  char string[LC_SHA1_Length + 1 + LC_SHA1_Length];
  sprintf(string, "%s\n%s", parentSHA, treeSHA);
  return LCStringCreate(string);
}

void LCCommitDealloc(void* object) {
  LCCommitRef commit = (LCCommitRef)object;
  LCRelease(commit->parent);
  LCRelease(commit->tree);
}