
#include "LivelyStore.h"

struct LCSHA {
  LCObjectInfo info;
  LCBlobRef sha;
  void* object;
};

void LCSHADealloc(void* object);

LCType typeSHA = {
  .dealloc = LCSHADealloc
};

LCSHARef LCSHACreate(LCBlobRef blobs[], size_t count);
void computeSHA1(LCBlobRef blobs[], size_t count, LCByte buffer[]);

LCSHARef LCSHACreateFromHashableObject(void* object) {
  LCObjectInfoRef info = (LCObjectInfoRef)object;
  LCHashableObjectRef hashableObject = info->type->meta;
  LCBlobArrayRef blobArray = hashableObject->blobArrayCopy(object);
  LCSHARef sha = LCSHACreate(LCBlobArrayBlobs(blobArray), LCBlobArrayLength(blobArray));
  LCRelease(blobArray);
  return sha;
}

LCSHARef LCSHACreateFromHexString(LCStringRef hexString, LCBlobStoreRef store) {
  LCSHARef newSha = malloc(sizeof(struct LCSHA) + LC_SHA1_Length);
  if (newSha != NULL) {
    newSha->info.type = &typeSHA;
    newSha->sha = createBlobFromHexString(hexString);
  }
  return newSha;
}

LCSHARef LCSHACreate(LCBlobRef blobs[], size_t count) {
  LCSHARef newSha = malloc(sizeof(struct LCSHA) + LC_SHA1_Length);
  if (newSha != NULL) {
    newSha->info.type = &typeSHA;
    LCByte sha[LC_SHA1_Length];
    computeSHA1(blobs, count, sha);
    newSha->sha = LCBlobCreate(sha, LC_SHA1_Length);
  }
  return newSha;
};

LCBlobRef LCSHASHABlob(LCSHARef sha) {
  return sha->sha;
}

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
void computeSHA1(LCBlobRef blobs[], size_t count, LCByte buffer[]) {
  SHA_CTX context;
  SHA1_Init(&context);
  for(LCInteger i=0; i<count; i++) {
    SHA1_Update(&context, LCBlobDataRef(blobs[i]), LCBlobLength(blobs[i]));
  }
  SHA1_Final(buffer, &context);
}
#pragma GCC diagnostic warning "-Wdeprecated-declarations"

LCStringRef LCSHACreateHexString(LCSHARef sha) {
  return createHexStringFromBlob(sha->sha);
}

void* LCSHAObject(LCSHARef sha) {
  return sha->object;
}

LCBool LCSHAEqual(LCSHARef sha, LCSHARef anotherSHA) {
  LCByte* sha1Bytes = LCBlobDataRef(sha->sha);
  LCByte* sha2Bytes = LCBlobDataRef(anotherSHA->sha);
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

