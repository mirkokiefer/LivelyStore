

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
typedef bool LCBool;

typedef LCStringRef LCKeyRef;
typedef struct LCStore* LCStoreRef;
typedef struct LCCommitStage* LCCommitStageRef;
typedef struct LCTree* LCTreeRef;
typedef struct LCKeyValue* LCKeyValueRef;
typedef struct LCBlob* LCBlobRef;
typedef struct LCSHA* LCSHARef;

struct LCObjectMeta {
  LCInteger rCount;
  void (*dealloc)(void* object);;
};

typedef struct LCObjectMeta LCObjectMeta;

#define LC_SHA1_Length 20

#endif

#include "LCString.h"

#include "LCStore.h"
#include "LCCommitStage.h"
#include "LCTree.h"
#include "LCKeyValue.h"
#include "LCBlob.h"
#include "LCSHA.h"
#include "LCUtils.h"