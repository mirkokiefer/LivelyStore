

#ifndef LivelyStore_LCDataArray_h
#define LivelyStore_LCDataArray_h

#include "LivelyStore.h"

LCDataArrayRef LCDataArrayCreate(LCDataRef* datas, size_t length);
size_t LCDataArrayLength(LCDataArrayRef array);
LCDataRef LCDataDataAtIndex(LCDataArrayRef array, LCInteger index);
LCDataRef* LCDataArrayDatas(LCDataArrayRef array);
#endif
