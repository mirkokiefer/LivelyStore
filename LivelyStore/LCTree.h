

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStore.h"

LCTreeRef LCTreeCreate(LCTreeRef childTrees[], LCKeyValueRef childEntries[]);

LCSHARef LCTreeSHA1(LCTreeRef tree);


#endif
