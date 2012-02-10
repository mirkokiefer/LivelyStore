
#ifndef LivelyStore_LCSHA_h
#define LivelyStore_LCSHA_h

#include "LivelyStore.h"

LCSHARef LCSHACreate(LCBlobRef blobs[], size_t count);
LCStringRef LCSHAHexString(LCSHARef sha);

#endif
