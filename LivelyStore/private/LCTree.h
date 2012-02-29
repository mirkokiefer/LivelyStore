

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStore.h"

LCTreeRef LCTreeCreate(LCDictionaryRef childTrees, LCDictionaryRef childDataSHAs);
LCTreeRef LCTreeCreateFromSerialized(LCStringRef serializedTree);
LCDictionaryRef LCTreeChildTrees(LCTreeRef tree);
LCDictionaryRef LCTreeChildData(LCTreeRef tree);
LCStringRef LCTreeSHA(LCTreeRef tree);
LCStringRef LCTreeCreateSerializedString(LCTreeRef);
LCTreeRef LCTreeCreateTreeDeletingData(LCTreeRef oldTree, LCMutableArrayRef deletePaths);
LCTreeRef LCTreeCreateTreeAddingData(LCTreeRef tree, LCKeyValueRef pathData[], size_t length);
LCTreeRef LCTreeCopy(LCTreeRef tree);
#endif
