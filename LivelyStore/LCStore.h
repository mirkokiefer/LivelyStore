

#ifndef LivelyStore_LCStore_h
#define LivelyStore_LCStore_h

#include "LivelyStore.h"

LCStoreRef LCStoreCreate(LCStringRef location);
void LCStoreRetain(LCStoreRef store);
void LCStoreRelease(LCStoreRef store);
void LCStorePull(LCStoreRef target, LCStoreRef source);
void LCStorePush(LCStoreRef source, LCStoreRef target);
void LCStoreAddCommit(LCStoreRef store, LCCommitRef commit);

#endif
