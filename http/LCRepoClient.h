
#ifndef LivelyStore_LCRepoClient_h
#define LivelyStore_LCRepoClient_h

#include "LivelyStoreInternal.h"
#include "LCRepository.h"
#include "LCRemoteRepository.h"

typedef LCObjectRef LCRepoClient;
extern LCTypeRef LCTypeRepoClient;

typedef void(*pushChanges)(void *cookie, LCRemoteRepositoryRef remote, LCRepositoryRef local, char commit[HASH_LENGTH]);
typedef void(*metaDataPull)(void *cookie, LCRemoteRepositoryRef remote, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH]);
typedef void(*dataPull)(void *cookie, LCRemoteRepositoryRef remote, char* data[], size_t length);

LCRepoClient LCRepoClientCreate(void *cookie, pushChanges pushChangesHandler, metaDataPull metaDataPullHandler, 
                                dataPull dataPullHandler);
void LCRepoClientPush(LCRepoClient client, LCRemoteRepositoryRef remote, LCRepositoryRef local, char commit[HASH_LENGTH]);
void LCRepoClientPullMetaData(LCRepoClient client, LCRemoteRepositoryRef remote, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH]);
void LCRepoClientPullData(LCRepoClient client, LCRemoteRepositoryRef remote, char* data[], size_t length);

#endif
