
#ifndef LivelyStore_LCRepoClient_h
#define LivelyStore_LCRepoClient_h

#include "LivelyStoreInternal.h"
#include "LCRepository.h"
#include "LCRemoteRepository.h"

typedef LCObjectRef LCRepoClient;
extern LCTypeRef LCTypeRepoClient;

typedef void(*metaDataPull)(LCRemoteRepositoryRef remote, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH], LCStoreRef store);
typedef void(*dataPull)(LCRemoteRepositoryRef remote, char* data[], LCStoreRef store);

LCRepoClient LCRepoClientCreate(metaDataPull metaDataPullHandler, dataPull dataPullHandler, LCStoreRef store);
void LCRepoClientSendNewHead(LCRepoClient client, LCRemoteRepositoryRef remote, LCRepositoryRef local, char commit[HASH_LENGTH]);
void LCRepoClientPullMetaData(LCRepoClient client, LCRemoteRepositoryRef remote, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH]);
void LCRepoClientPullData(LCRepoClient client, LCRemoteRepositoryRef remote, char* data[], LCStoreRef store);

#endif
