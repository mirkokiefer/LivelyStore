
#ifndef LivelyStore_LCSHA_h
#define LivelyStore_LCSHA_h

#include "LivelyStore.h"

LCSHARef LCSHACreateFromHashableObject(void* object);
LCSHARef LCSHACreateFromHexString(LCStringRef hexString, LCBlobStoreRef blobStore);
void* LCSHAObject(LCSHARef sha);
LCStringRef LCSHACreateHexString(LCSHARef sha);
LCBool LCSHAEqual(LCSHARef sha, LCSHARef anotherSHA);
LCBlobRef LCSHASHABlob(LCSHARef sha);

#endif
