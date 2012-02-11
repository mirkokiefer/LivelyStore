
#include "LCBlob.h"

void LCBlobDealloc(void* object);
void LCBlobBlobs(void* blob, LCBlobRef* buffer);
size_t LCBlobBlobCount(void* blob);

struct LCBlob {
  LCObjectInfo info;
  size_t length;
  LCSHARef sha;
  LCByte data[];
};

LCHashableObject hashableBlob = {
  .blobs = LCBlobBlobs,
  .blobCount = LCBlobBlobCount
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

void LCBlobBlobs(void* object, LCBlobRef* buffer) {
  LCBlobRef blob = (LCBlobRef)object;
  LCBlobRef blobs[] = {blob};
  memcpy(buffer, blobs, sizeof(LCBlobRef));
}

size_t LCBlobBlobCount(void* blob) {
  return 1;
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
  LCRelease(blob->sha);
}