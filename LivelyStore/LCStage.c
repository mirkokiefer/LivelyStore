
#include "LCStage.h"

struct LCStage {
  LCObjectInfo info;
  size_t addPathsLength;
  size_t deletePathsLength;
  LCStringRef* deletePaths;
  LCPathValueRef* addPaths;
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

LCBool LCStageAddEntry(LCStageRef stage, char* path, unsigned char data[], size_t length) {  
  LCPathValueRef* keyValues = realloc(stage->addPaths, (stage->addPathsLength+1)*sizeof(LCPathValueRef));
  if(keyValues) {
    LCStringRef lcPath = LCStringCreate(path);
    LCDataRef lcData = LCDataCreate(data, length);
    LCPathValueRef keyValue = LCPathValueCreate(lcPath, lcData);
    LCRelease(lcPath);
    LCRelease(lcData);

    keyValues[stage->addPathsLength] = keyValue;
    stage->addPaths = keyValues;
    stage->addPathsLength ++;
  } else {
    return false;
  }
  return true;
}

bool LCStageDeletePath(LCStageRef stage, char* key) {
  LCStringRef* deletePaths = realloc(stage->deletePaths, (stage->deletePathsLength+1)*sizeof(LCStringRef));
  if(deletePaths) {
    
  } else {
    return false;
  }
  return true;
}

LCPathValueRef* LCStagePathsToAdd(LCStageRef stage) {
  return stage->addPaths;
}

LCStringRef* LCStagePathsToDelete(LCStageRef stage) {
  return stage->deletePaths;
}

size_t LCStageAddPathsCount(LCStageRef stage) {
  return stage->addPathsLength;
}

size_t LCStageDeletePathsCount(LCStageRef stage) {
  return stage->deletePathsLength;
}

void LCStageDealloc(void* object) {
  LCStageRef stage = (LCStageRef)object;
  for(LCInteger i=0; i<stage->addPathsLength; i++) {
    LCRelease(stage->addPaths[i]);
  }
  for(LCInteger i=0; i<stage->deletePathsLength; i++) {
    LCRelease(stage->deletePaths[i]);
  }
  free(stage->addPaths);
  free(stage->deletePaths);
}
