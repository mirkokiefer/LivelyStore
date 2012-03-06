
#include "LCMemoryStore.h"

void memoryStoreStoreData(void* store, LCDataType type, char* sha, unsigned char data[], size_t length);
void memoryStoreDeleteData(void* store, LCDataType type, char* sha);
size_t memoryStoreGetDataLength(void* store, LCDataType type, char* sha);
void memoryStoreGetData(void* store, LCDataType type, char* sha, unsigned char buffer[]);

struct LCMemoryStore {
  LCObjectInfo info;
  LCStringRef location;
  LCDictionaryRef dict;
};

void LCMemoryStoreDealloc(void* object);

LCType typeMemoryStore = {
  .dealloc = LCMemoryStoreDealloc
};

LCMemoryStoreRef LCMemoryStoreCreate(char* location) {
  LCMemoryStoreRef newStore = malloc(sizeof(struct LCMemoryStore));
  if (newStore != NULL) {
    newStore->info.type = &typeMemoryStore;
    newStore->location = LCStringCreate(location);
    newStore->dict = LCDictionaryCreate(NULL, 0);
  }
  return newStore;
};

void LCMemoryStoreRegister(LCMemoryStoreRef store, LCStoreRef storeInterface) {
  LCStoreSetStoreObject(storeInterface, store);
  LCStoreSetStoreCallback(storeInterface, memoryStoreStoreData);
}

void LCMemoryStoreDealloc(void* object) {
  LCMemoryStoreRef store = (LCMemoryStoreRef)object;
  LCRelease(store->location);
  LCRelease(store->dict);
}

void memoryStoreStoreData(void* storeObj, LCDataType type, char* sha, unsigned char data[], size_t length) {
  LCMemoryStoreRef store = (LCMemoryStoreRef)storeObj;
  LCDataRef dataObj = LCDataCreate(data, length);
  LCStringRef shaObj = LCStringCreate(sha);
  LCDictionarySetValueForKey(store->dict, shaObj, dataObj);
  LCRelease(dataObj);
  LCRelease(shaObj);
}

void memoryStoreDeleteData(void* storeObj, LCDataType type, char* sha) {
  LCMemoryStoreRef store = (LCMemoryStoreRef)storeObj;
  LCStringRef shaObj = LCStringCreate(sha);
  LCDictionaryDeleteKey(store->dict, shaObj);
  LCRelease(shaObj);
}

size_t memoryStoreGetDataLength(void* storeObj, LCDataType type, char* sha) {
  LCMemoryStoreRef store = (LCMemoryStoreRef)storeObj;
  LCStringRef shaObj = LCStringCreate(sha);
  LCDataRef data = LCDictionaryValueForKey(store->dict, shaObj);
  LCRelease(shaObj);
  return LCDataLength(data);
}

void memoryStoreGetData(void* storeObj, LCDataType type, char* sha, unsigned char buffer[]) {
  LCMemoryStoreRef store = (LCMemoryStoreRef)storeObj;
  LCStringRef shaObj = LCStringCreate(sha);
  LCDataRef data = LCDictionaryValueForKey(store->dict, shaObj);
  LCRelease(shaObj);
  memcpy(buffer, LCDataDataRef(data), LCDataLength(data));
}