
#include "LCStage.h"

struct LCPathValue {
  LCObjectInfo info;
  LCStringRef key;
  LCDataRef value;
};

void LCPathValueDealloc(void* object);

LCType typePathValue = {
  .dealloc = LCPathValueDealloc
};

LCPathValueRef LCPathValueCreate(LCStringRef key, LCDataRef value) {
  LCPathValueRef newPathValue = malloc(sizeof(struct LCPathValue));
  if (newPathValue != NULL) {
    newPathValue->info.type = &typePathValue;
    LCRetain(key);
    LCRetain(value);
    newPathValue->key=key;
    newPathValue->value=value;
  }
  return newPathValue;
};

LCStringRef LCPathValuePath(LCPathValueRef keyValue) {
  return keyValue->key;
}

LCDataRef LCPathValueValue(LCPathValueRef keyValue) {
  return keyValue->value;
}

LCPathValueSHARef LCPathValueCreatePathValueSHA(LCPathValueRef keyValue) {
  return LCPathValueSHACreate(keyValue->key, LCDataSHA1(keyValue->value));
}

void LCPathValueDealloc(void* object) {
  LCPathValueRef keyValue = (LCPathValueRef)object;
  LCRelease(keyValue->key);
  LCRelease(keyValue->value);
}