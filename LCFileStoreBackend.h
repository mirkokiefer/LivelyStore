
#ifndef LivelyStore_imac_LCFileStore_h
#define LivelyStore_imac_LCFileStore_h

#include "LivelyStore.h"

struct LCStoreBackend* createLCFileStoreBackend(char* location);
void freeLCFileStoreBackend(struct LCStoreBackend* backend);

#endif
