

#ifndef LivelyStore_LCStage_h
#define LivelyStore_LCStage_h

#include "LivelyStoreInternal.h"

typedef LCObjectRef LCStageRef;
extern LCTypeRef LCTypeStage;

LCStageRef LCStageCreate(void);
void LCStageAddEntry(LCStageRef stage, char* path, LCObjectRef data);
void LCStageDeletePath(LCStageRef stage, char* path);

// private
void LCStageAddKeyValues(LCStageRef stage, LCKeyValueRef keyValues[], size_t length);
LCKeyValueRef* LCStageUpdates(LCStageRef stage);
LCArrayRef* LCStagePathsToDelete(LCStageRef stage);
size_t LCStageUpdatesLength(LCStageRef stage);

#endif
