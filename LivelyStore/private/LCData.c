
#include "LCData.h"

void LCDataDealloc(void* object);
LCDataArrayRef LCDataDatasArrayCopy(void* data);
size_t LCDataDataCount(void* data);

struct LCData {
  LCObjectInfo info;
  size_t length;
  LCSHARef sha;
  LCByte data[];
};

LCHashableObject hashableData = {
  .dataArrayCopy = LCDataDatasArrayCopy,
};

LCType typeData = {
  .dealloc = LCDataDealloc,
  .meta = &hashableData
};

LCDataRef LCDataCreate(LCByte data[], size_t length) {
  LCDataRef newData = malloc(sizeof(struct LCData) + length*sizeof(LCByte));
  if (newData != NULL) {
    newData->info.type = &typeData;
    newData->length = length;
    memcpy(newData->data, data, length*sizeof(LCByte));
  }
  return newData;
};

size_t LCDataLength(LCDataRef data) {
  return data->length;
}

void LCDataData(LCDataRef data, LCByte buffer[]) {
  memcpy(buffer, data->data, data->length*sizeof(LCByte));
}

LCByte* LCDataDataRef(LCDataRef data) {
  return data->data;
}

LCDataArrayRef LCDataDatasArrayCopy(void* object) {
  LCDataRef data = (LCDataRef)object;
  return LCDataArrayCreate(&data, 1);
}

LCSHARef LCDataSHA1(LCDataRef data) {
  if(data->sha == NULL) {
    data->sha = LCSHACreateFromHashableObject(data);
  }
  return data->sha;
}

LCBool LCDataEqual(LCDataRef data, LCDataRef anotherData) {
  LCSHARef data1SHA = LCDataSHA1(data);
  LCSHARef data2SHA = LCDataSHA1(anotherData);
  return LCSHAEqual(data1SHA, data2SHA);
}

LCStringRef LCBLobCreateString(LCDataRef data) {
  return LCStringCreate((char*)(data->data));
}

void LCDataDealloc(void* object) {
  LCDataRef data = (LCDataRef)object;
  if(data->sha) {
    LCRelease(data->sha);    
  }
}