
#include "LCStage.h"

typedef struct stageData* stageDataRef;

struct stageData {
  LCMutableDictionaryRef updates;
};

void stageDealloc(LCObjectRef object);

struct LCType typeStage = {
  .name = "LCStage",
  .immutable = false,
  .dealloc = stageDealloc
};

LCTypeRef LCTypeStage = &typeStage;

LCStageRef LCStageCreate() {
  stageDataRef newStage = malloc(sizeof(struct stageData));
  if (newStage) {
    newStage->updates = LCMutableDictionaryCreate(NULL, 0);
    return objectCreate(LCTypeStage, newStage);
  } else {
    return NULL;
  }
};

static LCMutableDictionaryRef stageUpdates(LCStageRef stage) {
  stageDataRef stageData = objectData(stage);
  return stageData->updates;
}

void LCStageAddEntry(LCStageRef stage, char* path, LCObjectRef data) {  
  LCStringRef lcPath = LCStringCreate(path);
  LCArrayRef pathArray = createPathArray(lcPath);
  LCMutableDictionarySetValueForKey(stageUpdates(stage), pathArray, data);
  objectRelease(lcPath);
  objectRelease(pathArray);
}

void LCStageDeletePath(LCStageRef stage, char* path) {
  LCStringRef lcPath = LCStringCreate(path);
  LCArrayRef pathArray = createPathArray(lcPath);
  LCMutableDictionarySetValueForKey(stageUpdates(stage), pathArray, NULL);
  objectRelease(lcPath);
  objectRelease(pathArray);
}

void LCStageAddKeyValues(LCStageRef stage, LCKeyValueRef keyValues[], size_t length) {
  for (LCInteger i=0; i<length; i++) {
    LCArrayRef pathArray = createPathArray(LCKeyValueKey(keyValues[i]));
    LCKeyValueRef pathValue = LCKeyValueCreate(pathArray, LCKeyValueValue(keyValues[i]));
    LCMutableDictionaryAddEntry(stageUpdates(stage), pathValue);
    objectRelease(pathArray);
    objectRelease(pathValue);
  }
}

LCKeyValueRef* LCStageUpdates(LCStageRef stage) {
  return LCMutableDictionaryEntries(stageUpdates(stage));
}

size_t LCStageUpdatesLength(LCStageRef stage) {
  return LCMutableDictionaryLength(stageUpdates(stage));
}

void stageDealloc(LCObjectRef stage) {
  objectRelease(stageUpdates(stage));
}
