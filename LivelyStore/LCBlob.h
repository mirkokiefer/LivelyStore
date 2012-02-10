

#ifndef LivelyStore_LCBlob_h
#define LivelyStore_LCBlob_h

#include "LivelyStore.h"

LCBlobRef LCBlobCreate(LCByte data[], size_t length);
void LCBlobRetain(LCBlobRef blob);
void LCBlobRelease(LCBlobRef blob);

size_t LCBlobLength(LCBlobRef blob);
void LCBlobData(LCBlobRef blob, LCByte buffer[]);
LCByte* LCBlobDataRef(LCBlobRef blob);

// buffer[20]
LCSHARef LCBlobSHA1(LCBlobRef blob);

#endif