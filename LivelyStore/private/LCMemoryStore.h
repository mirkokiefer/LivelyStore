
#ifndef LivelyStore_imac_LCMemoryStore_h
#define LivelyStore_imac_LCMemoryStore_h

#include "LivelyStore.h"

struct LCStoreBackend* createLCMemoryStoreBackend(char* location);
void freeLCMemoryStore(struct LCStoreBackend* backend);

#endif
