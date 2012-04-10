

#include "LCRepoClient.h"

typedef struct repoClientData* repoClientDataRef;

struct repoClientData {
  metaDataPull metaDataPullHandler;
  dataPull dataPullHandler;
  LCStoreRef store;
};

struct LCType typeRepoClient = {
  .name = "LCRepoClient",
  .immutable = true,
};

LCTypeRef LCTypeRepoClient = &typeRepoClient;

static repoClientDataRef repoClientData(LCRepoClient http) {
  return objectData(http);
}

static repoClientDataRef repoClientInit() {
  repoClientDataRef data = malloc(sizeof(struct repoClientData));
  return data;
}

LCRepoClient LCRepoClientCreate(metaDataPull metaDataPullHandler, dataPull dataPullHandler, LCStoreRef store) {
  repoClientDataRef data = repoClientInit();
  data->metaDataPullHandler = metaDataPullHandler;
  data->dataPullHandler = dataPullHandler;
  data->store = store;
  return objectCreate(LCTypeRepoClient, data);
}

void LCRepoClientSendNewHead(LCRepoClient client, LCRemoteRepositoryRef remote, LCRepositoryRef local, char commit[HASH_LENGTH]);
void LCRepoClientPullMetaData(LCRepoClient client, LCRemoteRepositoryRef remote, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH]);
void LCRepoClientPullData(LCRepoClient client, LCRemoteRepositoryRef remote, char* data[], LCStoreRef store);

