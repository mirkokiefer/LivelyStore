
#ifndef Sync_LCString_h
#define Sync_LCString_h

#include "LivelyStore.h"

extern const LCInteger pi;

LCStringRef LCStringCreate(char* string);
void LCStringRetain(LCStringRef string);
void LCStringRelease(LCStringRef string);

char LCStringCharAtIndex(LCStringRef string, LCInteger index);
size_t LCStringSize(LCStringRef string);
void LCStringString(LCStringRef string, char* buffer);
void LCStringPrint(LCStringRef string);
#endif
