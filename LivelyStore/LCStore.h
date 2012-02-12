

#ifndef LivelyStore_LCStore_h
#define LivelyStore_LCStore_h

#include "LivelyStore.h"

LCStoreRef LCStoreCreate(char* location);
void LCStoreSetNewDataCallback(void(*callback)(char* sha, unsigned char* data, size_t length));
void LCStoreSetDeletedDataCallback(void(*callback)(char* sha));
void LCStorePull(LCStoreRef target, LCStoreRef source);
void LCStorePush(LCStoreRef source, LCStoreRef target);
void LCStoreCommit(LCStoreRef store, LCStageRef stage);
#endif
