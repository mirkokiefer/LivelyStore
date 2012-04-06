

#ifndef LivelyStore_LCRepository_h
#define LivelyStore_LCRepository_h

#include "LivelyStoreInternal.h"
#include "LCStage.h"
#include "LCCommit.h"

typedef LCObjectRef LCRepositoryRef;
extern LCTypeRef LCTypeRepository;

typedef struct commit_conflict_s* commit_conflict_t;

typedef LCArrayRef(*createResolvedData)(void *cookie, LCStringRef path, LCObjectRef localData, LCObjectRef foreignData);

LCRepositoryRef LCRepositoryCreate(LCCommitRef head);
void LCRepositoryPull(LCRepositoryRef target, LCRepositoryRef source);
void LCRepositoryPush(LCRepositoryRef source, LCRepositoryRef target);
void LCRepositoryCommit(LCRepositoryRef store, LCStageRef stage);
LCCommitRef LCRepositoryHead(LCRepositoryRef store);
LCObjectRef LCRepositoryData(LCRepositoryRef store, LCCommitRef commit, char* path);
void LCRepositoryPersist(LCRepositoryRef repo, LCContextRef context);
void LCRepositoryDeleteCache(LCRepositoryRef repo, LCContextRef storedContext);
void LCRepositoryMerge(LCRepositoryRef localRepo, LCRepositoryRef foreignRepo, void *cookie, createResolvedData conflictStrategy);

LCContextRef createRepositoryContext(LCStoreRef store);

//conflict strategies
LCArrayRef conflictStrategyKeepLocal(void *cookie, LCStringRef path, LCObjectRef localData, LCObjectRef foreignData);
LCArrayRef conflictStrategyKeepForeign(void *cookie, LCStringRef path, LCObjectRef localData, LCObjectRef foreignData);
LCArrayRef conflictStrategyKeepBoth(void *cookie, LCStringRef path, LCObjectRef localData, LCObjectRef foreignData);
#endif
