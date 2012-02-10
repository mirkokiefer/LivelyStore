
#include "LivelyStore.h"

struct LCBlob {
  LCObjectMeta meta;
  size_t length;
  LCByte data[];
};

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
  LCSHARef sha = LCSHACreate(&blob, 1);
  return sha;
}