
#include "LivelyStoreTests.h"
#include "minuit.h"

int tests_run = 0;

static char* test_retain_counting() {
  LCStringRef test = LCStringCreate("abc");
  mu_assert("initial retain count=0", LCRetainCount(test)==0);
  LCRetain(test);
  mu_assert("retain increases retain count", LCRetainCount(test)==1);
  LCRelease(test);
  mu_assert("releasing decreases retain count", LCRetainCount(test)==0);
  LCRelease(test);
  return 0;
}

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
  
  LCBlobRef testData1Blob = LCStringCreateBlob(LCStringCreate(testData1));
  LCBlobRef testData1BlobNoNull = LCBlobCreate((LCByte*)testData1, strlen(testData1));
  LCBlobRef testData2Blob = LCStringCreateBlob(LCStringCreate(testData2));
  LCBlobRef testData3Blob = LCStringCreateBlob(LCStringCreate(testData3));
  LCBlobRef testData1and2Blobs[2];
  testData1and2Blobs[0] = testData1BlobNoNull;
  testData1and2Blobs[1] = testData2Blob;
  
  LCSHARef testData1SHA = LCBlobSHA1(testData1Blob);
  LCSHARef testData3SHA = LCBlobSHA1(testData3Blob);
  LCSHARef testData1and2SHA = LCSHACreate(testData1and2Blobs, 2);
  
  mu_assert("SHA1 from testData1 is correct", LCStringEqual(LCSHACreateHexString(testData1SHA), testData1RealSHA));
  mu_assert("SHA1 from testData3 is correct", LCStringEqual(LCSHACreateHexString(testData3SHA), testData3RealSHA));
  mu_assert("SHA1 from two LCBlobs is correct", LCStringEqual(LCSHACreateHexString(testData1and2SHA), testData3RealSHA));
  mu_assert("LCSHAEqual is correct", LCSHAEqual(testData3SHA, testData1and2SHA));
  return 0;
}


static char* test_blob() {
  char* aCString = "normal string";
  char* sameCString = "normal string";
  char* differentCString = "different string";

  LCBlobRef aBlob = LCBlobCreate((LCByte*)aCString, strlen(aCString)+1);
  LCBlobRef sameBlob = LCBlobCreate((LCByte*)sameCString, strlen(sameCString)+1);
  LCBlobRef differentBlob = LCBlobCreate((LCByte*)differentCString, strlen(differentCString)+1);

  
  LCByte blobData[LCBlobLength(aBlob)];
  LCBlobData(aBlob, blobData);
  
  mu_assert("LCBlob stores data correctly", strcmp(aCString, (char*)blobData)==0);
  mu_assert("LCBlobEqual works with same blob data", LCBlobEqual(aBlob, sameBlob));
  mu_assert("LCBlobEqual works with differing blob data", LCBlobEqual(aBlob, differentBlob)==false);

  return 0;
}

static char* test_keyValue() {
  LCStringRef someKey = LCStringCreate("somekey");
  LCBlobRef someValue = LCBlobCreate((LCByte*)"12345", 6);
  LCKeyValueRef keyValue = LCKeyValueCreate(someKey, someValue);
  mu_assert("LCKeyValue stores key correctly", LCStringEqual(LCKeyValueKey(keyValue), someKey));
  mu_assert("LCKeyValue stores value correctly", LCStringEqual(LCKeyValueKey(keyValue), someKey));
  return 0;
}

static char* test_commit() {
  LCStringRef key1 = LCStringCreate("key1");
  LCStringRef key2 = LCStringCreate("key2");
  LCBlobRef value1 = LCBlobCreate((LCByte*)"12345", 6);
  LCBlobRef value2 = LCBlobCreate((LCByte*)"67890", 6);
  LCKeyValueRef entry1 = LCKeyValueCreate(key1, value1);
  LCKeyValueRef entry2 = LCKeyValueCreate(key2, value2);
  
  LCCommitStageRef commit = LCCommitStageCreate();
  LCCommitStageAddEntry(commit, entry1);
  LCCommitStageAddEntry(commit, entry2);

  mu_assert("LCCommitStage entry adding works", LCCommitStageEntryCount(commit)==2);
  
  LCKeyValueRef entries[LCCommitStageEntryCount(commit)];
  LCCommitStageEntries(commit, entries);
  mu_assert("LCCommitStage stores entries correctly", (entries[0] == entry1) && (entries[1] == entry2));
  return 0;
}

static char* test_tree() {
  LCStringRef key1 = LCStringCreate("key1");
  LCStringRef key2 = LCStringCreate("key2");
  LCStringRef key3 = LCStringCreate("key3");
  LCBlobRef value1 = LCBlobCreate((LCByte*)"12345", 6);
  LCBlobRef value2 = LCBlobCreate((LCByte*)"67890", 6);
  LCKeyValueRef entry1 = LCKeyValueCreate(key1, value1);
  LCKeyValueRef entry2 = LCKeyValueCreate(key2, value2);
  LCKeyValueRef entry3 = LCKeyValueCreate(key3, value2);
  
  LCKeyValueRef entries1[] = {entry1};
  LCKeyValueRef entries2[] = {entry2, entry3};
  
  LCTreeRef tree1 = LCTreeCreate(NULL, 0, entries1, 1);
  LCTreeRef tree2 = LCTreeCreate(&tree1, 1, entries2, 2);
  
  LCKeyValueRef* tree1Entries = LCTreeChildEntries(tree1);
  LCKeyValueRef* tree2Entries = LCTreeChildEntries(tree2);
  LCTreeRef* tree2ChildTrees = LCTreeChildTrees(tree2);
  
  LCBool correct = (tree1Entries[0]==entry1) && (tree2Entries[0]==entry2) && (tree2Entries[1]==entry3);
  mu_assert("LCTree stores entries correctly", correct);
  mu_assert("LCTree stores child trees correctly", tree2ChildTrees[0] == tree1);
  return 0;
}

static char* all_tests() {
  mu_run_test(test_retain_counting);
  mu_run_test(test_string);
  mu_run_test(test_blob);
  mu_run_test(test_sha1);
  mu_run_test(test_keyValue);
  mu_run_test(test_commit);
  mu_run_test(test_tree);
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
