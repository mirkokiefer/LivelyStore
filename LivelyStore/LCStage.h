

#ifndef LivelyStore_LCStage_h
#define LivelyStore_LCStage_h

#include "LivelyStore.h"

LCStageRef LCStageCreate();
LCBool LCStageAddEntry(LCStageRef commit, LCKeyValueRef keyValue);
size_t LCStageEntryCount(LCStageRef commit);
void LCStageEntries(LCStageRef commit, LCKeyValueRef buffer[]);
LCSHARef LCStageSHA1(LCStageRef commit);

void LCCommitToStore(LCStoreRef store);

#endif
