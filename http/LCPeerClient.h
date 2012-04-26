
#ifndef LivelyStore_LCPeerClient_h
#define LivelyStore_LCPeerClient_h

#include "LivelyStoreInternal.h"
#include "LCRepository.h"

typedef LCObjectRef LCPeerClient;
extern LCTypeRef LCTypePeerClient;

LCPeerClient LCPeerClientCreate(char *address, char *login, char *password, LCContextRef context);
void LCPeerClientPush(LCPeerClient client, LCCommitRef commit);
void LCPeerClientPullMetaData(LCPeerClient client, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH]);
void LCPeerClientPullData(LCPeerClient client, char* dataHashs[], size_t length);

void LCHttpGlobalInit();
void LCHttpGlobalCleanup();
#endif
