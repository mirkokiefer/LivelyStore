
#include "LCCommitStage.h"

struct LCKeyValue {
  LCObjectMeta meta;
  LCStringRef key;
  LCBlobRef value;
};

void LCKeyValueDealloc(void* object);

LCKeyValueRef LCKeyValueCreate(LCStringRef key, LCBlobRef value) {
  LCKeyValueRef newKeyValue = malloc(sizeof(struct LCKeyValue));
  if (newKeyValue != NULL) {
    newKeyValue->meta.dealloc = LCKeyValueDealloc;
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

LCBlobRef LCKeyValueValue(LCKeyValueRef keyValue) {
  return keyValue->value;
}

void LCKeyValueDealloc(void* object) {
  LCKeyValueRef keyValue = (LCKeyValueRef)object;
  LCRelease(keyValue->key);
  LCRelease(keyValue->value);
}