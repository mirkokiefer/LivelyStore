
#ifndef LivelyStore_imac_LCMemoryStore_h
#define LivelyStore_imac_LCMemoryStore_h

#include "LivelyStore.h"

LCMemoryStoreRef LCMemoryStoreCreate(char* location);
void LCMemoryStoreRegister(LCMemoryStoreRef store, LCStoreRef storeInterface);

#endif
