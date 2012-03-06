
#include "LCArray.h"

LCCompare LCArrayCompare(void* object1, void* object2);
void LCArrayDealloc(void* object);

struct LCArray {
  LCObjectInfo info;
  size_t length;
  void* objects[];
};

LCType typeArray = {
  .dealloc = LCArrayDealloc,
  .compare = LCArrayCompare
};

LCArrayRef LCArrayCreate(void** objects, size_t length) {
  LCArrayRef newArray = LCNewObject(&typeArray, sizeof(struct LCArray) + length * sizeof(void*));
  for(LCInteger i=0; i<length; i++) {
    LCRetain(objects[i]);
  }
  newArray->length = length;
  memcpy(newArray->objects, objects, length * sizeof(void*));
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

LCMutableArrayRef LCArrayCreateMutableArray(LCArrayRef array) {
  return LCMutableArrayCreate(array->objects, array->length);
}

LCCompare LCArrayCompare(void* object1, void* object2) {
  LCArrayRef array1 = (LCArrayRef)object1;
  LCArrayRef array2 = (LCArrayRef)object2;
  return LCCompareObjects(array1->objects[0], array2->objects[0]);
}

void LCArrayDealloc(void* object) {
  LCArrayRef array = (LCArrayRef)object;
  for (LCInteger i=0; i<array->length; i++) {
    LCRelease(array->objects[i]);
  }
}
