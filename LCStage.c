
#include "LCStage.h"

struct LCStage {
  LCObjectInfo info;
  LCMutableArrayRef deletePaths;
  LCMutableArrayRef addPathValues;
};

void LCStageDealloc(void* object);

LCType typeStage = {
  .dealloc = LCStageDealloc
};

LCStageRef LCStageCreate() {
  LCStageRef newStage = LCNewObject(&typeStage, sizeof(struct LCStage));
  newStage->addPathValues = LCMutableArrayCreate(NULL, 0);
  newStage->deletePaths = LCMutableArrayCreate(NULL, 0);
  return newStage;
};

void LCStageAddEntry(LCStageRef stage, char* path, unsigned char data[], size_t length) {  
  LCStringRef lcPath = LCStringCreate(path);
  LCArrayRef pathArray = createPathArray(lcPath);
  LCDataRef lcData = LCDataCreate(data, length);
  LCKeyValueRef keyValue = LCKeyValueCreate(pathArray, lcData);
  LCRelease(lcPath);
  LCRelease(pathArray);
  LCRelease(lcData);
  
  LCMutableArrayAddObject(stage->addPathValues, keyValue);
}

void LCStageDeletePath(LCStageRef stage, char* path) {
  LCStringRef lcPath = LCStringCreate(path);
  LCArrayRef pathArray = createPathArray(lcPath);
  LCMutableArrayAddObject(stage->deletePaths, pathArray);
  LCRelease(lcPath);
  LCRelease(pathArray);
}

LCKeyValueRef* LCStagePathsToAdd(LCStageRef stage) {
  return (LCKeyValueRef*)LCMutableArrayObjects(stage->addPathValues);
}

LCStringRef* LCStagePathsToDelete(LCStageRef stage) {
  return (LCStringRef*)LCMutableArrayObjects(stage->deletePaths);
}

size_t LCStageAddPathsCount(LCStageRef stage) {
  return LCMutableArrayLength(stage->addPathValues);
}

size_t LCStageDeletePathsCount(LCStageRef stage) {
  return LCMutableArrayLength(stage->deletePaths);
}

void LCStageDealloc(void* object) {
  LCStageRef stage = (LCStageRef)object;
  LCRelease(stage->addPathValues);
  LCRelease(stage->deletePaths);
}
