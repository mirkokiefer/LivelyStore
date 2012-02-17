
#ifndef LivelyStore_LCPathDataSHA_h
#define LivelyStore_LCPathDataSHA_h

#include "LivelyStore.h"

LCPathDataSHARef LCPathDataSHACreate(LCStringRef path, LCStringRef sha);
LCStringRef LCPathDataSHAPath(LCPathDataSHARef pathValue);
LCStringRef LCPathDataSHASHA(LCPathDataSHARef pathValue);

#endif
