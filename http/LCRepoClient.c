

#include "LCRepoClient.h"

typedef struct repoClientData* repoClientDataRef;

struct repoClientData {
  pushChanges pushChangesHandler;
  metaDataPull metaDataPullHandler;
  dataPull dataPullHandler;
  void *cookie;
};

struct LCType typeRepoClient = {
  .name = "LCRepoClient",
  .immutable = true,
};

LCTypeRef LCTypeRepoClient = &typeRepoClient;

static repoClientDataRef repoClientData(LCRepoClient client) {
  return objectData(client);
}

static repoClientDataRef repoClientInit() {
  repoClientDataRef data = malloc(sizeof(struct repoClientData));
  return data;
}

LCRepoClient LCRepoClientCreate(void *cookie, pushChanges pushChangesHandler, metaDataPull metaDataPullHandler, 
                                dataPull dataPullHandler) {
  repoClientDataRef data = repoClientInit();
  data->cookie = cookie;
  data->pushChangesHandler = pushChangesHandler;
  data->metaDataPullHandler = metaDataPullHandler;
  data->dataPullHandler = dataPullHandler;
  return objectCreate(LCTypeRepoClient, data);
}

void LCRepoClientPush(LCRepoClient client, LCRemoteRepositoryRef remote, LCRepositoryRef local, char commit[HASH_LENGTH]) {
  repoClientDataRef data = repoClientData(client);
  data->pushChangesHandler(data->cookie, remote, local, commit);
}

void LCRepoClientPullMetaData(LCRepoClient client, LCRemoteRepositoryRef remote, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH]) {
  repoClientDataRef data = repoClientData(client);
  data->metaDataPullHandler(data->cookie, remote, fromCommit, toCommit);
}

void LCRepoClientPullData(LCRepoClient client, LCRemoteRepositoryRef remote, char* dataHashes[], size_t length) {
  repoClientDataRef data = repoClientData(client);
  data->dataPullHandler(data->cookie, remote, dataHashes, length);
}