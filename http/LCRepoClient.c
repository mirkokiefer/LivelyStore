

#include "LCRepoClient.h"

typedef struct repoClientData* repoClientDataRef;

struct repoClientData {
  pushChanges pushChangesHandler;
  metaDataPull metaDataPullHandler;
  dataPull dataPullHandler;
  LCStoreRef store;
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

LCRepoClient LCRepoClientCreate(LCStoreRef store, pushChanges pushChangesHandler, metaDataPull metaDataPullHandler, 
                                dataPull dataPullHandler) {
  repoClientDataRef data = repoClientInit();
  data->store = store;
  data->pushChangesHandler = pushChangesHandler;
  data->metaDataPullHandler = metaDataPullHandler;
  data->dataPullHandler = dataPullHandler;
  return objectCreate(LCTypeRepoClient, data);
}

void LCRepoClientPush(LCRepoClient client, LCRemoteRepositoryRef remote, LCCommitRef commit) {
  repoClientDataRef data = repoClientData(client);
  data->pushChangesHandler(remote, commit);
}

LCCommitRef LCRepoClientPullMetaData(LCRepoClient client, LCRemoteRepositoryRef remote, char fromCommit[HASH_LENGTH],
                                     char toCommit[HASH_LENGTH]) {
  repoClientDataRef data = repoClientData(client);
  return data->metaDataPullHandler(data->store, remote, fromCommit, toCommit);
}

void LCRepoClientPullData(LCRepoClient client, LCRemoteRepositoryRef remote, char* dataHashes[], size_t length) {
  repoClientDataRef data = repoClientData(client);
  data->dataPullHandler(data->store, remote, dataHashes, length);
}