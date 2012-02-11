
#include "LCBlob.h"

void LCBlobDealloc(void* object);
LCBlobArrayRef LCBlobBlobsArrayCopy(void* blob);
size_t LCBlobBlobCount(void* blob);

struct LCBlob {
  LCObjectInfo info;
  size_t length;
  LCSHARef sha;
  LCByte data[];
};

LCHashableObject hashableBlob = {
  .blobArrayCopy = LCBlobBlobsArrayCopy,
};

LCType typeBlob = {
  .dealloc = LCBlobDealloc,
  .meta = &hashableBlob
};

LCBlobRef LCBlobCreate(LCByte data[], size_t length) {
  LCBlobRef newBlob = malloc(sizeof(struct LCBlob) + length*sizeof(LCByte));
  if (newBlob != NULL) {
    newBlob->info.type = &typeBlob;
    newBlob->length = length;
    memcpy(newBlob->data, data, length*sizeof(LCByte));
  }
  return newBlob;
};

size_t LCBlobLength(LCBlobRef blob) {
  return blob->length;
}

void LCBlobData(LCBlobRef blob, LCByte buffer[]) {
  memcpy(buffer, blob->data, blob->length*sizeof(LCByte));
}

LCByte* LCBlobDataRef(LCBlobRef blob) {
  return blob->data;
}

LCBlobArrayRef LCBlobBlobsArrayCopy(void* object) {
  LCBlobRef blob = (LCBlobRef)object;
  return LCBlobArrayCreate(&blob, 1);
}

LCSHARef LCBlobSHA1(LCBlobRef blob) {
  if(blob->sha == NULL) {
    blob->sha = LCSHACreateFromHashableObject(blob);
  }
  return blob->sha;
}

LCBool LCBlobEqual(LCBlobRef blob, LCBlobRef anotherBlob) {
  LCSHARef blob1SHA = LCBlobSHA1(blob);
  LCSHARef blob2SHA = LCBlobSHA1(anotherBlob);
  return LCSHAEqual(blob1SHA, blob2SHA);
}

LCStringRef LCBLobCreateString(LCBlobRef blob) {
  return LCStringCreate((char*)(blob->data));
}

void LCBlobDealloc(void* object) {
  LCBlobRef blob = (LCBlobRef)object;
  if(blob->sha) {
    LCRelease(blob->sha);    
  }
}