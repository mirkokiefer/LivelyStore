
#include "LCStage.h"

typedef struct stageData* stageDataRef;

struct stageData {
  LCMutableArrayRef deletePaths;
  LCMutableArrayRef addPathValues;
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
    newStage->addPathValues = LCMutableArrayCreate(NULL, 0);
    newStage->deletePaths = LCMutableArrayCreate(NULL, 0);
    return objectCreate(LCTypeStage, newStage);
  } else {
    return NULL;
  }
};

static LCMutableArrayRef stageAddPaths(LCStageRef stage) {
  stageDataRef stageData = objectData(stage);
  return stageData->addPathValues;
}


static LCMutableArrayRef stageDeletePaths(LCStageRef stage) {
  stageDataRef stageData = objectData(stage);
  return stageData->deletePaths;
}

void LCStageAddEntry(LCStageRef stage, char* path, unsigned char data[], size_t length) {  
  LCStringRef lcPath = LCStringCreate(path);
  LCArrayRef pathArray = createPathArray(lcPath);
  LCDataRef lcData = LCDataCreate(data, length);
  LCKeyValueRef keyValue = LCKeyValueCreate(pathArray, lcData);
  objectRelease(lcPath);
  objectRelease(pathArray);
  objectRelease(lcData);
  LCMutableArrayAddObject(stageAddPaths(stage), keyValue);
}

void LCStageDeletePath(LCStageRef stage, char* path) {
  LCStringRef lcPath = LCStringCreate(path);
  LCArrayRef pathArray = createPathArray(lcPath);
  LCMutableArrayAddObject(stageDeletePaths(stage), pathArray);
  objectRelease(lcPath);
  objectRelease(pathArray);
}

LCKeyValueRef* LCStagePathsToAdd(LCStageRef stage) {
  return LCMutableArrayObjects(stageAddPaths(stage));
}

LCArrayRef* LCStagePathsToDelete(LCStageRef stage) {
  return LCMutableArrayObjects(stageDeletePaths(stage));
}

size_t LCStageAddPathsCount(LCStageRef stage) {
  return LCMutableArrayLength(stageAddPaths(stage));
}

size_t LCStageDeletePathsCount(LCStageRef stage) {
  return LCMutableArrayLength(stageDeletePaths(stage));
}

void stageDealloc(LCObjectRef stage) {
  objectRelease(stageAddPaths(stage));
  objectRelease(stageDeletePaths(stage));
}
