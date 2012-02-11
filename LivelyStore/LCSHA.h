
#ifndef LivelyStore_LCSHA_h
#define LivelyStore_LCSHA_h

#include "LivelyStore.h"

LCSHARef LCSHACreate(LCBlobRef blobs[], size_t count);
LCSHARef LCSHACreateFromHexString(LCStringRef hexString);
LCBlobRef LCSHABlob(LCSHARef sha);
LCStringRef LCSHACreateHexString(LCSHARef sha);
LCBool LCSHAEqual(LCSHARef sha, LCSHARef anotherSHA);

//private
void* LCSHAStoreLookup(LCSHARef sha, LCBlobStoreRef blobStore);
#endif
