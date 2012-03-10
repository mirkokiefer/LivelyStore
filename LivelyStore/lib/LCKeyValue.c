
#include "LCKeyValue.h"

struct LCKeyValue {
  LCObjectInfo info;
  void* key;
  void* value;
};

void LCKeyValueDealloc(void* object);
LCCompare LCKeyValueCompare(void* object1, void* object2);
void keyValuePrint(void* object, FILE* fd);

LCType typeKeyValue = {
  .dealloc = LCKeyValueDealloc,
  .compare = LCKeyValueCompare,
  .print = keyValuePrint
};

LCKeyValueRef LCKeyValueCreate(void* key, void* value) {
  LCKeyValueRef newKeyValue = LCNewObject(&typeKeyValue, sizeof(struct LCKeyValue));
  LCRetain(key);
  LCRetain(value);
  newKeyValue->key=key;
  newKeyValue->value=value;
  return newKeyValue;
};

void* LCKeyValueKey(LCKeyValueRef keyValue) {
  return keyValue->key;
}

void* LCKeyValueValue(LCKeyValueRef keyValue) {
  return keyValue->value;
}

LCCompare LCKeyValueCompare(void* object1, void* object2) {
  return LCCompareObjects(LCKeyValueKey(object1), LCKeyValueKey(object2));
}

void LCKeyValueDealloc(void* object) {
  LCKeyValueRef keyValue = (LCKeyValueRef)object;
  LCRelease(keyValue->key);
  LCRelease(keyValue->value);
}

void keyValuePrint(void* object, FILE* fd) {
  LCKeyValueRef kv = (LCKeyValueRef)object;
  LCPrint(LCKeyValueKey(kv), fd);
  fprintf(fd, " ");
  LCPrint(LCKeyValueValue(kv), fd);
}