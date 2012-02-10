
#include "LivelyStore.h"

struct LCSHA {
  LCObjectMeta meta;
  unsigned char sha[];
};

void computeSHA1(LCBlobRef blobs[], size_t count, unsigned char buffer[]);
void LCSHADealloc(void* object);
char hexDigitToCharacter(char hexDigit);
void unsignedCharToHex(unsigned char input, char* buffer);
void unsignedCharArrayToHexString(unsigned char input[], size_t length, char* buffer);

LCSHARef LCSHACreate(LCBlobRef blobs[], size_t count) {
  LCSHARef newSha = malloc(sizeof(struct LCSHA) + LC_SHA1_Length);
  if (newSha != NULL) {
    newSha->meta.dealloc = LCSHADealloc;
    computeSHA1(blobs, count, newSha->sha);
  }
  return newSha;
};

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
void computeSHA1(LCBlobRef blobs[], size_t count, unsigned char buffer[]) {
  SHA_CTX context;
  SHA1_Init(&context);
  for(LCInteger i=0; i<count; i++) {
    SHA1_Update(&context, LCBlobDataRef(blobs[i]), LCBlobLength(blobs[i]));
  }
  SHA1_Final(buffer, &context);
}
#pragma GCC diagnostic warning "-Wdeprecated-declarations"

void LCSHADealloc(void* object) {}

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

LCStringRef LCSHACreateHexString(LCSHARef sha) {
  char shaString[41];
  unsignedCharArrayToHexString(sha->sha, 20, shaString); 
  LCStringRef hexString = LCStringCreate(shaString);
  return hexString;
}

LCBool LCSHAEqual(LCSHARef sha, LCSHARef anotherSHA) {
  for(LCInteger i=0; i<LC_SHA1_Length; i++) {
    if(sha->sha[i] != anotherSHA->sha[i]) {
      return false;
    }
  }
  return true;
}