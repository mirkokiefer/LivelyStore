

#ifndef LivelyStore_LCData_h
#define LivelyStore_LCData_h

#include "LivelyStore.h"

LCDataRef LCDataCreate(LCByte data[], size_t length);
size_t LCDataLength(LCDataRef data);
LCByte* LCDataDataRef(LCDataRef data);
LCStringRef LCDataSHA1(LCDataRef data);
#endif
