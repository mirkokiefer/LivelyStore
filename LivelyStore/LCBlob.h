

#ifndef LivelyStore_LCBlob_h
#define LivelyStore_LCBlob_h

#include "LivelyStore.h"

LCBlobRef LCBlobCreate(void *data, size_t length);
void LCBlobRetain(LCBlobRef blob);
void LCBlobRelease(LCBlobRef blob);

size_t LCBlobSize(LCBlobRef blob);
void LCBlobData(LCBlobRef blob, void *buffer);

// buffer[20]
LCSHARef LCBlobSHA1(LCBlobRef blob);

#endif
