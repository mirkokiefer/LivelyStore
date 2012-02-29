
#ifndef LivelyStore_imac_LCDictionary_h
#define LivelyStore_imac_LCDictionary_h

#include "LivelyStore.h"

LCDictionaryRef LCDictionaryCreate(LCKeyValueRef keyValues[], size_t length);
void* LCDictionaryValueForKey(LCDictionaryRef dict, void* key);
void LCDictionarySetValueForKey(LCDictionaryRef dict, void* key, void* value);
void LCDictionaryDeleteKey(LCDictionaryRef dict, void* key);
LCDictionaryRef LCDictionaryCopy(LCDictionaryRef dict);

#endif
