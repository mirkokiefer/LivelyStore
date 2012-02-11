
#include "LCStage.h"

struct LCStage {
  LCObjectInfo info;
  size_t length;
  LCKeyValueRef* keyValues;
};

void LCStageDealloc(void* object);

LCType typeStage = {
  .dealloc = LCStageDealloc
};

LCStageRef LCStageCreate() {
  LCStageRef newCommit = malloc(sizeof(struct LCStage));
  if (newCommit != NULL) {
    newCommit->info.type = &typeStage;
  }
  return newCommit;
};

LCBool LCStageAddEntry(LCStageRef commit, LCKeyValueRef keyValue) {
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

size_t LCStageEntryCount(LCStageRef commit) {
  return commit->length;
}

void LCStageEntries(LCStageRef commit, LCKeyValueRef buffer[]) {
  memcpy(buffer, commit->keyValues, commit->length*sizeof(LCKeyValueRef));
}

void LCStageDealloc(void* object) {
  LCStageRef commit = (LCStageRef)object;
  for(LCInteger i=0; i<commit->length; i++) {
    LCRelease(commit->keyValues[i]);
  }
  free(commit->keyValues);
}