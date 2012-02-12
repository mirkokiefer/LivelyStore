
#include "LCStore.h"

struct LCStore {
  LCObjectInfo info;
  LCDataStoreRef dataStore;
  LCCommitRef head;
  void (*newDataCallback)(char* sha, unsigned char* data, size_t length);
  void(*deletedDataCallback)(char* sha);
};

void LCStoreDealloc(void* object);

LCType typeStore = {
  .dealloc = LCStoreDealloc
};

LCStoreRef LCStoreCreate(char* location) {
  LCStoreRef newStore = malloc(sizeof(struct LCStore));
  if (newStore != NULL) {
    newStore->info.type = &typeStore;
    LCStringRef lcLocation = LCStringCreate(location);
    newStore->dataStore = LCDataStoreCreate(lcLocation);
    LCRelease(lcLocation);
  }
  return newStore;
};

void LCStoreSetHead(LCStoreRef store, LCCommitRef commit) {
   LCCommitTree(commit);
}

void LCStoreDealloc(void* object) {
  LCStoreRef store = (LCStoreRef)object;
  LCRelease(store->dataStore);
  LCRelease(store->head);
}