
#include "LivelyStoreTests.h"

void LCRetain(void* object) {
  LCObjectMeta* meta = (LCObjectMeta*)object;
  (*meta).rCount++;
}

void LCRelease(void* object) {
  LCObjectMeta* meta = (LCObjectMeta*)object;
  (*meta).rCount--;
  if((*meta).rCount == -1) {
    if((*meta).dealloc) {
      (*meta).dealloc(object);      
    }
    free(object);
    //printf("dealloc: %p", object);
  }
}

LCInteger LCRetainCount(void* object) {
  LCObjectMeta* meta = (LCObjectMeta*)object;
  return (*meta).rCount;
}