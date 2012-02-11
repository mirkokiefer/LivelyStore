
#ifndef LivelyStore_LCKeyValueSHA_h
#define LivelyStore_LCKeyValueSHA_h

#include "LivelyStore.h"

LCKeyValueSHARef LCKeyValueSHACreate(LCStringRef key, LCSHARef value);
LCStringRef LCKeyValueSHAKey(LCKeyValueSHARef keyValue);
LCSHARef LCKeyValueSHAValue(LCKeyValueSHARef keyValue);

#endif
