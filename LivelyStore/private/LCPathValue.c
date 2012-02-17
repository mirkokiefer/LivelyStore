
#include "LCStage.h"

struct LCPathValue {
  LCObjectInfo info;
  LCKeyValueRef keyValue;
};

void LCPathValueDealloc(void* object);

LCType typePathValue = {
  .dealloc = LCPathValueDealloc
};

LCPathValueRef LCPathValueCreate(LCStringRef key, LCDataRef value) {
  LCPathValueRef newPathValue = malloc(sizeof(struct LCPathValue));
  if (newPathValue != NULL) {
    newPathValue->info.type = &typePathValue;
    newPathValue->keyValue = LCKeyValueCreate(key, value);
  }
  return newPathValue;
};

LCStringRef LCPathValuePath(LCPathValueRef pathValue) {
  return LCKeyValueKey(pathValue->keyValue);
}

LCDataRef LCPathValueValue(LCPathValueRef pathValue) {
  return LCKeyValueValue(pathValue->keyValue);
}

LCPathValueSHARef LCPathValueCreatePathValueSHA(LCPathValueRef pathValue) {
  return LCPathValueSHACreate(LCPathValuePath(pathValue), LCDataSHA1(LCPathValueValue(pathValue)));
}

void LCPathValueDealloc(void* object) {
  LCPathValueRef pathValue = (LCPathValueRef)object;
  LCRelease(pathValue->keyValue);
}