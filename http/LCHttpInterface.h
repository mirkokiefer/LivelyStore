
#ifndef LivelyStore_imac_LCHttpInterface_h
#define LivelyStore_imac_LCHttpInterface_h

#include "LivelyStoreInternal.h"
#include "LCRepository.h"
#include "LCRemoteRepository.h"

typedef LCObjectRef LCHttpInterfaceRef;
extern LCTypeRef LCTypeHttpInterface;

typedef bool(*authorizeFun)(LCRepositoryRef repo, char *repoName, char *login, char *password);

LCHttpInterfaceRef LCHttpInterfaceCreate(authorizeFun authorizationHandler);
void LCHttpInterfaceStart(LCHttpInterfaceRef http, char *port);
void LCHttpInterfaceStop(LCHttpInterfaceRef http);
void LCHttpInterfaceRegisterRepository(LCHttpInterfaceRef http, LCRepositoryRef repo, char *name);
void LCHttpInterfaceSendNewHead(LCHttpInterfaceRef http, LCRemoteRepositoryRef remote, LCRepositoryRef localRepo, char commit[HASH_LENGTH]);
void LCHttpInterfacePullCommitList(LCHttpInterfaceRef http, LCRemoteRepositoryRef remote, char fromCommit[HASH_LENGTH],
                                char toCommit[HASH_LENGTH], LCStoreRef store);
void LCHttpInterfacePullCommits(LCHttpInterfaceRef http, LCRemoteRepositoryRef remote, char* commits[], size_t length, LCStoreRef store);
void LCHttpInterfacePullTrees(LCHttpInterfaceRef http, LCRemoteRepositoryRef remote, char* trees[], size_t length, LCStoreRef store);
void LCHttpInterfacePullData(LCHttpInterfaceRef http, LCRemoteRepositoryRef remote, char* data[], LCStoreRef store);

#endif
