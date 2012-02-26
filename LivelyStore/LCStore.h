

#ifndef LivelyStore_LCStore_h
#define LivelyStore_LCStore_h

#include "LivelyStore.h"

LCStoreRef LCStoreCreate(char* location);
void LCStoreSetStoreCallback(LCStoreRef store, LCStoreDataCb callback);
void LCStoreSetDeleteCallback(LCStoreRef store, LCDeleteDataCb callback);
void LCStorePull(LCStoreRef target, LCStoreRef source);
void LCStorePush(LCStoreRef source, LCStoreRef target);
void LCStoreCommit(LCStoreRef store, LCStageRef stage);
#endif
