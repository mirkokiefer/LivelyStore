
#ifndef LivelyStore_imac_LCRemoteRepository_h
#define LivelyStore_imac_LCRemoteRepository_h

#include "LivelyStoreInternal.h"

typedef LCObjectRef LCRemoteRepositoryRef;
extern LCTypeRef LCTypeRemoteRepository;

LCRemoteRepositoryRef LCRemoteRepositoryCreate(char *url, char *login, char *password);
char* LCRemoteRepositoryUrl(LCRemoteRepositoryRef repo);
char* LCRemoteRepositoryLogin(LCRemoteRepositoryRef repo);
char* LCRemoteRepositoryPassword(LCRemoteRepositoryRef repo);
int LCRemoteRepositoryURLForPath(LCRemoteRepositoryRef repo, char *path, char buffer[], size_t length);
#endif
