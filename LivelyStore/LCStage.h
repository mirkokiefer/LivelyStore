

#ifndef LivelyStore_LCStage_h
#define LivelyStore_LCStage_h

#include "LivelyStore.h"

LCStageRef LCStageCreate();
bool LCStageAddEntry(LCStageRef stage, char* key, unsigned char data[], size_t length);
void LCStageCommitToStore(LCStoreRef store);

// private
LCKeyValueRef* LCStageEntries(LCStageRef stage);
#endif
