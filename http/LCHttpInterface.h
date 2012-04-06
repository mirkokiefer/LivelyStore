
#ifndef LivelyStore_imac_LCHttpInterface_h
#define LivelyStore_imac_LCHttpInterface_h

#include "LivelyStoreInternal.h"
#include "LCRepository.h"

typedef LCObjectRef LCHttpInterfaceRef;
extern LCTypeRef LCTypeHttpInterface;

LCHttpInterfaceRef LCHttpInterfaceCreate();
void LCHttpInterfaceStart(LCInteger port);
void LCHttpInterfaceStop();
void LCHttpInterfaceRegisterRepository(LCRepositoryRef repo, char *name);
void LCHttpInterfaceSendPullRequest(char *remoteRepo, char *localRepoName, char commit[HASH_LENGTH]);
void LCHttpInterfaceLoadData(char *remoteRepo, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH], LCStoreRef store);

#endif
