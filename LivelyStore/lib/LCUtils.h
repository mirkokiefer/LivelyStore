
#ifndef LivelyStore_LCUtils_h
#define LivelyStore_LCUtils_h

void* LCNewObject(LCType* type, size_t size);
void LCFree(void* memory);
void* LCRetain(void* object);
void* LCRelease(void* object);
LCInteger LCRetainCount(void* object);
LCCompare LCCompareObjects(void* object1, void* object2);
void LCSortObjects(void* objects[], size_t length);

char hexDigitToASCIChar(char hexDigit);
char asciCharToHexDigit(char hexDigit);
void byteToHexDigits(LCByte input, char* buffer);
LCByte hexDigitsToByte(char* hexDigits);
LCStringRef createHexString(LCByte data[], size_t length);
LCDataRef createDataFromHexString(LCStringRef hexString);
LCArrayRef createPathArray(LCStringRef path);
void writeToFile(LCByte data[], size_t length, char* filePath);
int makeDirectory(char* path);
int deleteDirectory(char *path);
LCStringRef getHomeFolder();
#endif
