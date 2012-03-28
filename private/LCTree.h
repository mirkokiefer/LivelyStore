

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStoreInternal.h"

typedef LCObjectRef LCTreeRef;
extern LCTypeRef LCTypeTree;

LCTreeRef LCTreeCreate(LCKeyValueRef childTrees[], size_t childTreesLength, LCKeyValueRef childData[], size_t childDataLength);
size_t LCTreeChildTreesLength(LCTreeRef tree);
LCKeyValueRef* LCTreeChildTrees(LCTreeRef tree);
size_t LCTreeChildDataLength(LCTreeRef tree);
LCKeyValueRef* LCTreeChildData(LCTreeRef tree);
LCTreeRef LCTreeChildTreeAtKey(LCTreeRef tree, LCStringRef key);
LCDataRef LCTreeChildDataAtKey(LCTreeRef tree, LCStringRef key);
LCTreeRef LCTreeChildTreeAtPath(LCTreeRef tree, LCArrayRef path);
LCObjectRef LCTreeChildDataAtPath(LCTreeRef tree, LCArrayRef path);
LCTreeRef LCTreeCreateTreeUpdatingData(LCTreeRef oldTree, LCMutableArrayRef updatePathValues);
LCTreeRef LCTreeCopy(LCTreeRef tree);
void LCTreeChangedPathValues(LCTreeRef originalTree, LCTreeRef newTree, LCMutableArrayRef changedData, LCMutableArrayRef changedTrees);
#endif
