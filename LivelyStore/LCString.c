
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
  return strlen(string->content)+1;
}

void LCStringString(LCStringRef string, char* buffer) {
  memcpy(buffer, string->content, strlen(string->content)+1);
}

char* LCStringStringRef(LCStringRef string) {
  return string->content;
}

LCBlobRef LCStringCreateBlob(LCStringRef string) {
  return LCBlobCreate((LCByte*)string->content, strlen(string->content)+1);
}

void LCStringPrint(LCStringRef string) {
  char content[LCStringLength(string)];
  LCStringString(string, content);
  printf("%s\n", content);
}

LCBool LCStringEqual(LCStringRef string, LCStringRef otherString) {
  return strcmp(string->content, otherString->content) == 0;
}