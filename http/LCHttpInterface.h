
#ifndef LivelyStore_imac_LCHttpInterface_h
#define LivelyStore_imac_LCHttpInterface_h

#include "LivelyStoreInternal.h"
#include "LCRepository.h"

typedef LCObjectRef LCHttpInterfaceRef;
extern LCTypeRef LCTypeHttpInterface;

typedef bool(*authorizeFun)(char *remoteRepo, char *localRepoName, LCRepositoryRef localRepo, char *login, char *password);

LCHttpInterfaceRef LCHttpInterfaceCreate(authorizeFun authorizationHandler);
void LCHttpInterfaceStart(LCHttpInterfaceRef http, char *port);
void LCHttpInterfaceStop(LCHttpInterfaceRef http);
void LCHttpInterfaceRegisterRepository(LCHttpInterfaceRef http, LCRepositoryRef repo, char *name);
void LCHttpInterfaceSendPullRequest(LCHttpInterfaceRef http, char *remoteRepo, LCRepositoryRef localRepo,
                                    char commit[HASH_LENGTH], char *message);
void LCHttpInterfacePullData(LCHttpInterfaceRef http, char *remoteRepo, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH], LCStoreRef store);

#endif
