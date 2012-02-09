

#ifndef LivelyStore_LCCommit_h
#define LivelyStore_LCCommit_h

#include "LivelyStore.h"

LCCommitRef LCCommitCreate();
void LCCommitAddEntry(LCCommitRef commit, LCStringRef key, LCBlobRef blob);
void LCCommitRemoveEntry(LCCommitRef commit, LCStringRef key);

void LCCommitSHA1(LCCommitRef commit, unsigned char* buffer);


#endif
