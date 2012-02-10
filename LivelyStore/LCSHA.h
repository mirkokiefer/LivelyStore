
#ifndef LivelyStore_LCSHA_h
#define LivelyStore_LCSHA_h

#include "LivelyStore.h"

LCSHARef LCSHACreate(LCBlobRef blob);
void LCSHARetain(LCSHARef sha);
void LCSHARelease(LCSHARef sha);
LCStringRef LCSHAHexString(LCSHARef sha);

#endif
