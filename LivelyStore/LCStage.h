

#ifndef LivelyStore_LCStage_h
#define LivelyStore_LCStage_h

#include "LivelyStore.h"

LCStageRef LCStageCreate();
bool LCStageAddEntry(LCStageRef stage, char* key, unsigned char data[], size_t length);
bool LCStageDeleteKey(LCStageRef stage, char* key);
void LCStageCommitToStore(LCStoreRef store);

// private
LCKeyValueRef* LCStageKeysToAdd(LCStageRef stage);
LCStringRef* LCStageKeysToDelete(LCStageRef stage);
size_t LCStageAddKeysCount(LCStageRef stage);
size_t LCStageDeleteKeysCount(LCStageRef stage);

#endif
