

#ifndef LivelyStore_LCBlobArray_h
#define LivelyStore_LCBlobArray_h

#include "LivelyStore.h"

LCBlobArrayRef LCBlobArrayCreate(LCBlobRef* blobs, size_t length);
size_t LCBlobArrayLength(LCBlobArrayRef array);
LCBlobRef LCBlobBlobAtIndex(LCBlobArrayRef array, LCInteger index);

#endif
