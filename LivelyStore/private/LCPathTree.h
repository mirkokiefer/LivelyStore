

#ifndef LivelyStore_LCPathTree_h
#define LivelyStore_LCPathTree_h

#include "LivelyStore.h"

LCPathTreeRef LCPathDataCreate(LCStringRef path, LCTreeRef tree);
LCStringRef LCPathDataPath(LCPathTreeRef pathTree);
LCTreeRef LCPathDataTree(LCPathTreeRef pathTree);
LCPathDataSHARef LCPathDataCreatePathDataSHA(LCPathTreeRef pathTree);

#endif
