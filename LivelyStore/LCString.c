
#include "LCString.h"

struct LCString {
  size_t length;
  LCInteger rCount;
  char content[];
};

static inline void LCStringDealloc(LCStringRef string);

LCStringRef LCStringCreate(char* string) {
  LCStringRef newString = malloc(sizeof(struct LCString) + strlen(string));
  if (newString != NULL) {
    newString->length = strlen(string);
    memcpy(newString->content, string, newString->length);
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

size_t LCStringSize(LCStringRef string) {
  return string->length+1;
}

void LCStringString(LCStringRef string, char* buffer) {
  memcpy(buffer, string->content, string->length);
  buffer[string->length] = '\0';
}

void LCStringPrint(LCStringRef string) {
  char content[LCStringSize(string)];
  LCStringString(string, content);
  printf("%s", content);
}

static inline void LCStringDealloc(LCStringRef string) {
  free(string);
}