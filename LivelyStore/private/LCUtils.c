
#include "LivelyStoreTests.h"

void LCRetain(void* object) {
  LCObjectInfo* info = (LCObjectInfo*)object;
  info->rCount++;
}

void LCRelease(void* object) {
  LCObjectInfo* info = (LCObjectInfo*)object;
  info->rCount--;
  if(info->rCount == -1) {
    LCTypeRef type = (*info).type;
    if(type->dealloc) {
      type->dealloc(object);      
    }
    free(object);
    //printf("dealloc: %p", object);
  }
}

LCInteger LCRetainCount(void* object) {
  LCObjectInfo* info = (LCObjectInfo*)object;
  return info->rCount;
}