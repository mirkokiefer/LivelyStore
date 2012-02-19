
#include "LCArray.h"

void LCArrayDealloc(void* object);

struct LCArray {
  LCObjectInfo info;
  size_t length;
  void* objects[];
};

LCType typeArray = {
  .dealloc = LCArrayDealloc
};

LCArrayRef LCArrayCreate(void** objects, size_t length) {
  LCArrayRef newArray = malloc(sizeof(struct LCArray) + length * sizeof(void*));
  if (newArray != NULL) {
    newArray->info.type = &typeArray;
    for(LCInteger i=0; i<length; i++) {
      LCRetain(objects[i]);
    }
    newArray->length = length;
    memcpy(newArray->objects, objects, length * sizeof(void*));
  }
  return newArray;
};

void** LCArrayObjects(LCArrayRef array) {
  return array->objects;
}

void* LCArrayObjectAtIndex(LCArrayRef array, LCInteger index) {
  return array->objects[index];
}

size_t LCArrayLength(LCArrayRef array) {
  return array->length;
}

LCArrayRef LCArrayCreateSubArray(LCArrayRef array, LCInteger start, size_t length) {
  if (length == -1) {
    length = array->length-start;
  }
  return LCArrayCreate(&(array->objects[start]), length);
}

void LCArrayDealloc(void* object) {
  LCArrayRef array = (LCArrayRef)object;
  for (LCInteger i=0; i<array->length; i++) {
    LCRelease(array->objects[i]);
  }
}
