
#include "LCDataArray.h"

void LCDataArrayDealloc(void* object);
LCDataArrayRef LCDataArrayCopy(void* data);
size_t LCDataArrayDataCount(void* data);

struct LCDataArray {
  LCObjectInfo info;
  size_t length;
  LCDataRef datas[];
};

LCHashableObject hashableDataArray = {
  .dataArrayCopy = LCDataArrayCopy,
};

LCType typeDataArray = {
  .dealloc = LCDataArrayDealloc,
  .meta = &hashableDataArray
};

LCDataArrayRef LCDataArrayCreate(LCDataRef* datas, size_t length) {
  LCDataArrayRef newArray = malloc(sizeof(struct LCDataArray) + length*sizeof(LCDataRef));
  if (newArray != NULL) {
    newArray->info.type = &typeDataArray;
    
    for (LCInteger i=0; i<length; i++) {
      LCRetain(datas[i]);
    }
    newArray->length = length;
    memcpy(newArray->datas, datas, length*sizeof(LCDataRef));
  }
  return newArray;
};

size_t LCDataArrayLength(LCDataArrayRef array) {
  return array->length;
}

LCDataRef LCDataDataAtIndex(LCDataArrayRef data, LCInteger index) {
  if(index < data->length) {
    return data->datas[index];    
  } else {
    return NULL;
  }
}

LCDataRef* LCDataArrayDatas(LCDataArrayRef array) {
  return array->datas;
}

LCDataArrayRef LCDataArrayCopy(void* object) {
  LCRetain(object);
  return object;
}

size_t LCDataArrayDataCount(void* data) {
  return ((LCDataArrayRef)data)->length;
}

void LCDataArrayDealloc(void* object) {
  LCDataArrayRef array = (LCDataArrayRef)object;
  for (LCInteger i=0; i<array->length; i++) {
    LCRelease(array->datas[i]);
  }
}