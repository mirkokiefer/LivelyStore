
#include "LCStore.h"

struct LCStore {
  LCObjectInfo info;
  LCDataStoreRef dataStore;
  LCCommitRef head;
  void (*newDataCallback)(char* sha, unsigned char* data, size_t length);
  void(*deletedDataCallback)(char* sha);
};

void LCStoreDealloc(void* object);
void invokeNewDataCallback(LCStoreRef store, LCSHARef sha, LCDataRef data);
void createSHAsAndInvokeCallback(LCStoreRef store, LCKeyValueRef addKeys[], size_t length, LCKeyValueSHARef buffer[]);
LCTreeRef buildTree(LCTreeRef current, LCKeyValueSHARef* add, size_t newLength, LCStringRef* delete, size_t deleteLength);
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
    LCRelease(lcLocation);
  }
  return newStore;
};

void LCStoreCommit(LCStoreRef store, LCStageRef stage) {
  LCKeyValueRef* addKeys = LCStageKeysToAdd(stage);
  size_t addKeysLength = LCStageAddKeysCount(stage);
  LCStringRef* deleteKeys = LCStageKeysToDelete(stage);
  size_t deleteKeysLength = LCStageDeleteKeysCount(stage);

  LCKeyValueSHARef keyValueSHAs[addKeysLength];
  createSHAsAndInvokeCallback(store, addKeys, addKeysLength, keyValueSHAs);
  
  LCTreeRef newTree = buildTree(LCCommitTree(store->head), keyValueSHAs, addKeysLength, deleteKeys, deleteKeysLength);
  
  LCCommitRef newHead = LCCommitCreate(store->head, newTree);
  setStoreHead(store, newHead);
}

void invokeNewDataCallback(LCStoreRef store, LCSHARef sha, LCDataRef data) {
  LCStringRef shaString = LCSHACreateHexString(sha);
  store->newDataCallback(LCStringStringRef(shaString), LCDataDataRef(data), LCDataLength(data));
  LCRelease(shaString);
}

void createSHAsAndInvokeCallback(LCStoreRef store, LCKeyValueRef addKeys[], size_t length, LCKeyValueSHARef buffer[]) {
  LCDataRef value;
  for (LCInteger i=0; i<length; i++) {
    value = LCKeyValueValue(addKeys[i]);
    invokeNewDataCallback(store, LCDataSHA1(value), value);
    buffer[i] = LCKeyValueCreateKeyValueSHA(addKeys[i]);
  }
}

LCTreeRef buildTree(LCTreeRef current, LCKeyValueSHARef* add, size_t newLength, LCStringRef* delete, size_t deleteLength) {
  
}

void setStoreHead(LCStoreRef store, LCCommitRef newHead) {
  free(store->head);
  store->head = newHead;
}

void LCStoreDealloc(void* object) {
  LCStoreRef store = (LCStoreRef)object;
  LCRelease(store->dataStore);
  LCRelease(store->head);
}