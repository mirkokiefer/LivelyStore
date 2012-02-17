

#ifndef LivelyStore_LCStage_h
#define LivelyStore_LCStage_h

#include "LivelyStore.h"

LCStageRef LCStageCreate();
bool LCStageAddEntry(LCStageRef stage, char* path, unsigned char data[], size_t length);
bool LCStageDeletePath(LCStageRef stage, char* path);
void LCStageCommitToStore(LCStoreRef store);

// private
LCPathValueRef* LCStagePathsToAdd(LCStageRef stage);
LCStringRef* LCStagePathsToDelete(LCStageRef stage);
size_t LCStageAddPathsCount(LCStageRef stage);
size_t LCStageDeletePathsCount(LCStageRef stage);

#endif
