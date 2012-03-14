

#ifndef LivelyStore_LCRepository_h
#define LivelyStore_LCRepository_h

#include "LivelyStore.h"

LCRepositoryRef LCRepositoryCreate(struct LCRepositoryBackend* backend, LCStringRef headSHA);
void LCRepositoryPull(LCRepositoryRef target, LCRepositoryRef source);
void LCRepositoryPush(LCRepositoryRef source, LCRepositoryRef target);
void LCRepositoryCommit(LCRepositoryRef store, LCStageRef stage);
LCCommitRef LCRepositoryHead(LCRepositoryRef store);
LCStringRef LCRepositoryDataSHA(LCRepositoryRef store, LCCommitRef commit, char* path);
LCDataRef LCRepositoryData(LCRepositoryRef store, LCStringRef dataSHA);
#endif
