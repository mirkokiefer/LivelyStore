

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

typedef LCStringRef LCPathRef;
typedef struct LCStore* LCStoreRef;
typedef struct LCStage* LCStageRef;
typedef struct LCPathData* LCPathDataRef;
typedef struct LCData* LCDataRef;
typedef struct LCKeyValue* LCKeyValueRef;

typedef struct LCTree* LCTreeRef;
typedef struct LCDataStore* LCDataStoreRef;
typedef struct LCCommit* LCCommitRef;

// internal types
typedef struct LCType LCType;
typedef struct LCType* LCTypeRef;
typedef struct LCObjectInfo LCObjectInfo;
typedef struct LCObjectInfo* LCObjectInfoRef;
typedef struct LCHashableObject LCHashableObject;
typedef struct LCHashableObject* LCHashableObjectRef;

struct LCType {
  void (*dealloc)(void* object);
  void* meta;
};

struct LCObjectInfo {
  LCInteger rCount;
  LCTypeRef type;
};

#define LC_SHA1_Length 20
#define LC_SHA1_HEX_Length 40

#endif

#include "LCString.h"
#include "LCKeyValue.h"

#include "LCStore.h"
#include "LCStage.h"
#include "LCTree.h"
#include "LCData.h"
#include "LCSHA.h"
#include "LCUtils.h"
#include "LCDataStore.h"
#include "LCCommit.h"