
#include "LivelyStore.h"

struct LCBlob {
  LCInteger rCount;
  size_t length;
  LCByte data[];
};

static inline void LCBlobDealloc(LCBlobRef aStruct);

LCBlobRef LCBlobCreate(LCByte data[], size_t length) {
  LCBlobRef newBlob = malloc(sizeof(struct LCBlob) + length*sizeof(LCByte));
  if (newBlob != NULL) {
    newBlob->length = length;
    memcpy(newBlob->data, data, length*sizeof(LCByte));
  }
  return newBlob;
};

void LCBlobRetain(LCBlobRef aStruct) {
  aStruct->rCount = aStruct->rCount + 1;
}

void LCBlobRelease(LCBlobRef aStruct) {
  aStruct->rCount = aStruct->rCount - 1;
  if(aStruct->rCount == 0) {
    LCBlobDealloc(aStruct);
  }
}

static inline void LCBlobDealloc(LCBlobRef aStruct) {
  free(aStruct);
}

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
  LCSHARef sha = LCSHACreate(blob);
  return sha;
}