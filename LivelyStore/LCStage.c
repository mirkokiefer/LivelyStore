
#include "LCStage.h"

struct LCStage {
  LCObjectInfo info;
  size_t addKeysLength;
  size_t deleteKeysLength;
  LCStringRef* deleteKeys;
  LCKeyValueRef* addKeys;
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
  LCKeyValueRef* keyValues = realloc(stage->addKeys, (stage->addKeysLength+1)*sizeof(LCKeyValueRef));
  if(keyValues) {
    LCStringRef lcKey = LCStringCreate(key);
    LCDataRef lcData = LCDataCreate(data, length);
    LCKeyValueRef keyValue = LCKeyValueCreate(lcKey, lcData);
    LCRelease(lcKey);
    LCRelease(lcData);

    keyValues[stage->addKeysLength] = keyValue;
    stage->addKeys = keyValues;
    stage->addKeysLength ++;
  } else {
    return false;
  }
  return true;
}

bool LCStageDeleteKey(LCStageRef stage, char* key) {
  LCStringRef* deleteKeys = realloc(stage->deleteKeys, (stage->deleteKeysLength+1)*sizeof(LCStringRef));
  if(deleteKeys) {
    
  } else {
    return false;
  }
  return true;
}

LCKeyValueRef* LCStageKeysToAdd(LCStageRef stage) {
  return stage->addKeys;
}

LCStringRef* LCStageKeysToDelete(LCStageRef stage) {
  return stage->deleteKeys;
}

size_t LCStageAddKeysCount(LCStageRef stage) {
  return stage->addKeysLength;
}

size_t LCStageDeleteKeysCount(LCStageRef stage) {
  return stage->deleteKeysLength;
}

void LCStageDealloc(void* object) {
  LCStageRef stage = (LCStageRef)object;
  for(LCInteger i=0; i<stage->addKeysLength; i++) {
    LCRelease(stage->addKeys[i]);
  }
  for(LCInteger i=0; i<stage->deleteKeysLength; i++) {
    LCRelease(stage->deleteKeys[i]);
  }
  free(stage->addKeys);
  free(stage->deleteKeys);
}
