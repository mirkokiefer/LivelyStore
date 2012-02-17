
#include "LCDataStore.h"

struct LCDataStore {
  LCObjectInfo info;
  LCStringRef location;
  void (*newDataCallback)(char* sha, unsigned char* data, size_t length);
  void(*deletedDataCallback)(char* sha);
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

void LCDataStoreSetNewDataCallback(LCDataStoreRef store, void(*callback)(char* sha, unsigned char* data, size_t length)) {
  store->newDataCallback = callback;
}

void LCDataStoreSetDeletedDataCallback(LCDataStoreRef store, void(*callback)(char* sha)) {
  store->deletedDataCallback = callback;
}

void LCDataStorePutData(LCDataStoreRef store, char* sha, unsigned char* data, size_t length) {
  store->newDataCallback(sha, data, length);
}

void LCDataStoreDeleteData(LCDataStoreRef store, char* sha) {
  store->deletedDataCallback(sha);
}

void LCDataStoreDealloc(void* object) {
  LCDataStoreRef store = (LCDataStoreRef)object;
  LCRelease(store->location);
}