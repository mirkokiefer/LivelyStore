
#include "LCStore.h"

struct LCStore {
  LCObjectInfo info;
  LCDataStoreRef dataStore;
  LCCommitRef head;
};

void LCStoreDealloc(void* object);
void storeDataWithSHAs(LCStoreRef store, LCPathDataRef addPaths[], size_t length, LCPathDataSHARef buffer[]);
LCTreeRef buildTree(LCTreeRef current, LCPathDataSHARef* add, size_t newLength, LCStringRef* delete, size_t deleteLength);
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

void LCStoreSetNewDataCallback(LCStoreRef store, void(*callback)(char* sha, unsigned char* data, size_t length)) {
  LCDataStoreSetNewDataCallback(store->dataStore, callback);
}

void LCStoreSetDeletedDataCallback(LCStoreRef store, void(*callback)(char* sha)) {
  LCDataStoreSetDeletedDataCallback(store->dataStore, callback);
}


void LCStoreCommit(LCStoreRef store, LCStageRef stage) {
  LCPathDataRef* addPaths = LCStagePathsToAdd(stage);
  size_t addPathsLength = LCStageAddPathsCount(stage);
  LCStringRef* deletePaths = LCStagePathsToDelete(stage);
  size_t deletePathsLength = LCStageDeletePathsCount(stage);

  LCPathDataSHARef keyValueSHAs[addPathsLength];
  storeDataWithSHAs(store, addPaths, addPathsLength, keyValueSHAs);
  
  LCTreeRef newTree = buildTree(LCCommitTree(store->head), keyValueSHAs, addPathsLength, deletePaths, deletePathsLength);
  
  LCCommitRef newHead = LCCommitCreate(store->head, newTree);
  setStoreHead(store, newHead);
}

void storeDataWithSHAs(LCStoreRef store, LCPathDataRef addPaths[], size_t length, LCPathDataSHARef buffer[]) {
  LCDataRef value;
  char* sha;
  for (LCInteger i=0; i<length; i++) {
    value = LCPathDataValue(addPaths[i]);
    sha = LCStringStringRef(LCDataSHA1(value));
    LCDataStorePutData(store->dataStore, sha, LCDataDataRef(value), LCDataLength(value));
    buffer[i] = LCPathDataCreatePathDataSHA(addPaths[i]);
  }
}

LCTreeRef buildTree(LCTreeRef current, LCPathDataSHARef* add, size_t newLength, LCStringRef* delete, size_t deleteLength) {
  /*
   Ich gehe gezielt runter zu den geänderten pfaden/trees - und erstelle dann rekursiv nach oben die bäume neu
   Ich sollte die keys mit einem Pfad objekt ersetzen...
   letzten endes sollte doch blob, tree oder commit immer dann einen newDataCallback aufrufen wenn der SHA berechnet wurde
   da alle diese Objekte den SHA cachen, wird das auch nur einmal vorkommen.
   wenn ich also den baum neu aufbau und am ende ganz oben SHA aufrufe, werden alle bäume automatisch gespeichert.
   voll geil!
   LCData wird gespeichtert sobald LCPathDataSHA erstellt wird.
   LCTree wird gespeichter sobald, LCCommit den SHA vom top-tree will.
   LCCommit wird gespeichert sobald LCStore den SHA will um den head neu zu setzen.
   
   */
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