
#include "LCDictionary.h"

void LCDictionaryDealloc(void* object);

struct LCDictionary {
  LCObjectInfo info;
  LCMutableArrayRef keyValues;
};

LCType typeDictionary = {
  .dealloc = LCDictionaryDealloc
};

LCDictionaryRef LCDictionaryCreate(LCKeyValueRef keyValues[], size_t length) {
  LCDictionaryRef newDict = LCNewObject(&typeDictionary, sizeof(struct LCDictionary));
  newDict->keyValues = LCMutableArrayCreate((void**)keyValues, length);
  return newDict;
};

void* LCDictionaryValueForKey(LCDictionaryRef dict, void* key) {
  LCKeyValueRef currKeyValue;
  for (LCInteger i=0; i<LCMutableArrayLength(dict->keyValues); i++) {
    currKeyValue = LCMutableArrayObjectAtIndex(dict->keyValues, i);
    if(LCCompareObjects(key, LCKeyValueKey(currKeyValue)) == LCEqual) {
      return LCKeyValueValue(currKeyValue);
    }
  }
  return NULL;
}

void LCDictionaryDeleteKey(LCDictionaryRef dict, void* key) {
  LCKeyValueRef currKeyValue;
  for (LCInteger i=0; i<LCMutableArrayLength(dict->keyValues); i++) {
    currKeyValue = LCMutableArrayObjectAtIndex(dict->keyValues, i);
    if(LCCompareObjects(key, LCKeyValueKey(currKeyValue)) == LCEqual) {
      LCMutableArrayRemoveIndex(dict->keyValues, i);
    }
  }
}

void LCDictionarySetValueForKey(LCDictionaryRef dict, void* key, void* value) {
  LCDictionaryDeleteKey(dict, key);
  LCKeyValueRef keyValue = LCKeyValueCreate(key, value);
  LCMutableArrayAddObject(dict->keyValues, keyValue);
}

void LCDictionaryAddEntry(LCDictionaryRef dict, LCKeyValueRef keyValue) {
  LCDictionaryDeleteKey(dict, LCKeyValueKey(keyValue));
  if (LCKeyValueValue(keyValue) != NULL) {
    LCMutableArrayAddObject(dict->keyValues, keyValue);
  }
}

void LCDictionaryAddEntries(LCDictionaryRef dict, LCKeyValueRef keyValues[], size_t length) {
  for (LCInteger i=0; i<length; i++) {
    LCDictionaryAddEntry(dict, keyValues[i]);
  }
}

LCDictionaryRef LCDictionaryCopy(LCDictionaryRef dict) {
  return LCDictionaryCreate((LCKeyValueRef*)LCMutableArrayObjects(dict->keyValues), LCMutableArrayLength(dict->keyValues));
}

size_t LCDictionaryLength(LCDictionaryRef dict) {
  return LCMutableArrayLength(dict->keyValues);
}

LCKeyValueRef* LCDictionaryEntries(LCDictionaryRef dict) {
  return (LCKeyValueRef*)LCMutableArrayObjects(dict->keyValues);
}

void LCDictionaryDealloc(void* object) {
  LCDictionaryRef dict = (LCDictionaryRef)object;
  LCRelease(dict->keyValues);
}
