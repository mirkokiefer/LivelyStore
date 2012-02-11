

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStore.h"

LCTreeRef LCTreeCreate(LCKeyValueSHARef childTrees[], size_t childTreesLength,
                       LCKeyValueSHARef childDatas[], size_t childDatasLength);
LCTreeRef LCTreeCreateFromSerialized(LCStringRef serializedTree);
size_t LCTreeChildTreesLength(LCTreeRef tree);
size_t LCTreeChildDatasLength(LCTreeRef tree);
LCKeyValueSHARef* LCTreeChildTrees(LCTreeRef tree);
LCKeyValueSHARef* LCTreeChildDatas(LCTreeRef tree);
LCSHARef LCTreeSHA(LCTreeRef tree);
LCStringRef LCTreeSerialize(LCTreeRef);

#endif
