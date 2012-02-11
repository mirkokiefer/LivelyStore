
#ifndef LivelyStore_LCSHA_h
#define LivelyStore_LCSHA_h

#include "LivelyStore.h"

LCSHARef LCSHACreateFromHashableObject(void* object);
LCSHARef LCSHACreateFromHexString(LCStringRef hexString, LCDataStoreRef dataStore);
void* LCSHAObject(LCSHARef sha);
LCStringRef LCSHACreateHexString(LCSHARef sha);
LCBool LCSHAEqual(LCSHARef sha, LCSHARef anotherSHA);
LCDataRef LCSHASHAData(LCSHARef sha);

#endif
