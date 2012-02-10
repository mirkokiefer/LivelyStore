

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <openssl/sha.h>
#include <string.h>

#ifndef __LC_TYPES__
#define __LC_TYPES__

typedef int LCInteger;
typedef uint8_t LCUInteger8Bit;
typedef struct LCString* LCStringRef;
typedef unsigned char LCByte;

typedef LCStringRef LCKeyRef;
typedef struct LCStore* LCStoreRef;
typedef struct LCCommit* LCCommitRef;
typedef struct LCTree* LCTreeRef;
typedef struct LCKeyValue* LCKeyValueRef;
typedef struct LCBlob* LCBlobRef;
typedef struct LCSHA* LCSHARef;

#define LC_SHA1_Length 20

#endif

#include "LCString.h"

#include "LCStore.h"
#include "LCCommit.h"
#include "LCTree.h"
#include "LCKeyValue.h"
#include "LCBlob.h"
#include "LCSHA.h"