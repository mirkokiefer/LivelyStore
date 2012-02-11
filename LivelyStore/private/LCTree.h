

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStore.h"

LCTreeRef LCTreeCreate(LCKeyValueSHARef childTrees[], size_t childTreesLength,
                       LCKeyValueSHARef childBlobs[], size_t childBlobsLength);
LCTreeRef LCTreeCreateFromSerialized(LCStringRef serializedTree);
size_t LCTreeChildTreesLength(LCTreeRef tree);
size_t LCTreeChildBlobsLength(LCTreeRef tree);
LCKeyValueSHARef* LCTreeChildTrees(LCTreeRef tree);
LCKeyValueSHARef* LCTreeChildBlobs(LCTreeRef tree);
LCSHARef LCTreeSHA1(LCTreeRef tree);
LCStringRef LCTreeSerialize(LCTreeRef);

#endif
