
#ifndef LivelyStore_LCPathValueSHA_h
#define LivelyStore_LCPathValueSHA_h

#include "LivelyStore.h"

LCPathValueSHARef LCPathValueSHACreate(LCStringRef key, LCSHARef value);
LCStringRef LCPathValueSHAPath(LCPathValueSHARef keyValue);
LCSHARef LCPathValueSHAValue(LCPathValueSHARef keyValue);

#endif
