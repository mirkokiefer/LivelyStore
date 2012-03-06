

#ifndef LivelyStore_LCStore_h
#define LivelyStore_LCStore_h

#include "LivelyStore.h"

LCStoreRef LCStoreCreate(struct LCStoreBackend* backend);
void LCStorePull(LCStoreRef target, LCStoreRef source);
void LCStorePush(LCStoreRef source, LCStoreRef target);
void LCStoreCommit(LCStoreRef store, LCStageRef stage);
void LCStoreHead(LCStoreRef store, char* buffer[]);
void LCStoreSetHead(LCStoreRef store, char* commit);
void LCStoreCommitHistory(LCStoreRef store, char* buffer, size_t start, size_t elements);
size_t LCStoreCommitHistoryLength(LCStoreRef store);
void LCStoreDataSHA(LCStoreRef store, char* commit, char* path, char* buffer);
size_t LCStoreDataLength(LCStoreRef store, char* sha);
void LCStoreData(LCStoreRef store, char* sha, unsigned char buffer[]);
#endif
