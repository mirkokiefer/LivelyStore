
#ifndef LivelyStore_LCCommit_h
#define LivelyStore_LCCommit_h

#include "LivelyStoreInternal.h"
#include "LCTree.h"

typedef LCObjectRef LCCommitRef;
extern LCTypeRef LCTypeCommit;

LCCommitRef LCCommitCreate(LCTreeRef tree, LCCommitRef parents[], size_t parentsLength);
LCCommitRef* LCCommitParents(LCCommitRef commit);
size_t LCCommitParentsLength(LCCommitRef commit);
LCTreeRef LCCommitTree(LCCommitRef commit);
LCStringRef LCCommitCreateSerializedString(LCCommitRef commit);
LCCommitRef LCCommitFindParent(LCCommitRef commit, char hash[HASH_LENGTH]);
#endif
