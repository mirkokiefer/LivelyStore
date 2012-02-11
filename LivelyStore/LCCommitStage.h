

#ifndef LivelyStore_LCCommitStage_h
#define LivelyStore_LCCommitStage_h

#include "LivelyStore.h"

LCCommitStageRef LCCommitStageCreate();
LCBool LCCommitStageAddEntry(LCCommitStageRef commit, LCKeyValueRef keyValue);
size_t LCCommitStageEntryCount(LCCommitStageRef commit);
void LCCommitStageEntries(LCCommitStageRef commit, LCKeyValueRef buffer[]);
LCSHARef LCCommitStageSHA1(LCCommitStageRef commit);

void LCCommitToStore(LCStoreRef store);

#endif
