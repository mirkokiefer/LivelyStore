
#include "LCBackendWrapper.h"

void putData(LCBackendWrapperRef store, LCDataType type, LCStringRef key, LCByte data[], size_t length);
void deleteData(LCBackendWrapperRef store, LCDataType type, LCStringRef key);
LCDataRef getData(LCBackendWrapperRef store, LCDataType type, LCStringRef key);

struct LCBackendWrapper {
  LCObjectInfo info;
  LCStringRef location;
  struct LCStoreBackend* backend;
};

void LCBackendWrapperDealloc(void* object);

LCType typeBackendWrapper = {
  .dealloc = LCBackendWrapperDealloc
};

LCBackendWrapperRef LCBackendWrapperCreate(struct LCStoreBackend* backend) {
  LCBackendWrapperRef newStore = LCNewObject(&typeBackendWrapper, sizeof(struct LCBackendWrapper));
  newStore->backend = backend;
  return newStore;
};

void LCBackendWrapperPutData(LCBackendWrapperRef store, LCStringRef sha, LCDataRef data) {
  putData(store, LCData, sha, LCDataDataRef(data), LCDataLength(data));
}

void LCBackendWrapperDeleteData(LCBackendWrapperRef store, LCStringRef sha) {
  deleteData(store, LCData, sha);
}

void LCBackendWrapperPutTreeData(LCBackendWrapperRef store, LCStringRef sha, LCStringRef data) {
  putData(store, LCTree, sha, (LCByte*)LCStringStringRef(data), LCStringLength(data)+1);
}

void LCBackendWrapperPutCommitData(LCBackendWrapperRef store, LCStringRef sha, LCStringRef data) {
  putData(store, LCCommit, sha, (LCByte*)LCStringStringRef(data), LCStringLength(data)+1);  
}

void LCBackendWrapperPutTagData(LCBackendWrapperRef store, LCStringRef tag, LCStringRef data) {
  //TODO
}

size_t LCBackendWrapperGetDataLength(LCBackendWrapperRef store, char* sha) {
  return store->backend->getLengthCb(store->backend->storeObject, LCData, sha);
}

LCDataRef LCBackendWrapperGetData(LCBackendWrapperRef store, LCStringRef sha) {
  return getData(store, LCData, sha);
}

LCStringRef LCBackendWrapperGetTreeData(LCBackendWrapperRef store, LCStringRef sha) {
  LCDataRef data = getData(store, LCTree, sha);
  LCStringRef stringData = LCStringCreate((char*)LCDataDataRef(data));
  LCRelease(data);
  return stringData;
}

LCStringRef LCBackendWrapperGetCommitData(LCBackendWrapperRef store, LCStringRef sha) {
  LCDataRef data = getData(store, LCCommit, sha);
  if (data == NULL) {
    return NULL;
  }
  LCStringRef stringData = LCStringCreate((char*)LCDataDataRef(data));
  LCRelease(data);
  return stringData;
}

void LCBackendWrapperDealloc(void* object) {
  LCBackendWrapperRef store = (LCBackendWrapperRef)object;
  LCRelease(store->location);
}

void putData(LCBackendWrapperRef store, LCDataType type, LCStringRef key, LCByte data[], size_t length) {
  LCStoreDataCb cb = store->backend->storeCb;
  cb(store->backend->storeObject, type, LCStringStringRef(key), (unsigned char*)data, length);
}

void deleteData(LCBackendWrapperRef store, LCDataType type, LCStringRef key) {
  LCDeleteDataCb cb = store->backend->deleteCb;
  cb(store->backend->storeObject, type, LCStringStringRef(key));
}

LCDataRef getData(LCBackendWrapperRef store, LCDataType type, LCStringRef key) {
  LCGetDataCb getCb = store->backend->getCb;
  LCGetDataLengthCb lengthCb = store->backend->getLengthCb;
  size_t length = lengthCb(store->backend->storeObject, type, LCStringStringRef(key));
  if (length == -1) {
    return NULL;
  }
  LCByte buffer[length];
  getCb(store->backend->storeObject, type, LCStringStringRef(key), buffer);
  return LCDataCreate(buffer, length);
}