

#include "LivelyStore.h"

void sha1example() {
  char* testData1 = "compute sha1";
  char* testData2 = " abc";
  char* testData3 = "compute sha1 abc";
  
  unsigned char shaDirect[20];
  SHA1((unsigned char*)testData1, strlen(testData1)+1, shaDirect);
  int i;
  for (i = 0; i < 20; i++) {
    printf("%02x", shaDirect[i]);
  }
  printf("\n");
  
  LCBlobRef testData1Blob = LCBlobCreate((LCByte*)testData1, strlen(testData1)+1);
  LCBlobRef testData1BlobNoNull = LCBlobCreate((LCByte*)testData1, strlen(testData1));
  LCBlobRef testData2Blob = LCStringBlob(LCStringCreate(testData2));
  LCBlobRef testData3Blob = LCStringBlob(LCStringCreate(testData3));
  LCBlobRef testData1and2Blobs[2];
  testData1and2Blobs[0] = testData1BlobNoNull;
  testData1and2Blobs[1] = testData2Blob;

  LCSHARef sha = LCBlobSHA1(testData1Blob);
  LCStringPrint(LCSHAHexString(sha));
  
  LCStringRef testString = LCStringCreate(testData1);
  LCSHARef sha1 = LCBlobSHA1(LCStringBlob(testString));
  LCStringPrint(LCSHAHexString(sha1));
  
  LCSHARef testData3SHA = LCSHACreate(&testData3Blob, 1);
  LCStringPrint(LCSHAHexString(testData3SHA));
  
  LCSHARef testData1and2SHA = LCSHACreate(testData1and2Blobs, 2);
  LCStringPrint(LCSHAHexString(testData1and2SHA));

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
