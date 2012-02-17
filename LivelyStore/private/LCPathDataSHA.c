
#include "LCPathDataSHA.h"

struct LCPathDataSHA {
  LCObjectInfo info;
  LCKeyValueRef keyValue;
};

void LCPathDataSHADealloc(void* object);

LCType typePathDataSHA = {
  .dealloc = LCPathDataSHADealloc
};

LCPathDataSHARef LCPathDataSHACreate(LCStringRef path, LCStringRef sha) {
  LCPathDataSHARef newPathData = malloc(sizeof(struct LCPathDataSHA));
  if (newPathData != NULL) {
    newPathData->info.type = &typePathDataSHA;
    newPathData->keyValue = LCKeyValueCreate(path, sha);
  }
  return newPathData;
};

LCStringRef LCPathDataSHAPath(LCPathDataSHARef pathDataSHA) {
  return LCKeyValueKey(pathDataSHA->keyValue);
}

LCStringRef LCPathDataSHASHA(LCPathDataSHARef pathDataSHA) {
  return LCKeyValueValue(pathDataSHA->keyValue);
}

void LCPathDataSHADealloc(void* object) {
  LCPathDataSHARef pathDataSHA = (LCPathDataSHARef)object;
  LCRelease(pathDataSHA->keyValue);
}