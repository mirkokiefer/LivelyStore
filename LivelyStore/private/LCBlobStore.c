
#include "LCBlobStore.h"

struct LCBlobStore {
  LCObjectInfo info;
  LCStringRef location;
};

void LCBlobStoreDealloc(void* object);

LCType typeBlobStore = {
  .dealloc = LCBlobStoreDealloc
};

LCBlobStoreRef LCBlobStoreCreate(LCStringRef location) {
  LCBlobStoreRef newStore = malloc(sizeof(struct LCBlobStore));
  if (newStore != NULL) {
    newStore->info.type = &typeBlobStore;
    LCRetain(location);
    newStore->location = location;
  }
  return newStore;
};

LCBlobRef LCBlobStoreGet(LCBlobStoreRef store, LCSHARef key) {
  return LCBlobCreate(NULL, 0);
}


void LCBlobStoreDealloc(void* object) {
  LCBlobStoreRef store = (LCBlobStoreRef)object;
  LCRelease(store->location);
}