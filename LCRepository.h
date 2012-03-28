

#ifndef LivelyStore_LCRepository_h
#define LivelyStore_LCRepository_h

#include "LivelyStoreInternal.h"
#include "LCStage.h"
#include "LCCommit.h"

typedef LCObjectRef LCRepositoryRef;
extern LCTypeRef LCTypeRepository;

LCRepositoryRef LCRepositoryCreate(LCCommitRef head);
void LCRepositoryPull(LCRepositoryRef target, LCRepositoryRef source);
void LCRepositoryPush(LCRepositoryRef source, LCRepositoryRef target);
void LCRepositoryCommit(LCRepositoryRef store, LCStageRef stage);
LCCommitRef LCRepositoryHead(LCRepositoryRef store);
LCDataRef LCRepositoryData(LCRepositoryRef store, LCCommitRef commit, char* path);
void LCRepositoryPersist(LCRepositoryRef repo, LCContextRef context);
#endif
