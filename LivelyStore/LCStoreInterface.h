
#ifndef LivelyStore_imac_LCStoreInterface_h
#define LivelyStore_imac_LCStoreInterface_h

#include "LivelyStore.h"

LCStoreInterfaceRef LCStoreInterfaceCreate(struct LCStoreBackend* backend, char headCommit[LC_SHA1_HEX_Length]);
void LCStoreInterfacePull(LCStoreRef target, LCStoreRef source);
void LCStoreInterfacePush(LCStoreRef source, LCStoreRef target);
void LCStoreInterfaceCommit(LCStoreRef store, LCStageRef stage);
void LCStoreInterfaceHead(LCStoreRef store, char* headShaBuffer);
LCSuccess LCStoreInterfaceDataSHA(LCStoreRef store, char* commit, char* path, char dataSHABuffer[LC_SHA1_HEX_Length]);
size_t LCStoreInterfaceDataLength(LCStoreRef store, char sha[LC_SHA1_HEX_Length]);
LCSuccess LCStoreInterfaceData(LCStoreRef store, char sha[LC_SHA1_HEX_Length], unsigned char dataBuffer[]);

#endif
