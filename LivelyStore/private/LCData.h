

#ifndef LivelyStore_LCData_h
#define LivelyStore_LCData_h

#include "LivelyStore.h"

LCDataRef LCDataCreate(LCByte data[], size_t length);

size_t LCDataLength(LCDataRef data);
void LCDataData(LCDataRef data, LCByte buffer[]);
LCByte* LCDataDataRef(LCDataRef data);
LCBool LCDataEqual(LCDataRef data, LCDataRef anotherData);
LCSHARef LCDataSHA1(LCDataRef data);
LCStringRef LCBLobCreateString(LCDataRef data);
#endif
