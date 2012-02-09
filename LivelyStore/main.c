

#include "LivelyStore.h"

void sha1example() {
  char* testData = "compute sha1";
  LCBlobRef testBlob = LCBlobCreate(testData, strlen(testData)+1);
  
  unsigned char shaBlob[20];
  LCBlobSHA1(testBlob, shaBlob);

  unsigned char shaDirect[20];
  SHA1((unsigned char*)testData, strlen(testData), shaDirect);
  
  int i;
  for (i = 0; i < 20; i++) {
    printf("%02x:%02x ", shaBlob[i], shaDirect[i]);
  }
  printf("\n");
}


int main (int argc, const char * argv[])
{

  // insert code here...
  LCStringRef test = LCStringCreate("string one");
  char* aString = "normal string";
  printf("%i %s \n", LCStringSize(test), aString);
  
  char copyString[LCStringSize(test)];
  LCStringString(test, copyString);
  printf("%s\n", copyString);
  
  LCStringRef stringArray[2];
  stringArray[0] = test;
  stringArray[1] = LCStringCreate("string two");
  
  for (LCInteger i=0; i<2; i++) {
    LCStringString(stringArray[i], copyString);
    printf("%i: %s \n", i, copyString);
  }
  
  LCBlobRef aBlob = LCBlobCreate(aString, strlen(aString)+1);
  char blobData[LCBlobSize(aBlob)]; 
  LCBlobData(aBlob, blobData);
  printf("test Blob: %s \n", blobData);
  sha1example();
  
  return 0;
}
