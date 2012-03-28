

#ifndef LivelyStore_LCStage_h
#define LivelyStore_LCStage_h

#include "LivelyStoreInternal.h"

typedef LCObjectRef LCStageRef;
extern LCTypeRef LCTypeStage;

LCStageRef LCStageCreate(void);
void LCStageAddEntry(LCStageRef stage, char* path, unsigned char data[], size_t length);
void LCStageDeletePath(LCStageRef stage, char* path);

// private
LCKeyValueRef* LCStagePathsToAdd(LCStageRef stage);
LCArrayRef* LCStagePathsToDelete(LCStageRef stage);
size_t LCStageAddPathsCount(LCStageRef stage);
size_t LCStageDeletePathsCount(LCStageRef stage);

#endif
