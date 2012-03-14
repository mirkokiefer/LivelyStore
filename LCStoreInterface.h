
#ifndef LivelyStore_imac_LCRepositoryInterface_h
#define LivelyStore_imac_LCRepositoryInterface_h

#include "LivelyStore.h"

LCRepositoryInterfaceRef LCRepositoryInterfaceCreate(struct LCRepositoryBackend* backend, char headCommit[LC_SHA1_HEX_Length]);
void LCRepositoryInterfacePull(LCRepositoryRef target, LCRepositoryRef source);
void LCRepositoryInterfacePush(LCRepositoryRef source, LCRepositoryRef target);
void LCRepositoryInterfaceCommit(LCRepositoryRef store, LCStageRef stage);
void LCRepositoryInterfaceHead(LCRepositoryRef store, char* headShaBuffer);
LCSuccess LCRepositoryInterfaceDataSHA(LCRepositoryRef store, char* commit, char* path, char dataSHABuffer[LC_SHA1_HEX_Length]);
size_t LCRepositoryInterfaceDataLength(LCRepositoryRef store, char sha[LC_SHA1_HEX_Length]);
LCSuccess LCRepositoryInterfaceData(LCRepositoryRef store, char sha[LC_SHA1_HEX_Length], unsigned char dataBuffer[]);

#endif
