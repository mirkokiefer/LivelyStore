
#ifndef LivelyStore_imac_LCHttpInterface_h
#define LivelyStore_imac_LCHttpInterface_h

#include "LivelyStoreInternal.h"
#include "LCRepository.h"

typedef LCObjectRef LCHttpInterfaceRef;
extern LCTypeRef LCTypeHttpInterface;

LCHttpInterfaceRef LCHttpInterfaceCreate();
void LCHttpInterfaceStart(LCHttpInterfaceRef http, char *port);
void LCHttpInterfaceStop(LCHttpInterfaceRef http);
void LCHttpInterfaceRegisterRepository(LCHttpInterfaceRef http, LCRepositoryRef repo, char *name);
void LCHttpInterfaceSendPullRequest(LCHttpInterfaceRef http, char *remoteRepo, char *localRepoName, char commit[HASH_LENGTH]);
void LCHttpInterfaceLoadData(LCHttpInterfaceRef http, char *remoteRepo, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH], LCStoreRef store);

#endif
