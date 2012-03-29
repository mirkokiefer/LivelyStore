
#include "LivelyStoreTests.h"
#include "minuit.h"

int tests_run = 0;

static LCTreeRef testdata_tree() {
  LCStringRef key1 = LCStringCreate("path1");
  LCStringRef key2 = LCStringCreate("path2");
  LCStringRef key3 = LCStringCreate("path3");
  LCStringRef value1 = LCStringCreate("123456");
  LCStringRef value2 = LCStringCreate("78910");
  LCKeyValueRef entry1 = LCKeyValueCreate(key1, value1);
  LCKeyValueRef entry2 = LCKeyValueCreate(key2, value2);
  LCKeyValueRef entry3 = LCKeyValueCreate(key3, value2);
  
  LCKeyValueRef childData1[] = {entry1};
  LCKeyValueRef childData2[] = {entry2, entry3};
  
  LCStringRef tree1Key = LCStringCreate("tree1");
  LCTreeRef tree1 = LCTreeCreate(NULL, 0, childData1, 1);
  
  LCKeyValueRef childTree1 = LCKeyValueCreate(tree1Key, tree1);
  LCKeyValueRef childTrees1[] = {childTree1};
  LCTreeRef tree2 = LCTreeCreate(childTrees1, 1, childData2, 2);
  return tree2;
}

static char* test_tree() {
  LCStringRef key1 = LCStringCreate("path1");
  LCStringRef key2 = LCStringCreate("path2");
  LCStringRef key3 = LCStringCreate("path3");
  LCStringRef value1 = LCStringCreate("123456");
  LCStringRef value2 = LCStringCreate("78910");
  LCKeyValueRef entry1 = LCKeyValueCreate(key1, value1);
  LCKeyValueRef entry2 = LCKeyValueCreate(key2, value2);
  LCKeyValueRef entry3 = LCKeyValueCreate(key3, value2);
  
  LCKeyValueRef childData1[] = {entry1};
  LCKeyValueRef childData2[] = {entry2, entry3};
  
  LCStringRef tree1Key = LCStringCreate("tree1");
  LCTreeRef tree1 = LCTreeCreate(NULL, 0, childData1, 1);
  
  LCKeyValueRef childTree1 = LCKeyValueCreate(tree1Key, tree1);
  LCKeyValueRef childTrees1[] = {childTree1};
  LCTreeRef tree2 = LCTreeCreate(childTrees1, 1, childData2, 2);
  
  
  LCTreeRef* tree2ChildTrees = LCTreeChildTrees(tree2);
  LCKeyValueRef* tree1Data = LCTreeChildData(tree1);
  LCKeyValueRef* tree2Data = LCTreeChildData(tree2);
  bool correct = (tree1Data[0]==entry1) && (tree2Data[0]==entry2) && (tree2Data[1]==entry3);
  mu_assert("LCTree stores entries correctly", correct);
  mu_assert("LCTree stores child trees correctly", tree2ChildTrees[0] == childTree1);
  
  LCTreeRef tree2Clone = LCTreeCopy(tree2);
  char tree1Hash[HASH_LENGTH];
  char tree2Hash[HASH_LENGTH];
  char tree2CloneHash[HASH_LENGTH];
  objectHash(tree1, tree1Hash);
  objectHash(tree2, tree2Hash);
  objectHash(tree2Clone, tree2CloneHash);
  mu_assert("LCTree Hash differs on differing trees", strcmp(tree1Hash, tree2Hash)!=0);
  mu_assert("LCTree Hash is identical on identical trees", strcmp(tree2Hash, tree2CloneHash)==0);
  
  mu_assert("LCTreeChildTreeAtKey", LCTreeChildTreeAtKey(tree2, tree1Key)==tree1);
  
  LCArrayRef path1 = LCArrayCreate(&key2, 1);
  LCObjectRef path2C[] = {tree1Key, key1};
  LCArrayRef path2 = LCArrayCreate(path2C, 2);
  mu_assert("LCTreeChildDataAtPath", (LCTreeChildDataAtPath(tree2, path1)==value2) && (LCTreeChildDataAtPath(tree2, path2)==value1));
  
  /*LCTreeRef tree2FromHash = 
  LCStringRef lazyData1 = LCTreeChildDataAtPath(tree2FromHash, path1);
  LCStringRef lazyData2 = LCTreeChildDataAtPath(tree2FromHash, path2);
  mu_assert("lazy loading of tree", LCStringEqual(lazyData1, value2Hash) && LCStringEqual(lazyData2, value1Hash));

  
  LCTreeRef nullTree = LCTreeCreate(testStore, NULL, NULL);
  LCStringRef nullTreeHash = LCTreeHash(nullTree);
  LCTreeRef nullTreeFromHash = LCTreeCreateFromHash(testStore, nullTreeHash);
  mu_assert("test null tree", LCTreeChildDataAtPath(nullTreeFromHash, path2)==NULL);
  */
  return 0;
}

