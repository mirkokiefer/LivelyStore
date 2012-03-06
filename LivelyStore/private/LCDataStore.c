
#include "LCDataStore.h"

void putData(LCDataStoreRef store, LCDataType type, LCStringRef key, LCByte data[], size_t length);
void deleteData(LCDataStoreRef store, LCDataType type, LCStringRef key);
LCDataRef getData(LCDataStoreRef store, LCDataType type, LCStringRef key);

struct LCDataStore {
  LCObjectInfo info;
  LCStringRef location;
  void* storeObject;
  LCStoreDataCb storeCb;
  LCDeleteDataCb deleteCb;
  LCGetDataCb getCb;
  LCGetDataLengthCb getLengthCb;
};

void LCDataStoreDealloc(void* object);

LCType typeDataStore = {
  .dealloc = LCDataStoreDealloc
};

LCDataStoreRef LCDataStoreCreate(LCStringRef location) {
  LCDataStoreRef newStore = malloc(sizeof(struct LCDataStore));
  if (newStore != NULL) {
    newStore->info.type = &typeDataStore;
    LCRetain(location);
    newStore->location = location;
  }
  return newStore;
};

void LCDataStoreSetStoreObject(LCDataStoreRef store, void* storeObject) {
  store->storeObject = LCRetain(storeObject);
}

void LCDataStoreSetNewDataCallback(LCDataStoreRef store, LCStoreDataCb callback) {
  store->storeCb = callback;
}

void LCDataStoreSetDeletedDataCallback(LCDataStoreRef store, LCDeleteDataCb callback) {
  store->deleteCb = callback;
}

void LCDataStoreSetGetDataCallback(LCDataStoreRef store, LCGetDataCb callback) {
  store->getCb = callback;
}

void LCDataStoreSetGetDataLengthCallback(LCDataStoreRef store, LCGetDataLengthCb callback) {
  store->getLengthCb = callback;
}

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

LCStringRef LCDataStoreGetTreeData(LCDataStoreRef store, LCStringRef sha) {
  LCDataRef data = getData(store, LCTree, sha);
  LCStringRef stringData = LCStringCreate((char*)LCDataDataRef(data));
  LCRelease(data);
  return stringData;
}

void LCDataStoreDealloc(void* object) {
  LCDataStoreRef store = (LCDataStoreRef)object;
  LCRelease(store->location);
}

void putData(LCDataStoreRef store, LCDataType type, LCStringRef key, LCByte data[], size_t length) {
  store->storeCb(store->storeObject, type, LCStringStringRef(key), (unsigned char*)data, length);
}

void deleteData(LCDataStoreRef store, LCDataType type, LCStringRef key) {
  store->deleteCb(store->storeObject, type, LCStringStringRef(key));
}

LCDataRef getData(LCDataStoreRef store, LCDataType type, LCStringRef key) {
  size_t length = store->getLengthCb(store->storeObject, type, LCStringStringRef(key));
  LCByte buffer[length];
  store->getCb(store->storeObject, type, LCStringStringRef(key), buffer);
  return LCDataCreate(buffer, length);
}