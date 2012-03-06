
#include "LCStore.h"

struct LCStore {
  LCObjectInfo info;
  LCDataStoreRef dataStore;
  LCCommitRef head;
};

void LCStoreDealloc(void* object);
void storeDataWithSHAs(LCStoreRef store, LCKeyValueRef addPaths[], size_t length, LCKeyValueRef pathSHABuffer[]);
LCTreeRef buildTree(LCStoreRef store, LCTreeRef current, LCKeyValueRef* addPathSHAs, size_t newLength,
                    LCStringRef* delete, size_t deleteLength);
void setStoreHead(LCStoreRef store, LCCommitRef newHead);

LCType typeStore = {
  .dealloc = LCStoreDealloc
};

LCStoreRef LCStoreCreate(char* location) {
  LCStoreRef newStore = malloc(sizeof(struct LCStore));
  if (newStore != NULL) {
    newStore->info.type = &typeStore;
    LCStringRef lcLocation = LCStringCreate(location);
    newStore->dataStore = LCDataStoreCreate(lcLocation);
    newStore->head = LCCommitCreate(NULL, NULL);
    LCRelease(lcLocation);
  }
  return newStore;
};

void LCStoreSetStoreCallback(LCStoreRef store, LCStoreDataCb callback) {
  LCDataStoreSetNewDataCallback(store->dataStore, callback);
}

void LCStoreSetDeleteCallback(LCStoreRef store, LCDeleteDataCb callback) {
  LCDataStoreSetDeletedDataCallback(store->dataStore, callback);
}

void LCStoreSetGetDataCallback(LCStoreRef store, LCGetDataCb callback) {
  LCDataStoreSetGetDataCallback(store->dataStore, callback);
}

void LCStoreCommit(LCStoreRef store, LCStageRef stage) {
  LCKeyValueRef* addPaths = LCStagePathsToAdd(stage);
  size_t addPathsLength = LCStageAddPathsCount(stage);
  LCStringRef* deletePaths = LCStagePathsToDelete(stage);
  size_t deletePathsLength = LCStageDeletePathsCount(stage);

  LCKeyValueRef keyValueSHAs[addPathsLength];
  storeDataWithSHAs(store, addPaths, addPathsLength, keyValueSHAs);
  
  LCTreeRef newTree = buildTree(store, LCCommitTree(store->head), keyValueSHAs, addPathsLength, deletePaths, deletePathsLength);
  
  LCCommitRef newHead = LCCommitCreate(store->head, newTree);
  setStoreHead(store, newHead);
}

void storeDataWithSHAs(LCStoreRef store, LCKeyValueRef addPaths[], size_t length, LCKeyValueRef pathSHABuffer[]) {
  LCDataRef value;
  LCStringRef sha;
  for (LCInteger i=0; i<length; i++) {
    value = LCKeyValueValue(addPaths[i]);
    sha = LCDataSHA1(value);
    LCDataStorePutData(store->dataStore, sha, value);
    pathSHABuffer[i] = LCKeyValueCreate(LCKeyValueKey(addPaths[i]), sha);
  }
}

LCTreeRef buildTree(LCStoreRef store, LCTreeRef current, LCKeyValueRef addPathSHAs[], size_t addPathSHAsLength,
                    LCStringRef* delete, size_t deleteLength) {
  LCMutableArrayRef updates = LCMutableArrayCreate((void**)addPathSHAs, addPathSHAsLength);
  LCKeyValueRef deleteUpdate;
  for (LCInteger i=0; i<deleteLength; i++) {
    deleteUpdate = LCKeyValueCreate(delete[i], NULL);
    LCMutableArrayAddObject(updates, deleteUpdate);
    LCRelease(deleteUpdate);
  }
  LCTreeRef newTree = LCTreeCreateTreeUpdatingData(current, store->dataStore, updates);
  LCRelease(updates);
  return newTree;
}

void setStoreHead(LCStoreRef store, LCCommitRef newHead) {
  LCRelease(store->head);
  store->head = newHead;
}

void LCStoreDealloc(void* object) {
  LCStoreRef store = (LCStoreRef)object;
  LCRelease(store->dataStore);
  LCRelease(store->head);
}