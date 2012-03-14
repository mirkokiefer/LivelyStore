
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

LCKeyValueRef LCDictionaryEntryForKey(LCDictionaryRef dict, void* key) {
  LCKeyValueRef currKeyValue;
  for (LCInteger i=0; i<LCMutableArrayLength(dict->keyValues); i++) {
    currKeyValue = LCMutableArrayObjectAtIndex(dict->keyValues, i);
    if(LCCompareObjects(key, LCKeyValueKey(currKeyValue)) == LCEqual) {
      return currKeyValue;
    }
  }
  return NULL;
}

void* LCDictionaryValueForKey(LCDictionaryRef dict, void* key) {
  LCKeyValueRef entry = LCDictionaryEntryForKey(dict, key);
  if (entry) {
    return LCKeyValueValue(entry);
  } else {
    return NULL;
  }
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

LCMutableArrayRef LCDictionaryCreateChangesArray(LCDictionaryRef original, LCDictionaryRef new) {
  LCDictionaryRef changes = LCDictionaryCreate(NULL, 0);
  LCKeyValueRef* originalKeyValues = (LCKeyValueRef*)LCMutableArrayObjects(original->keyValues);
  LCKeyValueRef* newKeyValues = (LCKeyValueRef*)LCMutableArrayObjects(new->keyValues);

  for (LCInteger i=0; i<LCMutableArrayLength(original->keyValues); i++) {
    void* key = LCKeyValueKey(originalKeyValues[i]);
    void* value = LCKeyValueValue(originalKeyValues[i]);
    void* newValue = LCDictionaryValueForKey(new, key);
    if (LCCompareObjects(value, newValue) != LCEqual) {
      LCDictionarySetValueForKey(changes, key, newValue);
    }
  }
  for (LCInteger i=0; i<LCMutableArrayLength(new->keyValues); i++) {
    void* key = LCKeyValueKey(newKeyValues[i]);
    void* newValue = LCKeyValueValue(newKeyValues[i]);
    void* originalValue = LCDictionaryValueForKey(original, key);
    if (LCCompareObjects(originalValue, newValue) != LCEqual) {
      LCDictionarySetValueForKey(changes, key, newValue);
    }
  }
  LCMutableArrayRef result = LCRetain(changes->keyValues);
  LCRelease(changes);
  return result;
}

LCMutableArrayRef LCDictionaryCreateAddedArray(LCDictionaryRef original, LCDictionaryRef new) {
  LCMutableArrayRef changes = LCMutableArrayCreate(NULL, 0);
  LCKeyValueRef* newKeyValues = (LCKeyValueRef*)LCMutableArrayObjects(new->keyValues);
  for (LCInteger i=0; i<LCMutableArrayLength(new->keyValues); i++) {
    void* key = LCKeyValueKey(newKeyValues[i]);
    void* originalValue = LCDictionaryValueForKey(original, key);
    if (originalValue == NULL) {
      LCMutableArrayAddObject(changes, newKeyValues[i]);
    }
  }
  return changes;
}

LCMutableArrayRef LCDictionaryCreateUpdatedArray(LCDictionaryRef original, LCDictionaryRef new) {
  LCMutableArrayRef changes = LCMutableArrayCreate(NULL, 0);
  LCKeyValueRef* originalKeyValues = (LCKeyValueRef*)LCMutableArrayObjects(original->keyValues);
  for (LCInteger i=0; i<LCMutableArrayLength(original->keyValues); i++) {
    void* key = LCKeyValueKey(originalKeyValues[i]);
    void* value = LCKeyValueValue(originalKeyValues[i]);
    LCKeyValueRef newEntry = LCDictionaryEntryForKey(new, key);
    void* newValue = LCKeyValueValue(newEntry);
    if ((LCCompareObjects(value, newValue) != LCEqual) && (newValue != NULL)) {
      LCMutableArrayAddObject(changes, newEntry);
    }
  }
  return changes;
}

LCMutableArrayRef LCDictionaryCreateDeletedArray(LCDictionaryRef original, LCDictionaryRef new) {
  LCMutableArrayRef changes = LCMutableArrayCreate(NULL, 0);
  LCKeyValueRef* originalKeyValues = (LCKeyValueRef*)LCMutableArrayObjects(original->keyValues);
  for (LCInteger i=0; i<LCMutableArrayLength(original->keyValues); i++) {
    void* key = LCKeyValueKey(originalKeyValues[i]);
    void* newValue = LCDictionaryValueForKey(new, key);
    if (newValue == NULL) {
      LCMutableArrayAddObject(changes, key);
    }
  }
  return changes;
}


void LCDictionaryDealloc(void* object) {
  LCDictionaryRef dict = (LCDictionaryRef)object;
  LCRelease(dict->keyValues);
}
