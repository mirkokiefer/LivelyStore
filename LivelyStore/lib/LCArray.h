

#ifndef LivelyStore_LCArray_h
#define LivelyStore_LCArray_h

#include "LivelyStore.h"

LCArrayRef LCArrayCreate(void** objects, size_t length);
LCArrayRef LCArrayCreateFromArrays(LCArrayRef arrays[], size_t length);
void** LCArrayObjects(LCArrayRef array);
void* LCArrayObjectAtIndex(LCArrayRef array, LCInteger index);
size_t LCArrayLength(LCArrayRef array);
LCArrayRef LCArrayCreateSubArray(LCArrayRef array, LCInteger start, size_t length);
LCMutableArrayRef LCArrayCreateMutableArray(LCArrayRef array);
LCArrayRef LCArrayCreateArrayWithMap(LCArrayRef array, void* info, LCCreateEachCb each);

#endif
