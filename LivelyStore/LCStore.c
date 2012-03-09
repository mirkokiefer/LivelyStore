
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

LCStoreRef LCStoreCreate(struct LCStoreBackend* backend, LCCommitRef head) {
  LCStoreRef newStore = LCNewObject(&typeStore, sizeof(struct LCStore));
  newStore->dataStore = LCDataStoreCreate(backend);
  if (head) {
    newStore->head = LCRetain(head);
  } else {
    newStore->head = LCCommitCreate(newStore->dataStore, NULL, NULL, 0);
  }
  return newStore;
};

void LCStoreCommit(LCStoreRef store, LCStageRef stage) {
  LCKeyValueRef* addPaths = LCStagePathsToAdd(stage);
  size_t addPathsLength = LCStageAddPathsCount(stage);
  LCStringRef* deletePaths = LCStagePathsToDelete(stage);
  size_t deletePathsLength = LCStageDeletePathsCount(stage);

  LCKeyValueRef keyValueSHAs[addPathsLength];
  storeDataWithSHAs(store, addPaths, addPathsLength, keyValueSHAs);
  
  LCTreeRef newTree = buildTree(store, LCCommitTree(store->head), keyValueSHAs, addPathsLength, deletePaths, deletePathsLength);
  
  LCCommitRef newHead = LCCommitCreate(store->dataStore, newTree, &(store->head), 1);
  setStoreHead(store, newHead);
}

LCCommitRef LCStoreHead(LCStoreRef store) {
  return store->head;
}

LCStringRef LCStoreDataSHA(LCStoreRef store, LCCommitRef commit, char* path) {
  if (!commit) {
    commit = store->head;
  }
  LCStringRef pathObj = LCStringCreate(path);
  LCArrayRef pathArray = createPathArray(pathObj);
  LCStringRef dataSHA = LCTreeChildDataAtPath(LCCommitTree(commit), pathArray);
  LCRelease(pathArray);
  LCRelease(pathObj);
  return dataSHA;
}

LCDataRef LCStoreData(LCStoreRef store, LCStringRef sha) {
  return LCDataStoreGetData(store->dataStore, sha);
}

void LCStoreDealloc(void* object) {
  LCStoreRef store = (LCStoreRef)object;
  LCRelease(store->dataStore);
  LCRelease(store->head);
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
  if (newHead != store->head) {
    LCRelease(store->head);
  }
  // retrieve SHA to force persisting all commits and trees
  LCCommitSHA(newHead);
  store->head = newHead;
}
