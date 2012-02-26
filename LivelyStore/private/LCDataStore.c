
#include "LCDataStore.h"

struct LCDataStore {
  LCObjectInfo info;
  LCStringRef location;
  LCStoreDataCb storeCb;
  LCDeleteDataCb deleteCb;
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

void LCDataStoreSetNewDataCallback(LCDataStoreRef store, LCStoreDataCb callback) {
  store->storeCb = callback;
}

void LCDataStoreSetDeletedDataCallback(LCDataStoreRef store, LCDeleteDataCb callback) {
  store->deleteCb = callback;
}

void LCDataStorePutData(LCDataStoreRef store, char* sha, unsigned char* data, size_t length) {
  store->storeCb(LCData, sha, data, length);
}

void LCDataStoreDeleteData(LCDataStoreRef store, char* sha) {
  store->deleteCb(LCData, sha);
}

void LCDataStoreDealloc(void* object) {
  LCDataStoreRef store = (LCDataStoreRef)object;
  LCRelease(store->location);
}