
#include "LCStage.h"

struct LCKeyValue {
  LCObjectInfo info;
  LCStringRef key;
  LCDataRef value;
};

void LCKeyValueDealloc(void* object);

LCType typeKeyValue = {
  .dealloc = LCKeyValueDealloc
};

LCKeyValueRef LCKeyValueCreate(LCStringRef key, LCDataRef value) {
  LCKeyValueRef newKeyValue = malloc(sizeof(struct LCKeyValue));
  if (newKeyValue != NULL) {
    newKeyValue->info.type = &typeKeyValue;
    LCRetain(key);
    LCRetain(value);
    newKeyValue->key=key;
    newKeyValue->value=value;
  }
  return newKeyValue;
};

LCStringRef LCKeyValueKey(LCKeyValueRef keyValue) {
  return keyValue->key;
}

LCDataRef LCKeyValueValue(LCKeyValueRef keyValue) {
  return keyValue->value;
}

LCKeyValueSHARef LCKeyValueCreateKeyValueSHA(LCKeyValueRef keyValue) {
  return LCKeyValueSHACreate(keyValue->key, LCDataSHA1(keyValue->value));
}

void LCKeyValueDealloc(void* object) {
  LCKeyValueRef keyValue = (LCKeyValueRef)object;
  LCRelease(keyValue->key);
  LCRelease(keyValue->value);
}