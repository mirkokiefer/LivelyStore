
#include "LCKeyValue.h"

struct LCKeyValue {
  LCObjectInfo info;
  void* key;
  void* value;
};

void LCKeyValueDealloc(void* object);

LCType typeKeyValue = {
  .dealloc = LCKeyValueDealloc
};

LCKeyValueRef LCKeyValueCreate(void* key, void* value) {
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

void* LCKeyValueKey(LCKeyValueRef keyValue) {
  return keyValue->key;
}

void* LCKeyValueValue(LCKeyValueRef keyValue) {
  return keyValue->value;
}

void LCKeyValueDealloc(void* object) {
  LCKeyValueRef keyValue = (LCKeyValueRef)object;
  LCRelease(keyValue->key);
  LCRelease(keyValue->value);
}