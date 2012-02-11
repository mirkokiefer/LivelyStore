
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
  LCSHARef value1SHA = LCBlobSHA1(LCBlobCreate((LCByte*)"12345", 6));
  LCSHARef value2SHA = LCBlobSHA1(LCBlobCreate((LCByte*)"67890", 6));
  LCKeyValueSHARef entry1 = LCKeyValueSHACreate(key1, value1SHA);
  LCKeyValueSHARef entry2 = LCKeyValueSHACreate(key2, value2SHA);
  LCKeyValueSHARef entry3 = LCKeyValueSHACreate(key3, value2SHA);
  
  LCKeyValueSHARef blobs1[] = {entry1};
  LCKeyValueSHARef blobs2[] = {entry2, entry3};
  
  LCStringRef tree1Name = LCStringCreate("");
  LCTreeRef tree1 = LCTreeCreate(NULL, 0, blobs1, 1);
  LCKeyValueSHARef childTree1 = LCKeyValueSHACreate(tree1Name, LCTreeSHA1(tree1));
  LCKeyValueSHARef childTrees1[] = {childTree1};
  LCTreeRef tree2 = LCTreeCreate(childTrees1, 1, blobs2, 2);
  
  LCKeyValueSHARef* tree1Blobs = LCTreeChildBlobs(tree1);
  LCKeyValueSHARef* tree2Blobs = LCTreeChildBlobs(tree2);
  LCKeyValueSHARef* tree2ChildTrees = LCTreeChildTrees(tree2);
  
  LCBool correct = (tree1Blobs[0]==entry1) && (tree2Blobs[0]==entry2) && (tree2Blobs[1]==entry3);
  mu_assert("LCTree stores entries correctly", correct);
  mu_assert("LCTree stores child trees correctly", tree2ChildTrees[0] == childTree1);
  
  LCTreeRef tree2Clone = LCTreeCreate(childTrees1, 1, blobs2, 2);
  LCSHARef tree1SHA = LCTreeSHA1(tree1);
  LCSHARef tree2SHA = LCTreeSHA1(tree2);
  LCSHARef tree2CloneSHA = LCTreeSHA1(tree2Clone);
  mu_assert("LCTree SHA differs on differing trees", LCSHAEqual(tree1SHA, tree2SHA)==false);
  mu_assert("LCTree SHA is identical on identical trees", LCSHAEqual(tree2SHA, tree2CloneSHA));
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
