
#include "LCBlobArray.h"

void LCBlobArrayDealloc(void* object);
LCBlobArrayRef LCBlobArrayCopy(void* blob);
size_t LCBlobArrayBlobCount(void* blob);

struct LCBlobArray {
  LCObjectInfo info;
  size_t length;
  LCBlobRef blobs[];
};

LCHashableObject hashableBlobArray = {
  .blobArrayCopy = LCBlobArrayCopy,
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

LCBlobRef* LCBlobArrayBlobs(LCBlobArrayRef array) {
  return array->blobs;
}

LCBlobArrayRef LCBlobArrayCopy(void* object) {
  LCRetain(object);
  return object;
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