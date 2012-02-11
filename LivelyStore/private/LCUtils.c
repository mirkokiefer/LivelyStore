
#include "LivelyStoreTests.h"

void* LCRetain(void* object) {
  LCObjectInfo* info = (LCObjectInfo*)object;
  info->rCount++;
  return object;
}

void* LCRelease(void* object) {
  LCObjectInfo* info = (LCObjectInfo*)object;
  info->rCount--;
  if(info->rCount == -1) {
    LCTypeRef type = (*info).type;
    if(type->dealloc) {
      type->dealloc(object);      
    }
    free(object);
    //printf("dealloc: %p", object);
  }
  return object;
}

LCInteger LCRetainCount(void* object) {
  LCObjectInfo* info = (LCObjectInfo*)object;
  return info->rCount;
}


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

LCStringRef createHexStringFromData(LCDataRef data) {
  LCByte* input = LCDataDataRef(data);
  size_t length = LCDataLength(data);
  char buffer[length*2];
  for(LCInteger i=0; i<length; i++) {
    byteToHexDigits(input[i], &buffer[i*2]);
  }
  buffer[length*2] = '\0';
  return LCStringCreate(buffer);
}

LCDataRef createDataFromHexString(LCStringRef hexString) {
  LCByte buffer[LC_SHA1_Length];
  for (LCInteger i=0; i<LC_SHA1_Length; i++) {
    char digit1 = LCStringCharAtIndex(hexString, i*2);
    char digit2 = LCStringCharAtIndex(hexString, (i*2)+1);
    char hexDigits[] = {asciCharToHexDigit(digit1), asciCharToHexDigit(digit2)};
    buffer[i] = hexDigitsToByte(hexDigits);
  }
  return LCDataCreate(buffer, LC_SHA1_Length);
}