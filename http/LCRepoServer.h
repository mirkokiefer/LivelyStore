
#ifndef LivelyStore_imac_LCRepoServer_h
#define LivelyStore_imac_LCRepoServer_h

#include "LivelyStoreInternal.h"
#include "LCRepository.h"

typedef LCObjectRef LCRepoServer;
extern LCTypeRef LCTypeRepoServer;

typedef bool(*authorizeFun)(LCRepositoryRef repo, char *repoName, char *login, char *password);

LCRepoServer LCRepoServerCreate(authorizeFun authorizationHandler);
void LCRepoServerStart(LCRepoServer http, char *port);
void LCRepoServerStop(LCRepoServer http);
void LCRepoServerRegisterRepository(LCRepoServer http, LCRepositoryRef repo, char *name);

#endif
