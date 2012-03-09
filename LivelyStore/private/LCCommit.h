
#ifndef LivelyStore_LCCommit_h
#define LivelyStore_LCCommit_h

#include "LivelyStore.h"

LCCommitRef LCCommitCreateFromSHA(LCDataStoreRef store, LCStringRef sha);
LCCommitRef LCCommitCreate(LCDataStoreRef store, LCTreeRef tree, LCCommitRef parents[], size_t parentsLength);
LCCommitRef* LCCommitParents(LCCommitRef commit);
size_t LCCommitParentsLength(LCCommitRef commit);
LCTreeRef LCCommitTree(LCCommitRef commit);
LCStringRef LCCommitCreateSerializedString(LCCommitRef commit);
LCStringRef LCCommitSHA(LCCommitRef commit);
LCCommitRef LCCommitFindParent(LCCommitRef commit, LCStringRef sha);
#endif
