

#include "LCPeerClient.h"

typedef struct peerClientData* peerClientDataRef;

struct peerClientData {
  LCStringRef address;
  LCStringRef login;
  LCStringRef password;
  LCStoreRef store;
};

struct LCType typePeerClient = {
  .name = "LCPeerClient",
  .immutable = true,
};

LCTypeRef LCTypePeerClient = &typePeerClient;

static peerClientDataRef peerClientData(LCPeerClient client) {
  return objectData(client);
}

static peerClientDataRef peerClientInit() {
  peerClientDataRef data = malloc(sizeof(struct peerClientData));
  return data;
}

LCPeerClient LCPeerClientCreate(char *address, char *login, char *password, LCStoreRef store) {
  peerClientDataRef data = peerClientInit();
  data->address = LCStringCreate(address);
  data->login = LCStringCreate(login);
  data->password = LCStringCreate(password);
  data->store = store;
  return objectCreate(LCTypePeerClient, data);
}

void LCPeerClientPush(LCPeerClient client, LCCommitRef commit) {
  
}

LCCommitRef LCPeerClientPullMetaData(LCPeerClient client, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH]);
void LCPeerClientPullData(LCPeerClient client, char* dataHashs[], size_t length);
