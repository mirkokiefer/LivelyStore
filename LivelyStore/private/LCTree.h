

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStore.h"

LCTreeRef LCTreeCreate(LCPathValueSHARef childTrees[], size_t childTreesLength,
                       LCPathValueSHARef childDatas[], size_t childDatasLength);
LCTreeRef LCTreeCreateFromSerialized(LCStringRef serializedTree);
size_t LCTreeChildTreesLength(LCTreeRef tree);
size_t LCTreeChildDatasLength(LCTreeRef tree);
LCPathValueSHARef* LCTreeChildTrees(LCTreeRef tree);
LCPathValueSHARef* LCTreeChildDatas(LCTreeRef tree);
LCSHARef LCTreeSHA(LCTreeRef tree);
LCStringRef LCTreeSerialize(LCTreeRef);

#endif
