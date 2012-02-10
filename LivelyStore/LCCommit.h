

#ifndef LivelyStore_LCCommit_h
#define LivelyStore_LCCommit_h

#include "LivelyStore.h"

LCCommitRef LCCommitCreate();
void LCCommitAddEntry(LCCommitRef commit, LCKeyValueRef keyValue);
LCSHARef LCCommitSHA1(LCCommitRef commit);


#endif
