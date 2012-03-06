
#ifndef LivelyStore_LCDataStore_h
#define LivelyStore_LCDataStore_h

#include "LivelyStore.h"

LCDataStoreRef LCDataStoreCreate(struct LCStoreBackend* backend);
void LCDataStorePutData(LCDataStoreRef store, LCStringRef sha, LCDataRef data);
void LCDataStoreDeleteData(LCDataStoreRef store, LCStringRef sha);
void LCDataStorePutTreeData(LCDataStoreRef store, LCStringRef sha, LCStringRef data);
void LCDataStorePutCommitData(LCDataStoreRef store, LCStringRef sha, LCStringRef data);
void LCDataStorePutTagData(LCDataStoreRef store, LCStringRef tag, LCStringRef data);
size_t LCDataStoreGetDataLength(LCDataStoreRef store, char* sha);
LCDataRef LCDataStoreGetData(LCDataStoreRef store, LCStringRef sha);
LCStringRef LCDataStoreGetTreeData(LCDataStoreRef store, LCStringRef sha);
LCStringRef LCDataStoreGetCommitData(LCDataStoreRef store, LCStringRef sha);

#endif
