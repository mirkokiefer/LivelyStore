
#ifndef LivelyStore_LCDataStore_h
#define LivelyStore_LCDataStore_h

#include "LivelyStore.h"

LCDataStoreRef LCDataStoreCreate(LCStringRef location);
void LCDataStoreSetNewDataCallback(LCDataStoreRef store, LCStoreDataCb callback);
void LCDataStoreSetDeletedDataCallback(LCDataStoreRef store, LCDeleteDataCb callback);

void LCDataStorePutData(LCDataStoreRef store, char* sha, unsigned char* data, size_t length);
void LCDataStoreDeleteData(LCDataStoreRef store, char* sha);
void LCDataStorePutTreeData(LCDataStoreRef store, char* sha, char* data);
void LCDataStorePutCommitData(LCDataStoreRef store, char* sha, char* data);
void LCDataStorePutTagData(LCDataStoreRef store, char* tag, char* data);

void LCDataStoreDelete(LCDataStoreRef store, LCStringRef sha);

#endif
