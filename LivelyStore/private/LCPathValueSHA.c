
#include "LCPathValueSHA.h"

struct LCPathValueSHA {
  LCObjectInfo info;
  LCStringRef key;
  LCSHARef value;
};

void LCPathValueSHADealloc(void* object);

LCType typePathValueSHA = {
  .dealloc = LCPathValueSHADealloc
};

LCPathValueSHARef LCPathValueSHACreate(LCStringRef key, LCSHARef value) {
  LCPathValueSHARef newPathValue = malloc(sizeof(struct LCPathValueSHA));
  if (newPathValue != NULL) {
    newPathValue->info.type = &typePathValueSHA;
    LCRetain(key);
    LCRetain(value);
    newPathValue->key=key;
    newPathValue->value=value;
  }
  return newPathValue;
};

LCStringRef LCPathValueSHAPath(LCPathValueSHARef keyValue) {
  return keyValue->key;
}

LCSHARef LCPathValueSHAValue(LCPathValueSHARef keyValue) {
  return keyValue->value;
}

void LCPathValueSHADealloc(void* object) {
  LCPathValueSHARef keyValue = (LCPathValueSHARef)object;
  LCRelease(keyValue->key);
  LCRelease(keyValue->value);
}