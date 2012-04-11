
#ifndef LivelyStore_LCRepoClient_h
#define LivelyStore_LCRepoClient_h

#include "LivelyStoreInternal.h"
#include "LCRepository.h"
#include "LCRemoteRepository.h"

typedef LCObjectRef LCRepoClient;
extern LCTypeRef LCTypeRepoClient;

typedef void(*pushChanges)(LCRemoteRepositoryRef remote, LCCommitRef commit);
typedef LCCommitRef(*metaDataPull)(LCStoreRef store, LCRemoteRepositoryRef remote, char fromCommit[HASH_LENGTH],
                                   char toCommit[HASH_LENGTH]);
typedef void(*dataPull)(LCStoreRef store, LCRemoteRepositoryRef remote, char* data[], size_t length);

LCRepoClient LCRepoClientCreate(LCStoreRef store, pushChanges pushChangesHandler, metaDataPull metaDataPullHandler, 
                                dataPull dataPullHandler);
void LCRepoClientPush(LCRepoClient client, LCRemoteRepositoryRef remote, LCCommitRef commit);
LCCommitRef LCRepoClientPullMetaData(LCRepoClient client, LCRemoteRepositoryRef remote, char fromCommit[HASH_LENGTH],
                                     char toCommit[HASH_LENGTH]);
void LCRepoClientPullData(LCRepoClient client, LCRemoteRepositoryRef remote, char* dataHashs[], size_t length);

LCRepoClient LCRepoClientCreateToPeer(char *baseURL, LCStoreRef store);
LCRepoClient LCRepoClientCreateToAWS(LCStoreRef store);
#endif
