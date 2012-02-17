
#ifndef LivelyStore_LCPathDataSHA_h
#define LivelyStore_LCPathDataSHA_h

#include "LivelyStore.h"

LCPathDataSHARef LCPathDataSHACreate(LCStringRef key, LCSHARef value);
LCStringRef LCPathDataSHAPath(LCPathDataSHARef keyValue);
LCSHARef LCPathDataSHAValue(LCPathDataSHARef keyValue);

#endif
