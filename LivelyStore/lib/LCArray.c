
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

LCArrayRef LCArrayCreateFromArrays(LCArrayRef arrays[], size_t length) {
  size_t totalLength = 0;
  for (LCInteger i=0; i<length; i++) {
    totalLength = totalLength + LCArrayLength(arrays[i]);
  }
  
  LCArrayRef newArray = LCNewObject(&typeArray, sizeof(struct LCArray) + totalLength * sizeof(void*));
  newArray->length = totalLength;
  size_t copyPos = 0;
  for (LCInteger i=0; i<length; i++) {
    size_t copyLength = LCArrayLength(arrays[i]);
    memcpy(&(newArray->objects[copyPos]), LCArrayObjects(arrays[i]), copyLength * sizeof(void*));
    copyPos = copyPos+copyLength;
  }
  for (LCInteger i=0; i<totalLength; i++) {
    LCRetain(newArray->objects[i]);
  }
  return newArray;

}

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
  if (start >= array->length) {
    return LCArrayCreate(NULL, 0);
  }
  if (length == -1) {
    length = array->length-start;
  }
  return LCArrayCreate(&(array->objects[start]), length);
}

LCMutableArrayRef LCArrayCreateMutableArray(LCArrayRef array) {
  return LCMutableArrayCreate(array->objects, array->length);
}

LCArrayRef LCArrayCreateArrayWithMap(LCArrayRef array, void* info, LCCreateEachCb each) {
  void* newObjects[array->length];
  for (LCInteger i=0; i<array->length; i++) {
    newObjects[i] = each(i, info, array->objects[i]);
  }
  LCArrayRef newArray = LCArrayCreate(newObjects, array->length);
  for (LCInteger i=0; i<array->length; i++) {
    LCRelease(newObjects[i]);
  }
  return newArray;
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
