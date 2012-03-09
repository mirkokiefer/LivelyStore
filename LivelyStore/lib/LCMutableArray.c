
#include "LCMutableArray.h"

void LCMutableArrayDealloc(void* object);
LCBool resizeBuffer(LCMutableArrayRef array, size_t size);

struct LCMutableArray {
  LCObjectInfo info;
  size_t length;
  size_t bufferLength;
  void** objects;
};

LCType typeMutableArray = {
  .dealloc = LCMutableArrayDealloc
};

LCMutableArrayRef LCMutableArrayCreate(void* objects[], size_t length) {
  LCMutableArrayRef newArray = LCNewObject(&typeMutableArray, sizeof(struct LCMutableArray));
  newArray->objects = NULL;
  if(length > 0) {
    for(LCInteger i=0; i<length; i++) {
      LCRetain(objects[i]);
    }
    resizeBuffer(newArray, length);    
    memcpy(newArray->objects, objects, length * sizeof(void*));  
  } else {
    resizeBuffer(newArray, 10);
  }
  newArray->length = length;
  return newArray;
};

void** LCMutableArrayObjects(LCMutableArrayRef array) {
  return array->objects;
}

void* LCMutableArrayObjectAtIndex(LCMutableArrayRef array, LCInteger index) {
  return array->objects[index];
}

size_t LCMutableArrayLength(LCMutableArrayRef array) {
  return array->length;
}

LCArrayRef LCMutableArrayCreateSubArray(LCMutableArrayRef array, LCInteger start, size_t length) {
  if (length == -1) {
    length = array->length-start;
  }
  return LCArrayCreate(&(array->objects[start]), length);
}

LCArrayRef LCMutableArrayCreateArray(LCMutableArrayRef array) {
  return LCArrayCreate(array->objects, array->length);
}

LCMutableArrayRef LCMutableArrayCopy(LCMutableArrayRef array) {
  return LCMutableArrayCreate(array->objects, array->length);
}

void LCMutableArrayAddObject(LCMutableArrayRef array, void* object) {
  if(array->length+1 > array->bufferLength) {
    resizeBuffer(array, array->bufferLength*2);
  }
  LCRetain(object);
  array->objects[array->length] = object;
  array->length = array->length + 1;
}

void LCMutableArrayAddObjects(LCMutableArrayRef array, void* objects[], size_t length) {
  for (LCInteger i=0; i<length; i++) {
    LCMutableArrayAddObject(array, objects[i]);
  }
}

void LCMutableArrayRemoveIndex(LCMutableArrayRef array, LCInteger index) {
  LCRelease(array->objects[index]);
  if (index < (array->length-1)) {
    size_t objectsToCopy = array->length - (index+1);
    memmove(&(array->objects[index]), &(array->objects[index+1]), objectsToCopy*sizeof(void*));
  }
  array->length = array->length-1;
}

void LCMutableArrayRemoveObject(LCMutableArrayRef array, void* object) {
  for (LCInteger i=0; i<array->length; i++) {
    if(array->objects[i] == object) {
      return LCMutableArrayRemoveIndex(array, i);
    }
  }
}

void LCMutableArraySort(LCMutableArrayRef array) {
  LCSortObjects(array->objects, array->length);
}

void LCMutableArrayDealloc(void* object) {
  LCMutableArrayRef array = (LCMutableArrayRef)object;
  for (LCInteger i=0; i<array->length; i++) {
    LCRelease(array->objects[i]);
  }
  LCFree(array->objects);
}

LCBool resizeBuffer(LCMutableArrayRef array, size_t length) {
  void* buffer = realloc(array->objects, sizeof(void*) * length);
  if(buffer) {
    array->objects = buffer;
    array->bufferLength = length;
    return true;
  } else {
    return false;
  }
}
