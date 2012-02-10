
#include "LivelyStore.h"

struct LCSHA {
  LCInteger rCount;
  unsigned char sha[];
};

static inline void LCSHADealloc(LCSHARef aStruct);
char hexDigitToCharacter(char hexDigit);
void unsignedCharToHex(unsigned char input, char* buffer);
void unsignedCharArrayToHexString(unsigned char input[], size_t length, char* buffer);

LCSHARef LCSHACreate(LCBlobRef blobs[], size_t count) {
  LCSHARef newSha = malloc(sizeof(struct LCSHA) + LC_SHA1_Length);
  if (newSha != NULL) {
    SHA_CTX context;
    SHA1_Init(&context);
    for(LCInteger i=0; i<count; i++) {
      SHA1_Update(&context, LCBlobDataRef(blobs[i]), LCBlobLength(blobs[i]));
    }
    SHA1_Final(newSha->sha, &context);
  }
  return newSha;
};

void LCSHARetain(LCSHARef aStruct) {
  aStruct->rCount = aStruct->rCount + 1;
}

void LCSHARelease(LCSHARef aStruct) {
  aStruct->rCount = aStruct->rCount - 1;
  if(aStruct->rCount == 0) {
    LCSHADealloc(aStruct);
  }
}

static inline void LCSHADealloc(LCSHARef aStruct) {
  free(aStruct);
}

char hexDigitToCharacter(char hexDigit) {
  if(hexDigit > 9) {
    hexDigit = hexDigit - 10 + 97; //97 is A
  } else {
    hexDigit = hexDigit + 48; //48 is 0
  }
  return hexDigit;
}

void unsignedCharToHex(unsigned char input, char* buffer) {
  buffer[0] = hexDigitToCharacter(input/16);
  buffer[1] = hexDigitToCharacter(input%16);
}

void unsignedCharArrayToHexString(unsigned char input[], size_t length, char* buffer) {
  for(LCInteger i=0; i<length; i++) {
    unsignedCharToHex(input[i], &buffer[i*2]);
  }
  buffer[length*2] = '\0';
}

LCStringRef LCSHAHexString(LCSHARef sha) {
  char shaString[41];
  unsignedCharArrayToHexString(sha->sha, 20, shaString); 
  LCStringRef hexString = LCStringCreate(shaString);
  return hexString;
}