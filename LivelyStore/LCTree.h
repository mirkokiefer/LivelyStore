

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStore.h"

LCTreeRef LCTreeCreate(LCTreeRef childTrees[], LCEntryRef childEntries[]);

void LCTreeSHA1(LCTreeRef tree, unsigned char* buffer);


#endif
