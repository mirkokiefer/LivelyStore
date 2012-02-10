

#include "LivelyStore.h"

void sha1example() {
  char* testData = "compute sha1";
  LCBlobRef testBlob = LCBlobCreate(testData, strlen(testData)+1);
  
  LCSHARef sha = LCBlobSHA1(testBlob);
  LCStringPrint(LCSHAHexString(sha));
  
  unsigned char shaDirect[20];
  SHA1((unsigned char*)testData, strlen(testData), shaDirect);
  int i;
  for (i = 0; i < 20; i++) {
    printf("%02x ", shaDirect[i]);
  }
  printf("\n");
}


int main (int argc, const char * argv[])
{

  // insert code here...
  char* aString = "normal string";
  LCStringRef test = LCStringCreate(aString);
  printf("LCStringPrint test: ");
  LCStringPrint(test);
  
  LCBlobRef aBlob = LCBlobCreate(aString, strlen(aString)+1);
  char blobData[LCBlobSize(aBlob)]; 
  LCBlobData(aBlob, blobData);
  printf("test Blob: %s \n", blobData);
  sha1example();
  
  unsigned char test1[3];
  test1[0] = 12;
  test1[1] = 250;
  test1[2] = '\0';
  
  char buffer[4+1];
  return 0;
}
