
#include "LCCommit.h"

struct LCCommit {
  LCObjectInfo info;
  LCBackendWrapperRef store;
  LCArrayRef parents;
  LCTreeRef tree;
  LCStringRef sha;
};

static LCArrayRef commitParents(LCCommitRef commit);

void LCCommitDealloc(void* object);
void commitDeserialize(LCCommitRef commit);
LCCommitRef findCommit(LCCommitRef commits[], size_t length, LCStringRef sha);

LCType typeCommit = {
  .dealloc = LCCommitDealloc
};

static LCArrayRef commitParents(LCCommitRef commit) {
  if((commit->parents == NULL) && commit->sha) {
    commitDeserialize(commit);
  }
  return commit->parents;
}

LCCommitRef LCCommitCreateFromSHA(LCBackendWrapperRef store, LCStringRef sha) {
  LCCommitRef newCommit = LCNewObject(&typeCommit, sizeof(struct LCCommit));
  newCommit->sha = LCRetain(sha);
  newCommit->store = LCRetain(store);
  newCommit->parents = NULL;
  newCommit->tree = NULL;
  return newCommit;
}

LCCommitRef LCCommitCreate(LCBackendWrapperRef store, LCTreeRef tree, LCCommitRef parents[], size_t parentsLength) {
  LCCommitRef newCommit = LCNewObject(&typeCommit, sizeof(struct LCCommit));
  newCommit->sha = NULL;
  newCommit->store = LCRetain(store);
  newCommit->parents = LCArrayCreate((void**)parents, parentsLength);
  newCommit->tree = LCRetain(tree);
  return newCommit;
};

LCTreeRef LCCommitTree(LCCommitRef commit) {
  if((commit->tree == NULL) && commit->sha) {
    commitDeserialize(commit);
  }
  return commit->tree;
}

LCCommitRef* LCCommitParents(LCCommitRef commit) {
  return (LCCommitRef*)LCArrayObjects(commitParents(commit));
}

size_t LCCommitParentsLength(LCCommitRef commit) {
  return LCArrayLength(commitParents(commit));
}

LCStringRef LCCommitSHA(LCCommitRef commit) {
  if(commit->sha == NULL) {
    LCStringRef serialized = LCCommitCreateSerializedString(commit);
    commit->sha = LCStringCreateSHAString(serialized);
    LCBackendWrapperPutCommitData(commit->store, commit->sha, serialized);
    LCRelease(serialized);
  }
  return commit->sha;
}

LCStringRef LCCommitCreateSerializedString(LCCommitRef commit) {
  size_t parentsLength = LCArrayLength(commitParents(commit));
  
  char* treeSHA = "";
  if (commit->tree) {
    treeSHA = LCStringStringRef(LCTreeSHA(commit->tree));
  }
  
  char string[(LC_SHA1_HEX_Length + 1) * (1+parentsLength)];
  strcpy(string, treeSHA);
  strcat(string, "\n");
  
  for (LCInteger i=0; i<parentsLength; i++) {
    LCCommitRef parent = LCArrayObjectAtIndex(commitParents(commit), i);
    LCStringRef parentSHA = LCCommitSHA(parent);
    strcat(string, LCStringStringRef(parentSHA));
    strcat(string, "\n");
  }
  return LCStringCreate(string);
}

LCCommitRef LCCommitFindParent(LCCommitRef commit, LCStringRef sha) {
  return findCommit(&commit, 1, sha);
}

void LCCommitDealloc(void* object) {
  LCCommitRef commit = (LCCommitRef)object;
  LCRelease(commit->parents);
  LCRelease(commit->tree);
  LCRelease(commit->sha);
}

static void* shaStringToCommit(LCInteger i, void* info, void* shaString) {
  return LCCommitCreateFromSHA(info, shaString);
}

void commitDeserialize(LCCommitRef commit) {
  LCStringRef data = LCBackendWrapperGetCommitData(commit->store, commit->sha);
  if ((data == NULL) || ((LCStringStringRef(data))[0] == '\n')) {
    return;
  }
  LCArrayRef tokens = LCStringCreateTokens(data, '\n');
  LCArrayRef lines = LCArrayCreateSubArray(tokens, 0, LCArrayLength(tokens)-1);
  LCStringRef treeSHA = LCArrayObjectAtIndex(lines, 0);
  LCArrayRef parentSHAs = LCArrayCreateSubArray(lines, 1, -1);
  commit->tree = LCTreeCreateFromSHA(commit->store, treeSHA);
  commit->parents = LCArrayCreateArrayWithMap(parentSHAs, commit->store, shaStringToCommit);
  
  LCRelease(tokens);
  LCRelease(lines);
  LCRelease(parentSHAs);
}

LCCommitRef findCommit(LCCommitRef commits[], size_t length, LCStringRef sha) {
  if (length == 0) {
    return NULL;
  }
  for (LCInteger i=0; i<length; i++) {
    if(LCStringEqual(LCCommitSHA(commits[i]), sha)) {
      return commits[i];
    }
  }
  LCMutableArrayRef parents = LCMutableArrayCreate(NULL, 0);
  for (LCInteger i=0; i<length; i++) {
    LCMutableArrayAddObjects(parents, (void**)LCCommitParents(commits[i]), LCCommitParentsLength(commits[i]));
  }
  LCCommitRef result = findCommit((LCCommitRef*)LCMutableArrayObjects(parents), LCMutableArrayLength(parents), sha);
  LCRelease(parents);
  return result;
}
