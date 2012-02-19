
#include "LCString.h"

struct LCString {
  LCObjectInfo info;
  char content[];
};

LCType typeString = {
  .dealloc = NULL,
};

LCStringRef LCStringCreate(char* string) {
  LCStringRef newString = malloc(sizeof(struct LCString) + strlen(string)+1);
  if (newString != NULL) {
    newString->info.type = &typeString;
    memcpy(newString->content, string, strlen(string)+1);
  }
  return newString;
};

LCStringRef LCStringCreateFromChars(char* characters, size_t length) {
  char string[length+1];
  string[length] = '\0';
  memcpy(string, characters, length*sizeof(char));
  return LCStringCreate(string);
}

LCStringRef LCStringCreateFromStrings(LCStringRef strings[], size_t count) {
  size_t totalLength = 1;
  for (LCInteger i=0; i<count; i++) {
    totalLength = totalLength + LCStringLength(strings[i])-1;
  }
  char buffer[totalLength];
  buffer[0]='\0';
  for (LCInteger i=0; i<count; i++) {
    strcat(buffer, LCStringStringRef(strings[i]));
  }
  return LCStringCreate(buffer);
}

char LCStringCharAtIndex(LCStringRef string, LCInteger index) {
  return string->content[index];
}

size_t LCStringLength(LCStringRef string) {
  return strlen(string->content);
}

void LCStringString(LCStringRef string, char* buffer) {
  memcpy(buffer, string->content, strlen(string->content)+1);
}

char* LCStringStringRef(LCStringRef string) {
  return string->content;
}

LCDataRef LCStringCreateData(LCStringRef string) {
  return LCDataCreate((LCByte*)string->content, strlen(string->content)+1);
}

void LCStringPrint(LCStringRef string) {
  char content[LCStringLength(string)];
  LCStringString(string, content);
  printf("%s\n", content);
}

LCBool LCStringEqual(LCStringRef string, LCStringRef otherString) {
  return strcmp(string->content, otherString->content) == 0;
}

LCBool LCStringEqualCString(LCStringRef string, char* cString) {
  return strcmp(string->content, cString) == 0;
}

LCArrayRef LCStringCreateTokens(LCStringRef string, char delimiter) {
  char* cString = string->content;
  LCStringRef substrings[strlen(cString)/2];
  LCInteger tokenStart = 0;
  LCInteger substringCount = 0;
  for (LCInteger i=0; i<strlen(cString); i++) {
    if(cString[i] == delimiter) {
      substrings[substringCount] = LCStringCreateFromChars(&cString[tokenStart], i-tokenStart);
      substringCount = substringCount + 1;
      tokenStart = i+1;
    }
  }
  substrings[substringCount] = LCStringCreate(&cString[tokenStart]);
  substringCount = substringCount + 1;
  LCArrayRef array = LCArrayCreate((void**)substrings, substringCount);
  for(LCInteger i=0; i<substringCount; i++) {
    LCRelease(substrings[i]);
  }
  return array;
}

LCStringRef LCStringCreateSHAString(LCStringRef string) {
  return LCCreateSHAString((LCByte*)string->content, LCStringLength(string));
}
