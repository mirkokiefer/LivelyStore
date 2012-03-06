
#include "LCDataStore.h"

void putData(LCDataStoreRef store, LCDataType type, LCStringRef key, LCByte data[], size_t length);
void deleteData(LCDataStoreRef store, LCDataType type, LCStringRef key);
LCDataRef getData(LCDataStoreRef store, LCDataType type, LCStringRef key);

struct LCDataStore {
  LCObjectInfo info;
  LCStringRef location;
  struct LCStoreBackend* backend;
};

void LCDataStoreDealloc(void* object);

LCType typeDataStore = {
  .dealloc = LCDataStoreDealloc
};

LCDataStoreRef LCDataStoreCreate(struct LCStoreBackend* backend) {
  LCDataStoreRef newStore = LCNewObject(&typeDataStore, sizeof(struct LCDataStore));
  newStore->backend = backend;
  return newStore;
};

void LCDataStorePutData(LCDataStoreRef store, LCStringRef sha, LCDataRef data) {
  putData(store, LCData, sha, LCDataDataRef(data), LCDataLength(data));
}

void LCDataStoreDeleteData(LCDataStoreRef store, LCStringRef sha) {
  deleteData(store, LCData, sha);
}

void LCDataStorePutTreeData(LCDataStoreRef store, LCStringRef sha, LCStringRef data) {
  putData(store, LCTree, sha, (LCByte*)LCStringStringRef(data), LCStringLength(data)+1);
}

void LCDataStorePutCommitData(LCDataStoreRef store, LCStringRef sha, LCStringRef data) {
  putData(store, LCCommit, sha, (LCByte*)LCStringStringRef(data), LCStringLength(data)+1);  
}

void LCDataStorePutTagData(LCDataStoreRef store, LCStringRef tag, LCStringRef data) {
  //TODO
}

size_t LCDataStoreGetDataLength(LCDataStoreRef store, char* sha) {
  return store->backend->getLengthCb(store->backend->storeObject, LCData, sha);
}

LCDataRef LCDataStoreGetData(LCDataStoreRef store, LCStringRef sha) {
  return getData(store, LCData, sha);
}

LCStringRef LCDataStoreGetTreeData(LCDataStoreRef store, LCStringRef sha) {
  LCDataRef data = getData(store, LCTree, sha);
  LCStringRef stringData = LCStringCreate((char*)LCDataDataRef(data));
  LCRelease(data);
  return stringData;
}

LCStringRef LCDataStoreGetCommitData(LCDataStoreRef store, LCStringRef sha) {
  LCDataRef data = getData(store, LCCommit, sha);
  if (data == NULL) {
    return NULL;
  }
  LCStringRef stringData = LCStringCreate((char*)LCDataDataRef(data));
  LCRelease(data);
  return stringData;
}

void LCDataStoreDealloc(void* object) {
  LCDataStoreRef store = (LCDataStoreRef)object;
  LCRelease(store->location);
}

void putData(LCDataStoreRef store, LCDataType type, LCStringRef key, LCByte data[], size_t length) {
  LCStoreDataCb cb = store->backend->storeCb;
  cb(store->backend->storeObject, type, LCStringStringRef(key), (unsigned char*)data, length);
}

void deleteData(LCDataStoreRef store, LCDataType type, LCStringRef key) {
  LCDeleteDataCb cb = store->backend->deleteCb;
  cb(store->backend->storeObject, type, LCStringStringRef(key));
}

LCDataRef getData(LCDataStoreRef store, LCDataType type, LCStringRef key) {
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