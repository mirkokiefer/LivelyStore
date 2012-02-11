
#include "LivelyStore.h"

struct LCSHA {
  LCObjectInfo info;
  LCDataRef sha;
  void* object;
};

void LCSHADealloc(void* object);

LCType typeSHA = {
  .dealloc = LCSHADealloc
};

LCSHARef LCSHACreate(LCDataRef datas[], size_t count);
void computeSHA1(LCDataArrayRef datas, LCByte buffer[]);

LCSHARef LCSHACreateFromHashableObject(void* object) {
  LCSHARef newSha = malloc(sizeof(struct LCSHA) + LC_SHA1_Length);
  if (newSha != NULL) {
    LCObjectInfoRef info = (LCObjectInfoRef)object;
    LCHashableObjectRef hashableObject = info->type->meta;
    LCDataArrayRef dataArray = hashableObject->dataArrayCopy(object);
    
    newSha->info.type = &typeSHA;
    LCByte sha[LC_SHA1_Length];
    computeSHA1(dataArray, sha);
    newSha->sha = LCDataCreate(sha, LC_SHA1_Length);
    LCRelease(dataArray);
  }
  return newSha;
}

LCSHARef LCSHACreateFromHexString(LCStringRef hexString, LCDataStoreRef store) {
  LCSHARef newSha = malloc(sizeof(struct LCSHA) + LC_SHA1_Length);
  if (newSha != NULL) {
    newSha->info.type = &typeSHA;
    newSha->sha = createDataFromHexString(hexString);
  }
  return newSha;
}

LCDataRef LCSHASHAData(LCSHARef sha) {
  return sha->sha;
}

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
void computeSHA1(LCDataArrayRef array, LCByte buffer[]) {
  SHA_CTX context;
  SHA1_Init(&context);
  LCDataRef* datas = LCDataArrayDatas(array);
  for(LCInteger i=0; i<LCDataArrayLength(array); i++) {
    SHA1_Update(&context, LCDataDataRef(datas[i]), LCDataLength(datas[i]));
  }
  SHA1_Final(buffer, &context);
}
#pragma GCC diagnostic warning "-Wdeprecated-declarations"

LCStringRef LCSHACreateHexString(LCSHARef sha) {
  return createHexStringFromData(sha->sha);
}

void* LCSHAObject(LCSHARef sha) {
  return sha->object;
}

LCBool LCSHAEqual(LCSHARef sha, LCSHARef anotherSHA) {
  LCByte* sha1Bytes = LCDataDataRef(sha->sha);
  LCByte* sha2Bytes = LCDataDataRef(anotherSHA->sha);
  for(LCInteger i=0; i<LC_SHA1_Length; i++) {
    if(sha1Bytes[i] != sha2Bytes[i]) {
      return false;
    }
  }
  return true;
}

void LCSHADealloc(void* object) {
  LCRelease(((LCSHARef)object)->sha);
}

