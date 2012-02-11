
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

LCBool LCStageAddEntry(LCStageRef stage, char* key, unsigned char data[], size_t length) {  
  LCKeyValueRef* keyValues = realloc(stage->keyValues, (stage->length+1)*sizeof(LCKeyValueRef));
  if(keyValues) {
    LCStringRef lcKey = LCStringCreate(key);
    LCDataRef lcData = LCDataCreate(data, length);
    LCKeyValueRef keyValue = LCKeyValueCreate(lcKey, lcData);
    LCRelease(lcKey);
    LCRelease(lcData);

    keyValues[stage->length] = keyValue;
    stage->keyValues = keyValues;
    stage->length ++;
  } else {
    return false;
  }
  return true;
}

LCKeyValueRef* LCStageEntries(LCStageRef stage) {
  return stage->keyValues;
}

void LCStageDealloc(void* object) {
  LCStageRef commit = (LCStageRef)object;
  for(LCInteger i=0; i<commit->length; i++) {
    LCRelease(commit->keyValues[i]);
  }
  free(commit->keyValues);
}