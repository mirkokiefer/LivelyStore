
#include "LCStage.h"

struct LCPathData {
  LCObjectInfo info;
  LCKeyValueRef keyValue;
};

void LCPathDataDealloc(void* object);

LCType typePathData = {
  .dealloc = LCPathDataDealloc
};

LCPathDataRef LCPathDataCreate(LCStringRef key, LCDataRef value) {
  LCPathDataRef newPathData = malloc(sizeof(struct LCPathData));
  if (newPathData != NULL) {
    newPathData->info.type = &typePathData;
    newPathData->keyValue = LCKeyValueCreate(key, value);
  }
  return newPathData;
};

LCStringRef LCPathDataPath(LCPathDataRef pathData) {
  return LCKeyValueKey(pathData->keyValue);
}

LCDataRef LCPathDataValue(LCPathDataRef pathData) {
  return LCKeyValueValue(pathData->keyValue);
}

LCPathDataSHARef LCPathDataCreatePathDataSHA(LCPathDataRef pathData) {
  return LCPathDataSHACreate(LCPathDataPath(pathData), LCDataSHA1(LCPathDataValue(pathData)));
}

void LCPathDataDealloc(void* object) {
  LCPathDataRef pathData = (LCPathDataRef)object;
  LCRelease(pathData->keyValue);
}