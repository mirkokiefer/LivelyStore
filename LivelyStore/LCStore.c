
#include "LCStore.h"

struct LCStore {
  LCObjectInfo info;
  LCBlobStoreRef blobStore;
};

void LCStoreDealloc(void* object);

LCType typeStore = {
  .dealloc = LCStoreDealloc
};

LCStoreRef LCStoreCreate(LCStringRef location) {
  LCStoreRef newStore = malloc(sizeof(struct LCStore));
  if (newStore != NULL) {
    newStore->info.type = &typeStore;
    newStore->blobStore = LCBlobStoreCreate(location);
  }
  return newStore;
};

void LCStoreAddCommit(LCStoreRef store, LCCommitStageRef commit) {
  
}

void LCStoreDealloc(void* object) {
  LCStoreRef store = (LCStoreRef)object;
  LCRelease(store->blobStore);
}