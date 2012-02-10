
#include "LCString.h"

struct LCString {
  LCInteger rCount;
  char content[];
};

static inline void LCStringDealloc(LCStringRef string);

LCStringRef LCStringCreate(char* string) {
  LCStringRef newString = malloc(sizeof(struct LCString) + strlen(string)+1);
  if (newString != NULL) {
    memcpy(newString->content, string, strlen(string)+1);
  }
  return newString;
};

void LCStringRetain(LCStringRef string) {
  string->rCount = string->rCount + 1;
}

void LCStringRelease(LCStringRef string) {
  string->rCount = string->rCount - 1;
  if(string->rCount == 0) {
    LCStringDealloc(string);
  }
}

char LCStringCharAtIndex(LCStringRef string, LCInteger index) {
  return string->content[index];
}

size_t LCStringLength(LCStringRef string) {
  return strlen(string->content)+1;
}

void LCStringString(LCStringRef string, char* buffer) {
  memcpy(buffer, string->content, strlen(string->content)+1);
}

LCBlobRef LCStringBlob(LCStringRef string) {
  return LCBlobCreate((LCByte*)string->content, strlen(string->content)+1);
}

void LCStringPrint(LCStringRef string) {
  char content[LCStringLength(string)];
  LCStringString(string, content);
  printf("%s\n", content);
}

static inline void LCStringDealloc(LCStringRef string) {
  free(string);
}