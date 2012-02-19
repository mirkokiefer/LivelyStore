
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
  char charArray[] = {'a', 'b', 'c', 'd'};
  LCStringRef aLCString = LCStringCreate(aCString);
  LCStringRef anIdenticalLCString = LCStringCreate(anIdenticalCString);
  LCStringRef stringFromCharArray = LCStringCreateFromChars(charArray, 4);
  
  mu_assert("LCStringEqual is correct", LCStringEqual(aLCString, anIdenticalLCString));
  mu_assert("LCStringCreateFromChars is correct", LCStringEqual(stringFromCharArray, aLCString));
  
  LCStringRef bothStrings = LCStringCreate("abcdabcd");
  LCStringRef stringArray[] = {aLCString, anIdenticalLCString};
  LCStringRef mergedString = LCStringCreateFromStrings(stringArray, 2);
  mu_assert("LCStringCreateFromStrings is correct", LCStringEqual(mergedString, bothStrings));
  
  LCStringRef tokenString = LCStringCreate("ab/cd/ef");
  LCArrayRef tokenArray = LCStringCreateTokens(tokenString, '/');
  LCStringRef* tokens = (LCStringRef*)LCArrayObjects(tokenArray);
  mu_assert("LCStringCreateTokens is correct", LCStringEqualCString(tokens[0], "ab") &&
            LCStringEqualCString(tokens[1], "cd") && LCStringEqualCString(tokens[2], "ef"));
  return 0;
}

static char* test_array() {
  LCStringRef string1 = LCStringCreate("abc");
  LCStringRef string2 = LCStringCreate("def");
  LCStringRef string3 = LCStringCreate("ghi");
  void* stringArray[] = {string1, string2, string3};
  LCArrayRef array = LCArrayCreate(stringArray, 3);
  mu_assert("LCArray stores elements correctly", (LCArrayObjectAtIndex(array, 0)==string1) && (LCArrayObjectAtIndex(array, 1)==string2));
  
  LCArrayRef subArray = LCArrayCreateSubArray(array, 1, -1);
  mu_assert("LCArrayCreateSubArray is correct", (LCArrayObjectAtIndex(subArray, 0)==string2) && (LCArrayObjectAtIndex(subArray, 1)==string3));
  return 0;
}

static char* test_sha1() {
  char* testData1 = "compute sha1";
  LCStringRef testData1RealSHA = LCStringCreate("eefbec885d1042d22ea36fd1690d94dec9029680");
  
  LCStringRef testData1SHA = LCCreateSHAString((LCByte*)testData1, strlen(testData1));
  
  mu_assert("SHA1 from testData1 is correct", LCStringEqual(testData1SHA, testData1RealSHA));
  return 0;
}


static char* test_data() {
  char* aCString = "normal string";
  
  LCDataRef aData = LCDataCreate((LCByte*)aCString, strlen(aCString)+1);
  LCByte* dataFromLCData = LCDataDataRef(aData);
  
  mu_assert("LCData stores data correctly", strcmp(aCString, (char*)dataFromLCData)==0);
  return 0;
}

static char* test_stage() {
  char* path1 = "path1";
  char* path2 = "path2";
  LCByte value1[] = "12345";
  LCByte value2[] = "67890";
  
  LCStageRef stage = LCStageCreate();
  LCStageAddEntry(stage, path1, value1, 5);
  LCStageAddEntry(stage, path2, value2, 5);
  
  LCKeyValueRef* entries = LCStagePathsToAdd(stage);
  mu_assert("LCStage stores entries correctly", (LCStringEqual(LCKeyValueKey(entries[0]), LCStringCreate(path1))));
  return 0;
}

static char* test_tree() {
  LCStringRef path1 = LCStringCreate("path1");
  LCStringRef path2 = LCStringCreate("path2");
  LCStringRef path3 = LCStringCreate("path3");
  LCStringRef value1SHA = LCCreateSHAString((LCByte*)"12345", 6);
  LCStringRef value2SHA = LCCreateSHAString((LCByte*)"67890", 6);
  LCKeyValueRef entry1 = LCKeyValueCreate(path1, value1SHA);
  LCKeyValueRef entry2 = LCKeyValueCreate(path2, value2SHA);
  LCKeyValueRef entry3 = LCKeyValueCreate(path3, value2SHA);
  
  LCKeyValueRef childData1[] = {entry1};
  LCKeyValueRef childData2[] = {entry2, entry3};
  
  LCStringRef tree1Name = LCStringCreate("root");
  LCTreeRef tree1 = LCTreeCreate(NULL, 0, childData1, 1);
  LCKeyValueRef childTree1 = LCKeyValueCreate(tree1Name, tree1);
  LCKeyValueRef childTrees1[] = {childTree1};
  LCTreeRef tree2 = LCTreeCreate(childTrees1, 1, childData2, 2);
  
  LCKeyValueRef* tree1Data = LCTreeChildData(tree1);
  LCKeyValueRef* tree2Data = LCTreeChildData(tree2);
  LCKeyValueRef* tree2ChildTrees = LCTreeChildTrees(tree2);
  
  LCBool correct = (tree1Data[0]==entry1) && (tree2Data[0]==entry2) && (tree2Data[1]==entry3);
  mu_assert("LCTree stores entries correctly", correct);
  mu_assert("LCTree stores child trees correctly", tree2ChildTrees[0] == childTree1);
  
  LCTreeRef tree2Clone = LCTreeCreate(childTrees1, 1, childData2, 2);
  LCStringRef tree1SHA = LCTreeSHA(tree1);
  LCStringRef tree2SHA = LCTreeSHA(tree2);
  LCStringRef tree2CloneSHA = LCTreeSHA(tree2Clone);
  mu_assert("LCTree SHA differs on differing trees", LCStringEqual(tree1SHA, tree2SHA)==false);
  mu_assert("LCTree SHA is identical on identical trees", LCStringEqual(tree2SHA, tree2CloneSHA));
  return 0;
}

static char* all_tests() {
  mu_run_test(test_retain_counting);
  mu_run_test(test_string);
  mu_run_test(test_array);
  mu_run_test(test_data);
  mu_run_test(test_sha1);
  mu_run_test(test_stage);
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
