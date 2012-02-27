

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStore.h"

LCTreeRef LCTreeCreate(LCArrayRef childTrees, LCArrayRef childDataSHAs);
LCTreeRef LCTreeCreateFromSerialized(LCStringRef serializedTree);
LCMutableArrayRef LCTreeChildTrees(LCTreeRef tree);
LCMutableArrayRef LCTreeChildData(LCTreeRef tree);
LCStringRef LCTreeSHA(LCTreeRef tree);
LCStringRef LCTreeCreateSerializedString(LCTreeRef);
LCTreeRef LCTreeCreateTreeDeletingData(LCTreeRef oldTree, LCStringRef deletePaths[], size_t length);
LCTreeRef LCTreeCreateTreeAddingData(LCTreeRef tree, LCKeyValueRef pathData[], size_t length);

#endif