static char* test_tree_operations() {
  LCTreeRef tree = testdata_tree();
  LCObjectRef path1[] = {LCStringCreate("tree1"), LCStringCreate("path1")};
  LCObjectRef path2[] = {LCStringCreate("path2")};
  LCObjectRef path3[] = {LCStringCreate("path3")};
  LCObjectRef path4[] = {LCStringCreate("tree1"), LCStringCreate("tree2"), LCStringCreate("path4")};
  LCArrayRef path1Array = LCArrayCreate(path1, 2);
  LCArrayRef path2Array = LCArrayCreate(path2, 1);
  LCArrayRef path3Array = LCArrayCreate(path3, 1);
  LCArrayRef path4Array = LCArrayCreate(path4, 3);
  LCStringRef value1 = LCStringCreate("test1");
  LCStringRef value2 = LCStringCreate("test2");
  LCObjectRef updates[] = {LCKeyValueCreate(path1Array, NULL), LCKeyValueCreate(path2Array, NULL),
    LCKeyValueCreate(path3Array, value1), LCKeyValueCreate(path4Array, value2)};
  LCTreeRef newTree = LCTreeCreateTreeUpdatingData(tree, LCMutableArrayCreate(updates, 4));
  
  mu_assert("LCTreeCreateTreeDeletingData", (LCTreeChildDataAtPath(newTree, path1Array)==NULL) &&
            (LCTreeChildDataAtPath(newTree, path2Array)==NULL) &&
            (LCTreeChildDataAtPath(newTree, path3Array)==value1) &&
            (LCTreeChildDataAtPath(newTree, path4Array)==value2));
  
  LCMutableArrayRef changedData = LCMutableArrayCreate(NULL, 0);
  LCMutableArrayRef changedTrees = LCMutableArrayCreate(NULL, 0);
  LCTreeChangedPathValues(tree, newTree, changedData, changedTrees);
  // add assert here
  return 0;
}

static char* test_library_interface() {  
  LCRepositoryRef store = LCRepositoryCreate(NULL);
  LCCommitRef head0 = LCRepositoryHead(store);
  LCStageRef stage1 = LCStageCreate();
  char *data1 = "123456";
  char *data2 = "1234567";
  char *data3 = "12345678";
  char *path1 = "tree1/value1";
  char *path2 = "value2";
  char *path3 = "tree1/tree1_1/value3";
  LCStageAddEntry(stage1, path1, (LCByte*)data1, strlen(data1));
  LCStageAddEntry(stage1, path2, (LCByte*)data2, strlen(data2));
  LCStageAddEntry(stage1, path3, (LCByte*)data3, strlen(data3));
  
  LCRepositoryCommit(store, stage1);
  LCCommitRef head1 = LCRepositoryHead(store);
  LCDataRef data1Retrieved = LCRepositoryData(store, head1, path1);
  LCDataRef data2Retrieved = LCRepositoryData(store, head1, path2);
  LCDataRef data3Retrieved = LCRepositoryData(store, head1, path3);

  mu_assert("commited objects and verify", (memcmp(LCDataDataRef(data1Retrieved), data1, sizeof(LCByte)*strlen(data1))==0) &&
            (memcmp(LCDataDataRef(data2Retrieved), data2, sizeof(LCByte)*strlen(data2))==0) &&
            (memcmp(LCDataDataRef(data3Retrieved), data3, sizeof(LCByte)*strlen(data3))==0));
  
  // another commit
  LCStageRef stage2 = LCStageCreate();
  char *data4 = "123456789";
  LCStageAddEntry(stage2, path1, (LCByte*)data4, strlen(data4));
  LCStageDeletePath(stage2, path3);
  LCRepositoryCommit(store, stage2);
  LCCommitRef head2 = LCRepositoryHead(store);

  LCDataRef data4Retrieved = LCRepositoryData(store, NULL, path1);
  LCDataRef data5Retrieved = LCRepositoryData(store, NULL, path3);
  mu_assert("perform subsequent commit", (memcmp(LCDataDataRef(data4Retrieved), data4, sizeof(LCByte)*strlen(data4))==0) &&
            data5Retrieved == NULL);
  
  
  LCDataRef data6Retrieved = LCRepositoryData(store, head1, path1);
  mu_assert("retrieving previous commit data", memcmp(LCDataDataRef(data6Retrieved), data1, sizeof(LCByte)*strlen(data1))==0);
  
  // find parent commit
  char head0Hash[HASH_LENGTH];
  objectHash(head0, head0Hash);
  LCCommitRef foundHead = LCCommitFindParent(head2, head0Hash);
  mu_assert("LCCommitFindParent", objectHashEqual(foundHead, head0));
  
  LCStringRef homeFolder = getHomeFolder();
  char *strings[] = {LCStringChars(homeFolder), "/testing/"};
  LCStringRef testPath = LCStringCreateFromStringArray(strings, 2);
  LCFileStoreRef fileStore = LCFileStoreCreate(LCStringChars(testPath));
  LCStoreRef storeRef = LCFileStoreStoreObject(fileStore);
  LCContextRef context = createRepositoryContext(storeRef);
  LCRepositoryPersist(store, context);
  LCRepositoryDeleteCache(store);
  LCDataRef lazyData1 = LCRepositoryData(store, NULL, path1);
  char *lazyData1Chars = (char*)LCDataDataRef(lazyData1);
  mu_assert("persisting LCRepository", memcmp(lazyData1Chars, data4, sizeof(LCByte)*strlen(data4))==0);
  deleteDirectory(LCStringChars(testPath));
  
  return 0;
}

static char* all_tests() {
  mu_run_test(test_tree);
  mu_run_test(test_tree_operations);
  mu_run_test(test_library_interface);

  return 0;
}

bool testsRun() {
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
