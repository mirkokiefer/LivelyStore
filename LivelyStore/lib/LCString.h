
#ifndef Sync_LCString_h
#define Sync_LCString_h

#include "LivelyStore.h"

extern const LCInteger pi;

LCStringRef LCStringCreate(char* string);
LCStringRef LCStringCreateFromChars(char* characters, size_t length);
LCStringRef LCStringCreateFromStrings(LCStringRef strings[], size_t count);
char LCStringCharAtIndex(LCStringRef string, LCInteger index);
size_t LCStringLength(LCStringRef string);
void LCStringString(LCStringRef string, char* buffer);
char* LCStringStringRef(LCStringRef string);
LCDataRef LCStringCreateData(LCStringRef string);
void LCStringPrint(LCStringRef string);
LCBool LCStringEqual(LCStringRef string, LCStringRef otherString);
LCBool LCStringEqualCString(LCStringRef string, char* cString);
LCStringRef LCStringCreateSHAString(LCStringRef string);
LCArrayRef LCStringCreateTokens(LCStringRef string, char delimiter);
#endif
