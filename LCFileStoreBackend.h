
#ifndef LivelyStore_imac_LCFileStore_h
#define LivelyStore_imac_LCFileStore_h

#include "LivelyStore.h"

struct LCRepositoryBackend* createLCFileStoreBackend(char* location);
void freeLCFileStoreBackend(struct LCRepositoryBackend* backend);

#endif
