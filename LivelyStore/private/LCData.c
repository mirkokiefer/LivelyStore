
#include "LCData.h"

void LCDataDealloc(void* object);

struct LCData {
  LCObjectInfo info;
  size_t length;
  LCStringRef sha;
  LCByte data[];
};

LCType typeData = {
  .dealloc = LCDataDealloc
};

LCDataRef LCDataCreate(LCByte data[], size_t length) {
  LCDataRef newData = LCNewObject(&typeData, sizeof(struct LCData) + length*sizeof(LCByte));
  newData->length = length;
  newData->sha = NULL;
  memcpy(newData->data, data, length*sizeof(LCByte));
  return newData;
};

size_t LCDataLength(LCDataRef data) {
  return data->length;
}

LCByte* LCDataDataRef(LCDataRef data) {
  return data->data;
}

LCStringRef LCDataSHA1(LCDataRef data) {
  if(data->sha == NULL) {
    data->sha = LCCreateSHAString(data->data, data->length); 
  }
  return data->sha;
}

void LCDataDealloc(void* object) {
  LCDataRef data = (LCDataRef)object;
  if(data->sha) {
    LCRelease(data->sha);    
  }
}