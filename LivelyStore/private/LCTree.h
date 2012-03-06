

#ifndef LivelyStore_LCTree_h
#define LivelyStore_LCTree_h

#include "LivelyStore.h"

LCTreeRef LCTreeCreate(LCDataStoreRef store, LCDictionaryRef childTrees, LCDictionaryRef childDataSHAs);
LCTreeRef LCTreeCreateFromSHA(LCDataStoreRef store, LCStringRef sha);
LCDictionaryRef LCTreeChildTrees(LCTreeRef tree);
LCDictionaryRef LCTreeChildData(LCTreeRef tree);
LCStringRef LCTreeSHA(LCTreeRef tree);
LCStringRef LCTreeCreateSerializedString(LCTreeRef);
LCTreeRef LCTreeChildTreeAtKey(LCTreeRef tree, LCStringRef key);
LCStringRef LCTreeChildDataAtKey(LCTreeRef tree, LCStringRef key);
LCTreeRef LCTreeChildTreeAtPath(LCTreeRef tree, LCArrayRef path);
LCStringRef LCTreeChildDataAtPath(LCTreeRef tree, LCArrayRef path);
LCTreeRef LCTreeCreateTreeUpdatingData(LCTreeRef oldTree, LCDataStoreRef store, LCMutableArrayRef updatePathValues);
LCTreeRef LCTreeCopy(LCTreeRef tree);
#endif
