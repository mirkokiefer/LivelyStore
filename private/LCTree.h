

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStoreInternal.h"

typedef LCObjectRef LCTreeRef;
extern LCTypeRef LCTypeTree;

LCTreeRef LCTreeCreate(LCKeyValueRef children[], size_t childrenLength);
size_t LCTreeChildrenLength(LCTreeRef tree);
LCKeyValueRef* LCTreeChildren(LCTreeRef tree);
LCObjectRef LCTreeChildAtKey(LCTreeRef tree, LCStringRef key);
LCObjectRef LCTreeChildAtPath(LCTreeRef tree, LCArrayRef path);
LCTreeRef LCTreeCreateTreeUpdatingData(LCTreeRef oldTree, LCKeyValueRef updatePathValues[], size_t updatePathValuesLength);
LCTreeRef LCTreeCopy(LCTreeRef tree);
LCArrayRef LCTreeChangedPathValues(LCTreeRef originalTree, LCTreeRef newTree);
#endif
