
#include "LCCommitStage.h"

struct LCCommitStage {
  LCObjectMeta meta;
  size_t length;
  LCKeyValueRef* keyValues;
};

void LCCommitStageDealloc(void* object);

LCCommitStageRef LCCommitStageCreate() {
  LCCommitStageRef newCommit = malloc(sizeof(struct LCCommitStage));
  if (newCommit != NULL) {
    newCommit->meta.dealloc = LCCommitStageDealloc;
  }
  return newCommit;
};

LCBool LCCommitStageAddEntry(LCCommitStageRef commit, LCKeyValueRef keyValue) {
  LCKeyValueRef* keyValues = realloc(commit->keyValues, (commit->length+1)*sizeof(keyValue));
  if(keyValues) {
    LCRetain(keyValue);
    keyValues[commit->length] = keyValue;
    commit->keyValues = keyValues;
    commit->length = commit->length + 1;
  } else {
    return false;
  }
  return true;
}

size_t LCCommitStageEntryCount(LCCommitStageRef commit) {
  return commit->length;
}

void LCCommitStageEntries(LCCommitStageRef commit, LCKeyValueRef buffer[]) {
  memcpy(buffer, commit->keyValues, commit->length*sizeof(LCKeyValueRef));
}

void LCCommitStageDealloc(void* object) {
  LCCommitStageRef commit = (LCCommitStageRef)object;
  for(LCInteger i=0; i<commit->length; i++) {
    LCRelease(commit->keyValues[i]);
  }
  free(commit->keyValues);
}