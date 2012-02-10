
#include "LivelyStore.h"

struct LCBlob {
  LCInteger rCount;
  size_t size;
  void* data;
};

static inline void LCBlobDealloc(LCBlobRef aStruct);

LCBlobRef LCBlobCreate(void* data, size_t size) {
  LCBlobRef newBlob = malloc(sizeof(struct LCBlob));
  if (newBlob != NULL) {
    void* dataMemory = malloc(size);
    if(dataMemory != NULL) {
      newBlob->size = size;
      memcpy(dataMemory, data, size);
      newBlob->data = dataMemory;
    } else {
      free(newBlob);
      return NULL;
    }
  }
  return newBlob;
};

void LCBlobRetain(LCBlobRef aStruct) {
  aStruct->rCount = aStruct->rCount + 1;
}

void LCBlobRelease(LCBlobRef aStruct) {
  aStruct->rCount = aStruct->rCount - 1;
  if(aStruct->rCount == 0) {
    LCBlobDealloc(aStruct);
  }
}

static inline void LCBlobDealloc(LCBlobRef aStruct) {
  free(aStruct);
}

size_t LCBlobSize(LCBlobRef blob) {
  return blob->size;
}

void LCBlobData(LCBlobRef blob, void* buffer) {
  memcpy(buffer, blob->data, blob->size);
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

void unsignedCharsToHex(unsigned char input[], size_t length, char* buffer) {
  for(LCInteger i=0; i<length; i++) {
    unsignedCharToHex(input[i], &buffer[i*2]);
  }
  buffer[length*2] = '\0';
}

LCStringRef LCBlobSHA1(LCBlobRef blob) {
  unsigned char* dataChars = (unsigned char*)blob->data;
  unsigned char buffer[20];
  SHA1(dataChars, strlen((char*)dataChars), buffer);
  char shaString[41];
  unsignedCharsToHex(buffer, 20, shaString); 
  LCStringRef sha = LCStringCreate(shaString);
  return sha;
}