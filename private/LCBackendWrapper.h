
#ifndef LivelyStore_LCBackendWrapper_h
#define LivelyStore_LCBackendWrapper_h

#include "LivelyStore.h"

LCBackendWrapperRef LCBackendWrapperCreate(struct LCRepositoryBackend* backend);
void LCBackendWrapperPutData(LCBackendWrapperRef store, LCStringRef sha, LCDataRef data);
void LCBackendWrapperDeleteData(LCBackendWrapperRef store, LCStringRef sha);
void LCBackendWrapperPutTreeData(LCBackendWrapperRef store, LCStringRef sha, LCStringRef data);
void LCBackendWrapperPutCommitData(LCBackendWrapperRef store, LCStringRef sha, LCStringRef data);
void LCBackendWrapperPutTagData(LCBackendWrapperRef store, LCStringRef tag, LCStringRef data);
size_t LCBackendWrapperGetDataLength(LCBackendWrapperRef store, char* sha);
LCDataRef LCBackendWrapperGetData(LCBackendWrapperRef store, LCStringRef sha);
LCStringRef LCBackendWrapperGetTreeData(LCBackendWrapperRef store, LCStringRef sha);
LCStringRef LCBackendWrapperGetCommitData(LCBackendWrapperRef store, LCStringRef sha);

#endif
