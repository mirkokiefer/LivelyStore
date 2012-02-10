
#ifndef LivelyStore_LCSHA_h
#define LivelyStore_LCSHA_h

#include "LivelyStore.h"

LCSHARef LCSHACreate(LCBlobRef blobs[], size_t count);
void LCSHARetain(LCSHARef sha);
void LCSHARelease(LCSHARef sha);
LCStringRef LCSHAHexString(LCSHARef sha);

#endif
