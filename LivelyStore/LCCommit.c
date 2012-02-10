
#include "LCCommit.h"

struct LCCommit {
  LCObjectMeta meta;
  size_t length;
  LCKeyValueRef* keyValues;
};

void LCCommitDealloc(void* object);

LCCommitRef LCCommitCreate() {
  LCCommitRef newCommit = malloc(sizeof(struct LCCommit));
  if (newCommit != NULL) {
    newCommit->meta.dealloc = LCCommitDealloc;
  }
  return newCommit;
};

//actually needs to return error
void LCCommitAddEntry(LCCommitRef commit, LCKeyValueRef keyValue) {
  LCKeyValueRef* keyValues = realloc(commit->keyValues, (commit->length+1)*sizeof(keyValue));
  if(keyValues) {
    LCRetain(keyValue);
    keyValues[commit->length] = keyValue;
    commit->keyValues = keyValues;
    commit->length = commit->length + 1;
  }
}

size_t LCCommitEntryCount(LCCommitRef commit) {
  return commit->length;
}

void LCCommitEntries(LCCommitRef commit, LCKeyValueRef buffer[]) {
  memcpy(buffer, commit->keyValues, commit->length*sizeof(LCKeyValueRef));
}

void LCCommitDealloc(void* object) {
  LCCommitRef commit = (LCCommitRef)object;
  for(LCInteger i=0; i<commit->length; i++) {
    LCRelease(commit->keyValues[i]);
  }
  free(commit->keyValues);
}