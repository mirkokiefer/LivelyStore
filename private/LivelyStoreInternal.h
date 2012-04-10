

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

#define LC_MAX_URL_LENGTH 1024

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
