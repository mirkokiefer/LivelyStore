
#include "LCKeyValueSHA.h"

struct LCKeyValueSHA {
  LCObjectInfo info;
  LCStringRef key;
  LCSHARef value;
};

void LCKeyValueSHADealloc(void* object);

LCType typeKeyValueSHA = {
  .dealloc = LCKeyValueSHADealloc
};

LCKeyValueSHARef LCKeyValueSHACreate(LCStringRef key, LCSHARef value) {
  LCKeyValueSHARef newKeyValue = malloc(sizeof(struct LCKeyValueSHA));
  if (newKeyValue != NULL) {
    newKeyValue->info.type = &typeKeyValueSHA;
    LCRetain(key);
    LCRetain(value);
    newKeyValue->key=key;
    newKeyValue->value=value;
  }
  return newKeyValue;
};

LCStringRef LCKeyValueSHAKey(LCKeyValueSHARef keyValue) {
  return keyValue->key;
}

LCSHARef LCKeyValueSHAValue(LCKeyValueSHARef keyValue) {
  return keyValue->value;
}

void LCKeyValueSHADealloc(void* object) {
  LCKeyValueSHARef keyValue = (LCKeyValueSHARef)object;
  LCRelease(keyValue->key);
  LCRelease(keyValue->value);
}