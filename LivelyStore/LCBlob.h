

#ifndef LivelyStore_LCBlob_h
#define LivelyStore_LCBlob_h

#include "LivelyStore.h"

LCBlobRef LCBlobCreate(LCByte data[], size_t length);

size_t LCBlobLength(LCBlobRef blob);
void LCBlobData(LCBlobRef blob, LCByte buffer[]);
LCByte* LCBlobDataRef(LCBlobRef blob);
LCBool LCBlobEqual(LCBlobRef blob, LCBlobRef anotherBlob);

// buffer[20]
LCSHARef LCBlobSHA1(LCBlobRef blob);

#endif
