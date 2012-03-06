
#ifndef LivelyStore_LCCommit_h
#define LivelyStore_LCCommit_h

#include "LivelyStore.h"

LCCommitRef LCCommitCreateFromSHA(LCDataStoreRef store, LCStringRef sha);
LCCommitRef LCCommitCreate(LCDataStoreRef store, LCCommitRef parent, LCTreeRef tree);
LCCommitRef LCCommitParent(LCCommitRef commit);
LCTreeRef LCCommitTree(LCCommitRef commit);
LCStringRef LCCommitCreateSerializedString(LCCommitRef commit);
LCStringRef LCCommitSHA(LCCommitRef commit);
#endif
