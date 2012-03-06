

#ifndef LivelyStore_LCStore_h
#define LivelyStore_LCStore_h

#include "LivelyStore.h"

LCStoreRef LCStoreCreate(struct LCStoreBackend* backend);
void LCStorePull(LCStoreRef target, LCStoreRef source);
void LCStorePush(LCStoreRef source, LCStoreRef target);
void LCStoreCommit(LCStoreRef store, LCStageRef stage);
void LCStoreHead(LCStoreRef store, char* headShaBuffer);
LCSuccess LCStoreCommitHistory(LCStoreRef store, char* commitsSHABuffer[], size_t start, size_t elements);
size_t LCStoreCommitHistoryLength(LCStoreRef store);
LCSuccess LCStoreDataSHA(LCStoreRef store, char* commit, char* path, char* dataSHABuffer);
size_t LCStoreDataLength(LCStoreRef store, char* sha);
LCSuccess LCStoreData(LCStoreRef store, char* sha, unsigned char dataBuffer[]);
#endif
