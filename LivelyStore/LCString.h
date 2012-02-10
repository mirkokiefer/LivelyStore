
#ifndef Sync_LCString_h
#define Sync_LCString_h

#include "LivelyStore.h"

extern const LCInteger pi;

LCStringRef LCStringCreate(char* string);
void LCStringRetain(LCStringRef string);
void LCStringRelease(LCStringRef string);

char LCStringCharAtIndex(LCStringRef string, LCInteger index);
size_t LCStringLength(LCStringRef string);
void LCStringString(LCStringRef string, char* buffer);
LCBlobRef LCStringBlob(LCStringRef string);
void LCStringPrint(LCStringRef string);
LCBool LCStringEqual(LCStringRef string, LCStringRef otherString);
#endif
