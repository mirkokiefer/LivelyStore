

#ifndef LivelyStore_LCMutableArray_h
#define LivelyStore_LCMutableArray_h

#include "LivelyStore.h"

LCMutableArrayRef LCMutableArrayCreate(void** objects, size_t length);
void** LCMutableArrayObjects(LCMutableArrayRef array);
void* LCMutableArrayObjectAtIndex(LCMutableArrayRef array, LCInteger index);
size_t LCMutableArrayLength(LCMutableArrayRef array);
LCArrayRef LCMutableArrayCreateSubArray(LCMutableArrayRef array, LCInteger start, size_t length);

LCArrayRef LCMutableArrayCreateArray(LCMutableArrayRef array);
void LCMutableArrayAddObject(LCMutableArrayRef array, void* object);
void LCMutableArrayRemoveIndex(LCMutableArrayRef array, LCInteger index);
void LCMutableArrayRemoveObject(LCMutableArrayRef array, void* object);
void LCMutableArraySort(LCMutableArrayRef array);

#endif
