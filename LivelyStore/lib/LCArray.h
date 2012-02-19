

#ifndef LivelyStore_LCArray_h
#define LivelyStore_LCArray_h

#include "LivelyStore.h"

LCArrayRef LCArrayCreate(void** objects, size_t length);
void** LCArrayObjects(LCArrayRef array);
void* LCArrayObjectAtIndex(LCArrayRef array, LCInteger index);
size_t LCArrayLength(LCArrayRef array);

#endif
