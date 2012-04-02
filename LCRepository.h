

#ifndef LivelyStore_LCRepository_h
#define LivelyStore_LCRepository_h

#include "LivelyStoreInternal.h"
#include "LCStage.h"
#include "LCCommit.h"

typedef LCObjectRef LCRepositoryRef;
extern LCTypeRef LCTypeRepository;

struct commit_conflict_s {
  char *conflictPaths;
  LCObjectRef localData;
  LCObjectRef foreignData;
  LCObjectRef resolvedDataBuffer;
};

typedef struct commit_conflict_s* commit_conflict_t;

typedef void(*resolveConflict)(void *cookie, commit_conflict_t conflicts[], size_t length);

LCRepositoryRef LCRepositoryCreate(LCCommitRef head);
void LCRepositoryPull(LCRepositoryRef target, LCRepositoryRef source);
void LCRepositoryPush(LCRepositoryRef source, LCRepositoryRef target);
void LCRepositoryCommit(LCRepositoryRef store, LCStageRef stage);
LCCommitRef LCRepositoryHead(LCRepositoryRef store);
LCDataRef LCRepositoryData(LCRepositoryRef store, LCCommitRef commit, char* path);
void LCRepositoryPersist(LCRepositoryRef repo, LCContextRef context);
void LCRepositoryDeleteCache(LCRepositoryRef repo, LCContextRef context);
int LCRepositoryMerge(LCRepositoryRef localRepo, LCRepositoryRef remoteRepo, resolveConflict conflictStrategy);

LCContextRef createRepositoryContext(LCStoreRef store);
#endif
