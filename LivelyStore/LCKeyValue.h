

#ifndef LivelyStore_LCKey_h
#define LivelyStore_LCKey_h

#include "LivelyStore.h"

LCKeyValueRef LCKeyValueCreate(LCStringRef key, LCBlobRef value);
LCStringRef LCKeyValueKey(LCKeyValueRef keyValue);
LCBlobRef LCKeyValueValue(LCKeyValueRef keyValue);

#endif
