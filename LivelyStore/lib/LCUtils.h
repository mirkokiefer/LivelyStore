
#ifndef LivelyStore_LCUtils_h
#define LivelyStore_LCUtils_h

void* LCRetain(void* object);
void* LCRelease(void* object);
LCInteger LCRetainCount(void* object);

char hexDigitToASCIChar(char hexDigit);
char asciCharToHexDigit(char hexDigit);
void byteToHexDigits(LCByte input, char* buffer);
LCByte hexDigitsToByte(char* hexDigits);
LCStringRef createHexString(LCByte data[], size_t length);
LCDataRef createDataFromHexString(LCStringRef hexString);
LCArrayRef createPathArray(LCStringRef path);

#endif
