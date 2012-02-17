

#ifndef LivelyStore_LCPathValue_h
#define LivelyStore_LCPathValue_h

#include "LivelyStore.h"

LCPathValueRef LCPathValueCreate(LCStringRef key, LCDataRef value);
LCStringRef LCPathValuePath(LCPathValueRef keyValue);
LCDataRef LCPathValueValue(LCPathValueRef keyValue);
LCPathValueSHARef LCPathValueCreatePathValueSHA(LCPathValueRef keyValue);

#endif
