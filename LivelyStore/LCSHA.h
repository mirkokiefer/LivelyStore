
#ifndef LivelyStore_LCSHA_h
#define LivelyStore_LCSHA_h

#include "LivelyStore.h"

LCSHARef LCSHACreate(unsigned char* data, size_t length);
void LCSHARetain(LCSHARef sha);
void LCSHARelease(LCSHARef sha);
LCStringRef LCSHAHexString(LCSHARef sha);

#endif
