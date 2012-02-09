
#include "LivelyStore.h"

struct LCBlob {
  LCInteger rCount;
  size_t size;
  void* data;
};

static inline void LCBlobDealloc(LCBlobRef aStruct);

LCBlobRef LCBlobCreate(void* data, size_t size) {
  LCBlobRef newBlob = malloc(sizeof(struct LCBlob));
  if (newBlob != NULL) {
    void* dataMemory = malloc(size);
    if(dataMemory != NULL) {
      newBlob->size = size;
      memcpy(dataMemory, data, size);
      newBlob->data = dataMemory;
    } else {
      free(newBlob);
      return NULL;
    }
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

size_t LCBlobSize(LCBlobRef blob) {
  return blob->size;
}

void LCBlobData(LCBlobRef blob, void* buffer) {
  memcpy(buffer, blob->data, blob->size);
}

void LCBlobSHA1(LCBlobRef blob, unsigned char* buffer) {
  unsigned char* dataChars = (unsigned char*)blob->data;
  SHA1(dataChars, strlen(dataChars), buffer);
}