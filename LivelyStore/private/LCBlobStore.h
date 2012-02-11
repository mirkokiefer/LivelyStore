
#ifndef LivelyStore_LCBlobStore_h
#define LivelyStore_LCBlobStore_h

#include "LivelyStore.h"

LCBlobStoreRef LCBlobStoreCreate(LCStringRef location);
LCSHARef LCBlobStorePut(LCBlobStoreRef store, LCBlobRef blob);
LCBlobRef LCBlobStoreGet(LCBlobStoreRef store, LCSHARef key);
void LCBlobStoreDelete(LCBlobStoreRef store, LCSHARef key);

#endif
