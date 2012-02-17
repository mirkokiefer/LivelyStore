
#include "LCPathValueSHA.h"

struct LCPathValueSHA {
  LCObjectInfo info;
  LCKeyValueRef keyValue;
};

void LCPathValueSHADealloc(void* object);

LCType typePathValueSHA = {
  .dealloc = LCPathValueSHADealloc
};

LCPathValueSHARef LCPathValueSHACreate(LCStringRef key, LCSHARef value) {
  LCPathValueSHARef newPathValue = malloc(sizeof(struct LCPathValueSHA));
  if (newPathValue != NULL) {
    newPathValue->info.type = &typePathValueSHA;
    newPathValue->keyValue = LCKeyValueCreate(key, value);
  }
  return newPathValue;
};

LCStringRef LCPathValueSHAPath(LCPathValueSHARef pathValueSHA) {
  return LCKeyValueKey(pathValueSHA->keyValue);
}

LCSHARef LCPathValueSHAValue(LCPathValueSHARef pathValueSHA) {
  return LCKeyValueValue(pathValueSHA->keyValue);
}

void LCPathValueSHADealloc(void* object) {
  LCPathValueSHARef pathValueSHA = (LCPathValueSHARef)object;
  LCRelease(pathValueSHA->keyValue);
}