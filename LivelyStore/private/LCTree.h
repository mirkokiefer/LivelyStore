

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStore.h"

LCTreeRef LCTreeCreate(LCPathDataSHARef childTrees[], size_t childTreesLength,
                       LCPathDataSHARef childDatas[], size_t childDatasLength);
LCTreeRef LCTreeCreateFromSerialized(LCStringRef serializedTree);
size_t LCTreeChildTreesLength(LCTreeRef tree);
size_t LCTreeChildDatasLength(LCTreeRef tree);
LCPathDataSHARef* LCTreeChildTrees(LCTreeRef tree);
LCPathDataSHARef* LCTreeChildDatas(LCTreeRef tree);
LCSHARef LCTreeSHA(LCTreeRef tree);
LCStringRef LCTreeSerialize(LCTreeRef);

#endif
