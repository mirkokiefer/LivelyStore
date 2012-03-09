
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
LCCommitRef findCommit(LCCommitRef start[], size_t length, LCStringRef sha);

LCType typeStore = {
  .dealloc = LCStoreDealloc
};

LCStoreRef LCStoreCreate(struct LCStoreBackend* backend, char headCommit[LC_SHA1_HEX_Length]) {
  LCStoreRef newStore = LCNewObject(&typeStore, sizeof(struct LCStore));
  newStore->dataStore = LCDataStoreCreate(backend);
  if (headCommit) {
    LCStringRef headCommitObj = LCStringCreate(headCommit);
    newStore->head = LCCommitCreateFromSHA(newStore->dataStore, headCommitObj);
    LCRelease(headCommitObj);
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

void LCStoreHead(LCStoreRef store, char* buffer) {
  strcpy(buffer, LCStringStringRef(LCCommitSHA(store->head)));
}

LCSuccess LCStoreDataSHA(LCStoreRef store, char* commit, char* path, char dataSHABuffer[LC_SHA1_HEX_Length]) {
  LCCommitRef commitObj;
  if (commit) {
    LCStringRef commitSHAObj = LCStringCreate(commit);
    commitObj = findCommit(&(store->head), 1, commitSHAObj); 
    LCRelease(commitSHAObj);
  } else {
    commitObj = store->head;
  }
  LCStringRef pathObj = LCStringCreate(path);
  LCArrayRef pathArray = createPathArray(pathObj);
  LCStringRef dataSHA = LCTreeChildDataAtPath(LCCommitTree(commitObj), pathArray);
  if (dataSHA) {
    strcpy(dataSHABuffer, LCStringStringRef(dataSHA));
  } else {
    return LCSuccessFalse;
  }
  LCRelease(pathObj);
  LCRelease(pathArray);
  return LCSuccessTrue;
}

size_t LCStoreDataLength(LCStoreRef store, char sha[LC_SHA1_HEX_Length]) {
  return LCDataStoreGetDataLength(store->dataStore, sha);
}

LCSuccess LCStoreData(LCStoreRef store, char sha[LC_SHA1_HEX_Length], unsigned char dataBuffer[]) {
  LCStringRef shaObj = LCStringCreate(sha);
  LCDataRef data = LCDataStoreGetData(store->dataStore, shaObj);
  if (data) {
    memcpy(dataBuffer, LCDataDataRef(data), LCDataLength(data)*sizeof(LCByte));    
  } else {
    return LCSuccessFalse;
  }
  return LCSuccessTrue;
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
  store->head = newHead;
}

LCCommitRef findCommit(LCCommitRef commits[], size_t length, LCStringRef sha) {
  if (length == 0) {
    return NULL;
  }
  for (LCInteger i=0; i<length; i++) {
    if(LCStringEqual(LCCommitSHA(commits[i]), sha)) {
      return commits[i];
    }
  }
  LCMutableArrayRef parents = LCMutableArrayCreate(NULL, 0);
  for (LCInteger i=0; i<length; i++) {
    LCMutableArrayAddObjects(parents, (void**)LCCommitParents(commits[i]), LCCommitParentsLength(commits[i]));
  }
  LCCommitRef result = findCommit((LCCommitRef*)LCMutableArrayObjects(parents), LCMutableArrayLength(parents), sha);
  LCRelease(parents);
  return result;
}