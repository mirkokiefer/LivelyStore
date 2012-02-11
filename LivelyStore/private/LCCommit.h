
#ifndef LivelyStore_LCCommit_h
#define LivelyStore_LCCommit_h

#include "LivelyStore.h"

LCCommitRef LCCommitCreate(LCCommitRef parent, LCTreeRef tree);
LCCommitRef LCCommitParent(LCCommitRef commit);
LCTreeRef LCCommitTree(LCCommitRef commit);
LCSHARef LCCommitSHA(LCCommitRef commit);
#endif
