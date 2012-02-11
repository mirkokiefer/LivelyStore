
#ifndef LivelyStore_LCDataStore_h
#define LivelyStore_LCDataStore_h

#include "LivelyStore.h"

LCDataStoreRef LCDataStoreCreate(LCStringRef location);
LCSHARef LCDataStorePut(LCDataStoreRef store, LCDataRef data);
LCDataRef LCDataStoreGet(LCDataStoreRef store, LCSHARef key);
void LCDataStoreDelete(LCDataStoreRef store, LCSHARef key);

#endif
