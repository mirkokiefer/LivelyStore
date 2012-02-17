

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStore.h"

LCTreeRef LCTreeCreate(LCPathDataSHARef childTrees[], size_t childTreesLength,
                       LCPathDataSHARef childData[], size_t childDataLength);
LCTreeRef LCTreeCreateFromSerialized(LCStringRef serializedTree);
size_t LCTreeChildTreesLength(LCTreeRef tree);
size_t LCTreeChildDataLength(LCTreeRef tree);
LCPathDataSHARef* LCTreeChildTrees(LCTreeRef tree);
LCPathDataSHARef* LCTreeChildData(LCTreeRef tree);
LCStringRef LCTreeSHA(LCTreeRef tree);
LCStringRef LCTreeCreateSerializedString(LCTreeRef);

#endif
