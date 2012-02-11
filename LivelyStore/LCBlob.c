
#include "LCBlob.h"

struct LCBlob {
  LCObjectMeta meta;
  size_t length;
  LCSHARef sha;
  LCByte data[];
};

void LCBlobDealloc(void* object);

LCBlobRef LCBlobCreate(LCByte data[], size_t length) {
  LCBlobRef newBlob = malloc(sizeof(struct LCBlob) + length*sizeof(LCByte));
  if (newBlob != NULL) {
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

LCSHARef LCBlobSHA1(LCBlobRef blob) {
  if(blob->sha == NULL) {
    blob->sha = LCSHACreate(&blob, 1);    
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