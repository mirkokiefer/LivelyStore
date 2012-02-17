
#ifndef LivelyStore_LCKeyValue_h
#define LivelyStore_LCKeyValue_h

#include "LivelyStore.h"

LCKeyValueRef LCKeyValueCreate(void* keyObject, void* valueObject);
void* LCKeyValueKey(LCKeyValueRef keyValue);
void* LCKeyValueValue(LCKeyValueRef keyValue);

#endif
