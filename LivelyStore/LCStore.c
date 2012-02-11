
#include "LCStore.h"

struct LCStore {
  LCObjectInfo info;
  LCBlobStoreRef blobStore;
  LCCommitRef head;
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

void LCStoreCommit(LCStoreRef store, LCStageRef stage) {
  store->head = LCStageCreateCommit(stage, store->head);
}

void LCStoreDealloc(void* object) {
  LCStoreRef store = (LCStoreRef)object;
  LCRelease(store->blobStore);
}