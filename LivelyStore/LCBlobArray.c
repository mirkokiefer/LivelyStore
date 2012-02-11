
#include "LCBlobArray.h"

void LCBlobArrayDealloc(void* object);
void LCBlobArrayBlobRef(void* blob, LCBlobRef* buffer);
size_t LCBlobArrayBlobCount(void* blob);

struct LCBlobArray {
  LCObjectInfo info;
  size_t length;
  LCBlobRef blobs[];
};

LCHashableObject hashableBlobArray = {
  .blobs = LCBlobArrayBlobRef,
  .blobCount = LCBlobArrayBlobCount
};

LCType typeBlobArray = {
  .dealloc = LCBlobArrayDealloc,
  .meta = &hashableBlobArray
};

LCBlobArrayRef LCBlobArrayCreate(LCBlobRef* blobs, size_t length) {
  LCBlobArrayRef newArray = malloc(sizeof(struct LCBlobArray) + length*sizeof(LCBlobRef));
  if (newArray != NULL) {
    newArray->info.type = &typeBlobArray;
    
    for (LCInteger i=0; i<length; i++) {
      LCRetain(blobs[i]);
    }
    newArray->length = length;
    memcpy(newArray->blobs, blobs, length*sizeof(LCBlobRef));
  }
  return newArray;
};

size_t LCBlobArrayLength(LCBlobArrayRef array) {
  return array->length;
}

LCBlobRef LCBlobBlobAtIndex(LCBlobArrayRef blob, LCInteger index) {
  if(index < blob->length) {
    return blob->blobs[index];    
  } else {
    return NULL;
  }
}

void LCBlobArrayBlobRef(void* object, LCBlobRef* buffer) {
  LCBlobArrayRef array = (LCBlobArrayRef)object;
  memcpy(buffer, array->blobs, array->length*sizeof(LCBlobRef));
}

size_t LCBlobArrayBlobCount(void* blob) {
  return ((LCBlobArrayRef)blob)->length;
}

void LCBlobArrayDealloc(void* object) {
  LCBlobArrayRef array = (LCBlobArrayRef)object;
  for (LCInteger i=0; i<array->length; i++) {
    LCRelease(array->blobs[i]);
  }
}