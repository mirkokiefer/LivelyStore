
#include "LCRepository.h"

struct LCRepository {
  LCObjectInfo info;
  LCBackendWrapperRef dataStore;
  LCCommitRef head;
};

void LCRepositoryDealloc(void* object);
void storeDataWithSHAs(LCRepositoryRef store, LCKeyValueRef addPaths[], size_t length, LCKeyValueRef pathSHABuffer[]);
LCTreeRef buildTree(LCRepositoryRef store, LCTreeRef current, LCKeyValueRef* addPathSHAs, size_t newLength,
                    LCStringRef* delete, size_t deleteLength);
void setStoreHead(LCRepositoryRef store, LCCommitRef newHead);

LCType typeStore = {
  .dealloc = LCRepositoryDealloc
};

LCRepositoryRef LCRepositoryCreate(struct LCRepositoryBackend* backend, LCStringRef headSHA) {
  LCRepositoryRef newStore = LCNewObject(&typeStore, sizeof(struct LCRepository));
  newStore->dataStore = LCBackendWrapperCreate(backend);
  if (headSHA) {
    newStore->head = LCCommitCreateFromSHA(newStore->dataStore, headSHA);
  } else {
    newStore->head = LCCommitCreate(newStore->dataStore, NULL, NULL, 0);
  }
  return newStore;
};

void LCRepositoryCommit(LCRepositoryRef store, LCStageRef stage) {
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

LCCommitRef LCRepositoryHead(LCRepositoryRef store) {
  return store->head;
}

LCStringRef LCRepositoryDataSHA(LCRepositoryRef store, LCCommitRef commit, char* path) {
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

LCDataRef LCRepositoryData(LCRepositoryRef store, LCStringRef sha) {
  return LCBackendWrapperGetData(store->dataStore, sha);
}

void LCRepositoryDealloc(void* object) {
  LCRepositoryRef store = (LCRepositoryRef)object;
  LCRelease(store->dataStore);
  LCRelease(store->head);
}

void storeDataWithSHAs(LCRepositoryRef store, LCKeyValueRef addPaths[], size_t length, LCKeyValueRef pathSHABuffer[]) {
  LCDataRef value;
  LCStringRef sha;
  for (LCInteger i=0; i<length; i++) {
    value = LCKeyValueValue(addPaths[i]);
    sha = LCDataSHA1(value);
    LCBackendWrapperPutData(store->dataStore, sha, value);
    pathSHABuffer[i] = LCKeyValueCreate(LCKeyValueKey(addPaths[i]), sha);
  }
}

LCTreeRef buildTree(LCRepositoryRef store, LCTreeRef current, LCKeyValueRef addPathSHAs[], size_t addPathSHAsLength,
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

void setStoreHead(LCRepositoryRef store, LCCommitRef newHead) {
  if (newHead != store->head) {
    LCRelease(store->head);
  }
  // retrieve SHA to force persisting all commits and trees
  LCCommitSHA(newHead);
  store->head = newHead;
}
