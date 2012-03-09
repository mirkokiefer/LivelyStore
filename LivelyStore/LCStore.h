

#ifndef LivelyStore_LCStore_h
#define LivelyStore_LCStore_h

#include "LivelyStore.h"

LCStoreRef LCStoreCreate(struct LCStoreBackend* backend, char* headSHA);
void LCStorePull(LCStoreRef target, LCStoreRef source);
void LCStorePush(LCStoreRef source, LCStoreRef target);
void LCStoreCommit(LCStoreRef store, LCStageRef stage);
LCCommitRef LCStoreHead(LCStoreRef store);
LCStringRef LCStoreDataSHA(LCStoreRef store, LCCommitRef commit, char* path);
LCDataRef LCStoreData(LCStoreRef store, LCStringRef dataSHA);
#endif
