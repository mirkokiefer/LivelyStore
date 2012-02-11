
#include "LivelyStore.h"

struct LCSHA {
  LCObjectInfo info;
  LCBlobRef sha;
  void* cachedValue;
};

void LCSHADealloc(void* object);

LCType typeSHA = {
  .dealloc = LCSHADealloc
};

void computeSHA1(LCBlobRef blobs[], size_t count, LCByte buffer[]);
char hexDigitToASCIChar(char hexDigit);
char asciCharToHexDigit(char hexDigit);
void byteToHexDigits(LCByte input, char* buffer);
LCByte hexDigitsToByte(char* hexDigits);
LCStringRef createHexStringFromBlob(LCBlobRef blob);
LCBlobRef createBlobFromHexString(LCStringRef hexString);

LCSHARef LCSHACreate(LCBlobRef blobs[], size_t count) {
  LCSHARef newSha = malloc(sizeof(struct LCSHA) + LC_SHA1_Length);
  if (newSha != NULL) {
    newSha->info.type = &typeSHA;
    LCByte sha[LC_SHA1_Length];
    computeSHA1(blobs, count, sha);
    newSha->sha = LCBlobCreate(sha, LC_SHA1_Length);
  }
  return newSha;
};

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
void computeSHA1(LCBlobRef blobs[], size_t count, LCByte buffer[]) {
  SHA_CTX context;
  SHA1_Init(&context);
  for(LCInteger i=0; i<count; i++) {
    SHA1_Update(&context, LCBlobDataRef(blobs[i]), LCBlobLength(blobs[i]));
  }
  SHA1_Final(buffer, &context);
}
#pragma GCC diagnostic warning "-Wdeprecated-declarations"

LCSHARef LCSHACreateFromHexString(LCStringRef hexString) {
  LCSHARef newSha = malloc(sizeof(struct LCSHA) + LC_SHA1_Length);
  if (newSha != NULL) {
    newSha->info.type = &typeSHA;
    newSha->sha = createBlobFromHexString(hexString);
  }
  return newSha;
}

LCStringRef LCSHACreateHexString(LCSHARef sha) {
  return createHexStringFromBlob(sha->sha);
}

LCBlobRef LCSHABlob(LCSHARef sha) {
  return sha->sha;
}

LCBool LCSHAEqual(LCSHARef sha, LCSHARef anotherSHA) {
  LCByte* sha1Bytes = LCBlobDataRef(sha->sha);
  LCByte* sha2Bytes = LCBlobDataRef(anotherSHA->sha);
  for(LCInteger i=0; i<LC_SHA1_Length; i++) {
    if(sha1Bytes[i] != sha2Bytes[i]) {
      return false;
    }
  }
  return true;
}

void LCSHADealloc(void* object) {
  LCRelease(((LCSHARef)object)->sha);
}

void* LCSHAStoreLookup(LCSHARef sha, LCBlobStoreRef blobStore) {
  if(sha->cachedValue == NULL) {
    sha->cachedValue = LCBlobStoreGet(blobStore, sha);    
  }
  return sha->cachedValue;
}

// utility functions
char hexDigitToASCIChar(char hexDigit) {
  if(hexDigit > 9) {
    hexDigit = hexDigit - 10 + 97; //97 is A
  } else {
    hexDigit = hexDigit + 48; //48 is 0
  }
  return hexDigit;
}

char asciCharToHexDigit(char asciChar) {
  if (asciChar >= 97) { //A = 97
    return asciChar - 97 + 10; // A -> 10, B -> 11...
  } else {
    return asciChar - 48; // if a number char (48=char 0)
  }
}

void byteToHexDigits(LCByte input, char* buffer) {
  buffer[0] = hexDigitToASCIChar(input/16);
  buffer[1] = hexDigitToASCIChar(input%16);
}

LCByte hexDigitsToByte(char* hexDigits) {
  return hexDigits[0]*16 + hexDigits[1];
}

LCStringRef createHexStringFromBlob(LCBlobRef blob) {
  LCByte* input = LCBlobDataRef(blob);
  size_t length = LCBlobLength(blob);
  char buffer[length*2];
  for(LCInteger i=0; i<length; i++) {
    byteToHexDigits(input[i], &buffer[i*2]);
  }
  buffer[length*2] = '\0';
  return LCStringCreate(buffer);
}

LCBlobRef createBlobFromHexString(LCStringRef hexString) {
  LCByte buffer[LC_SHA1_Length];
  for (LCInteger i=0; i<LC_SHA1_Length; i++) {
    char digit1 = LCStringCharAtIndex(hexString, i*2);
    char digit2 = LCStringCharAtIndex(hexString, (i*2)+1);
    char hexDigits[] = {asciCharToHexDigit(digit1), asciCharToHexDigit(digit2)};
    buffer[i] = hexDigitsToByte(hexDigits);
  }
  return LCBlobCreate(buffer, LC_SHA1_Length);
}
