

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStore.h"

LCTreeRef LCTreeCreate(LCKeyValueRef childTrees[], size_t childTreesLength,
                       LCKeyValueRef childDataSHAs[], size_t childDataLength);
LCTreeRef LCTreeCreateFromSerialized(LCStringRef serializedTree);
size_t LCTreeChildTreesLength(LCTreeRef tree);
size_t LCTreeChildDataLength(LCTreeRef tree);
LCKeyValueRef* LCTreeChildTrees(LCTreeRef tree);
LCKeyValueRef* LCTreeChildData(LCTreeRef tree);
LCStringRef LCTreeSHA(LCTreeRef tree);
LCStringRef LCTreeCreateSerializedString(LCTreeRef);

#endif
