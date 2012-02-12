

#ifndef LivelyStore_LCKeyValue_h
#define LivelyStore_LCKeyValue_h

#include "LivelyStore.h"

LCKeyValueRef LCKeyValueCreate(LCStringRef key, LCDataRef value);
LCStringRef LCKeyValueKey(LCKeyValueRef keyValue);
LCDataRef LCKeyValueValue(LCKeyValueRef keyValue);
LCKeyValueSHARef LCKeyValueCreateKeyValueSHA(LCKeyValueRef keyValue);

#endif
