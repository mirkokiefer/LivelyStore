

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
typedef struct LCKeyValue* LCKeyValueRef;
typedef struct LCBlob* LCBlobRef;
typedef struct LCBlobArray* LCBlobArrayRef;
typedef struct LCSHA* LCSHARef;

typedef struct LCTree* LCTreeRef;
typedef struct LCBlobStore* LCBlobStoreRef;
typedef struct LCKeyValueSHA* LCKeyValueSHARef;

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

struct LCHashableObject {
  void (*blobs)(void* object, LCBlobRef buffer[]);
  size_t (*blobCount)(void* object);
};


#define LC_SHA1_Length 20

#endif

#include "LCString.h"

#include "LCStore.h"
#include "LCCommitStage.h"
#include "LCTree.h"
#include "LCKeyValue.h"
#include "LCBlob.h"
#include "LCBlobArray.h"
#include "LCSHA.h"
#include "LCUtils.h"
#include "LCBlobStore.h"
#include "LCKeyValueSHA.h"