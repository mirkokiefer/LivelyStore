
#include "LCDataStore.h"

struct LCDataStore {
  LCObjectInfo info;
  LCStringRef location;
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

LCDataRef LCDataStoreGet(LCDataStoreRef store, LCSHARef key) {
  return LCDataCreate(NULL, 0);
}


void LCDataStoreDealloc(void* object) {
  LCDataStoreRef store = (LCDataStoreRef)object;
  LCRelease(store->location);
}