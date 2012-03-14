
#ifndef LivelyStore_imac_LCMemoryStore_h
#define LivelyStore_imac_LCMemoryStore_h

#include "LivelyStore.h"

struct LCRepositoryBackend* createLCMemoryStoreBackend(char* location);
void freeLCMemoryStoreBackend(struct LCRepositoryBackend* backend);

#endif
