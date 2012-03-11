
#ifndef LivelyStore_imac_LCMemoryStream_h
#define LivelyStore_imac_LCMemoryStream_h

#include "LivelyStore.h"

typedef struct LCMemoryStream* LCMemoryStreamRef;

LCMemoryStreamRef LCMemoryStreamCreate();
FILE* LCMemoryStreamFile(LCMemoryStreamRef streamObj);
size_t LCMemoryStreamLength(LCMemoryStreamRef streamObj);
char* LCMemoryStreamData(LCMemoryStreamRef streamObj);

#endif
