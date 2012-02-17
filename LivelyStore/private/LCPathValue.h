

#ifndef LivelyStore_LCPathData_h
#define LivelyStore_LCPathData_h

#include "LivelyStore.h"

LCPathDataRef LCPathDataCreate(LCStringRef key, LCDataRef value);
LCStringRef LCPathDataPath(LCPathDataRef keyValue);
LCDataRef LCPathDataValue(LCPathDataRef keyValue);
LCPathDataSHARef LCPathDataCreatePathDataSHA(LCPathDataRef keyValue);

#endif
