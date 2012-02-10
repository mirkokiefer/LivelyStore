

#include "LivelyStore.h"

void sha1example() {
  char* testData = "compute sha1";
  LCBlobRef testBlob = LCBlobCreate((LCByte*)testData, strlen(testData)+1);
  
  LCSHARef sha = LCBlobSHA1(testBlob);
  LCStringPrint(LCSHAHexString(sha));
  
  LCStringRef testString = LCStringCreate(testData);
  LCSHARef sha1 = LCBlobSHA1(LCStringBlob(testString));
  LCStringPrint(LCSHAHexString(sha1));
  
  unsigned char shaDirect[20];
  SHA1((unsigned char*)testData, strlen(testData)+1, shaDirect);
  int i;
  for (i = 0; i < 20; i++) {
    printf("%02x", shaDirect[i]);
  }
  printf("\n");
}


int main (int argc, const char * argv[]) {
  char* aString = "normal string";
  LCStringRef test = LCStringCreate(aString);
  printf("LCStringPrint test: ");
  LCStringPrint(test);
  
  LCBlobRef aBlob = LCBlobCreate((LCByte*)aString, strlen(aString)+1);
  LCByte blobData[LCBlobLength(aBlob)]; 
  LCBlobData(aBlob, blobData);
  printf("test Blob: %s \n", (char*)blobData);
  sha1example();
  
  unsigned char test1[3];
  test1[0] = 12;
  test1[1] = 250;
  test1[2] = '\0';
  
  char buffer[4+1];
  return 0;
}
