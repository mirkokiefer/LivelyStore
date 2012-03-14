

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <openssl/sha.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>

#include "LivelyC.h"

#ifndef __LC_TYPES__
#define __LC_TYPES__

typedef struct LCStage* LCStageRef;
typedef struct LCPathData* LCPathDataRef;
typedef struct LCTree* LCTreeRef;
typedef struct LCBackendWrapper* LCBackendWrapperRef;
typedef struct LCCommit* LCCommitRef;
typedef struct LCRepositoryInterface* LCRepositoryInterfaceRef;

typedef enum {
  LCData,
  LCTree,
  LCCommit
} LCDataType;

typedef enum {
  LCSuccessTrue,
  LCSuccessFalse
} LCSuccess;

#endif

#include "LCRepository.h"
#include "LCStage.h"
#include "LCTree.h"
#include "LCBackendWrapper.h"
#include "LCCommit.h"
#include "LCMemoryStoreBackend.h"
#include "LCFileStoreBackend.h"
#include "LCRepositoryInterface.h"
#include "LCMemoryStream.h"