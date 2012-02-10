
#include "LivelyStoreTests.h"
#include "minuit.h"

int tests_run = 0;

static char* test_string() {
  char* aCString = "abcd";
  char* anIdenticalCString = "abcd";
  LCStringRef aLCString = LCStringCreate(aCString);
  LCStringRef anIdenticalLCString = LCStringCreate(anIdenticalCString);
  
  mu_assert("LCStringEqual", LCStringEqual(aLCString, anIdenticalLCString));
  return 0;
}

static char* test_sha1() {
  char* testData1 = "compute sha1";
  char* testData2 = " abc";
  char* testData3 = "compute sha1 abc";
  LCStringRef testData1RealSHA = LCStringCreate("a0e1e14cb346a01f9fd10e60080b945f995524af");
  LCStringRef testData3RealSHA = LCStringCreate("253f7e747aa2bd4107cc0af2f78fe60fc075d090");
  
  LCBlobRef testData1Blob = LCStringBlob(LCStringCreate(testData1));
  LCBlobRef testData1BlobNoNull = LCBlobCreate((LCByte*)testData1, strlen(testData1));
  LCBlobRef testData2Blob = LCStringBlob(LCStringCreate(testData2));
  LCBlobRef testData3Blob = LCStringBlob(LCStringCreate(testData3));
  LCBlobRef testData1and2Blobs[2];
  testData1and2Blobs[0] = testData1BlobNoNull;
  testData1and2Blobs[1] = testData2Blob;
  
  LCSHARef testData1SHA = LCBlobSHA1(testData1Blob);
  LCSHARef testData3SHA = LCBlobSHA1(testData3Blob);
  LCSHARef testData1and2SHA = LCSHACreate(testData1and2Blobs, 2);
  
  mu_assert("SHA1 from testData1 is correct", LCStringEqual(LCSHAHexString(testData1SHA), testData1RealSHA));
  mu_assert("SHA1 from testData3 is correct", LCStringEqual(LCSHAHexString(testData3SHA), testData3RealSHA));
  mu_assert("SHA1 from two LCBlobs is correct", LCStringEqual(LCSHAHexString(testData1and2SHA), testData3RealSHA));

  return 0;
}


static char* test_blob() {
  char* aCString = "normal string";
  LCBlobRef aBlob = LCBlobCreate((LCByte*)aCString, strlen(aCString)+1);
  
  LCByte blobData[LCBlobLength(aBlob)];
  LCBlobData(aBlob, blobData);
  
  mu_assert("LCBlob stores data correctly", strcmp(aCString, (char*)blobData)==0);
  
  return 0;
}

static char* all_tests() {
  mu_run_test(test_string);
  mu_run_test(test_blob);
  mu_run_test(test_sha1);
  return 0;
}

LCBool testsRun() {
  char *result = all_tests();
  if (result != 0) {
    printf("%s\n", result);
  }
  else {
    printf("ALL TESTS PASSED\n");
  }
  printf("Tests run: %d\n", tests_run);
  
  return result == 0;
}
