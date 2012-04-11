
#ifndef LivelyStore_LCPeerClient_h
#define LivelyStore_LCPeerClient_h

#include "LivelyStoreInternal.h"
#include "LCRepository.h"
#include "LCRemoteRepository.h"

typedef LCObjectRef LCPeerClient;
extern LCTypeRef LCTypePeerClient;

LCPeerClient LCPeerClientCreate(char *address, char *login, char *password, LCStoreRef store);
void LCPeerClientPush(LCPeerClient client, LCCommitRef commit);
LCCommitRef LCPeerClientPullMetaData(LCPeerClient client, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH]);
void LCPeerClientPullData(LCPeerClient client, char* dataHashs[], size_t length);

#endif
