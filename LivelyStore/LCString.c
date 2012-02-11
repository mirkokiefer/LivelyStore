
#include "LCString.h"

struct LCString {
  LCObjectMeta meta;
  char content[];
};

LCStringRef LCStringCreate(char* string) {
  LCStringRef newString = malloc(sizeof(struct LCString) + strlen(string)+1);
  if (newString != NULL) {
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
  size_t bufferPosition = 0;
  size_t currentStringLength;
  for (LCInteger i=0; i<count; i++) {
    currentStringLength = LCStringLength(strings[i])-1;
    memcpy(&buffer[bufferPosition], LCStringStringRef(strings[i]), currentStringLength);
    bufferPosition = bufferPosition + currentStringLength;
  }
  buffer[totalLength-1]='\0';
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