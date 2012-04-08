
#include "LivelyStoreTests.h"
#include "url_open.h"

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
  
  LCKeyValueRef children1[] = {entry1};
  
  LCStringRef tree1Key = LCStringCreate("tree1");
  LCTreeRef tree1 = LCTreeCreate(children1, 1);
  
  LCKeyValueRef childTree1 = LCKeyValueCreate(tree1Key, tree1);
  LCKeyValueRef children2[] = {entry2, entry3, childTree1};
  LCTreeRef tree2 = LCTreeCreate(children2, 3);
  return tree2;
}

static LCStringRef create_test_path() {
  LCStringRef homeFolder = getHomeFolder();
  char *strings[] = {LCStringChars(homeFolder), "/testing/"};
  return LCStringCreateFromStringArray(strings, 2);
}

static LCContextRef create_test_context() {
  LCStringRef testPath = create_test_path();
  LCFileStoreRef fileStore = LCFileStoreCreate(LCStringChars(testPath));
  LCStoreRef storeRef = LCFileStoreStoreObject(fileStore);
  return createRepositoryContext(storeRef);
}

static void delete_test_context() {
  LCStringRef testPath = create_test_path();
  deleteDirectory(LCStringChars(testPath));
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
  
  LCKeyValueRef children1[] = {entry1};
  
  LCStringRef tree1Key = LCStringCreate("tree1");
  LCTreeRef tree1 = LCTreeCreate(children1, 1);
  
  LCKeyValueRef childTree1 = LCKeyValueCreate(tree1Key, tree1);
  LCKeyValueRef children2[] = {entry2, entry3, childTree1};
  LCTreeRef tree2 = LCTreeCreate(children2, 3);
  
  
  LCKeyValueRef* tree1Childen = LCTreeChildren(tree1);
  LCKeyValueRef* tree2Children = LCTreeChildren(tree2);
  bool correct = (tree1Childen[0]==entry1) && (tree2Children[0]==entry2) && (tree2Children[1]==entry3) && (tree2Children[2] == childTree1);
  mu_assert("LCTree stores entries correctly", correct);
  
  LCTreeRef tree2Clone = LCTreeCopy(tree2);
  char tree1Hash[HASH_LENGTH];
  char tree2Hash[HASH_LENGTH];
  char tree2CloneHash[HASH_LENGTH];
  objectHash(tree1, tree1Hash);
  objectHash(tree2, tree2Hash);
  objectHash(tree2Clone, tree2CloneHash);
  mu_assert("LCTree Hash differs on differing trees", strcmp(tree1Hash, tree2Hash)!=0);
  mu_assert("LCTree Hash is identical on identical trees", strcmp(tree2Hash, tree2CloneHash)==0);
  
  mu_assert("LCTreeChildTreeAtKey", LCTreeChildAtKey(tree2, tree1Key)==tree1);
  
  LCArrayRef path2 = LCArrayCreate(&key2, 1);
  LCObjectRef path1C[] = {tree1Key, key1};
  LCArrayRef path1 = LCArrayCreate(path1C, 2);
  mu_assert("LCTreeChildDataAtPath", (LCTreeChildAtPath(tree2, path2)==value2) && (LCTreeChildAtPath(tree2, path1)==value1));
  
  /*
  LCTreeRef nullTree = LCTreeCreate(testStore, NULL, NULL);
  LCStringRef nullTreeHash = LCTreeHash(nullTree);
  LCTreeRef nullTreeFromHash = LCTreeCreateFromHash(testStore, nullTreeHash);
  mu_assert("test null tree", LCTreeChildDataAtPath(nullTreeFromHash, path2)==NULL);
  */
  return 0;
}

static char* test_tree_operations() {
  LCTreeRef tree = testdata_tree();
  LCStringRef path1 = LCStringCreate("path2");
  LCStringRef path2 = LCStringCreate("tree1/path1");
  LCStringRef path3 = LCStringCreate("tree1/tree2/path3");
  LCArrayRef path1Array = createPathArray(path1);
  LCArrayRef path2Array = createPathArray(path2);
  LCArrayRef path3Array = createPathArray(path3);
  LCStringRef value1 = LCStringCreate("test1");
  LCKeyValueRef update1 = LCKeyValueCreate(path1Array, NULL);
  LCKeyValueRef update2 = LCKeyValueCreate(path2Array, NULL);
  LCKeyValueRef update3 = LCKeyValueCreate(path3Array, value1);
  LCObjectRef updates[] = {update1, update2, update3};
  LCTreeRef newTree = LCTreeCreateTreeUpdatingData(tree, updates, 3);
  
  mu_assert("LCTreeCreateTreeDeletingData", (LCTreeChildAtPath(newTree, path1Array)==NULL) &&
            (LCTreeChildAtPath(newTree, path2Array)==NULL) &&
            (LCTreeChildAtPath(newTree, path3Array)==value1));
  
  
  LCArrayRef changesArray = LCTreeChangedPathValues(tree, newTree);
  LCKeyValueRef* changes = LCArrayObjects(changesArray);
  
  LCArrayRef pathTree2Array = createPathArray(LCStringCreate("tree1/tree2"));
  LCTreeRef tree2 = LCTreeChildAtPath(newTree, pathTree2Array);
  LCKeyValueRef update3Test = LCKeyValueCreate(pathTree2Array, tree2);
  mu_assert("LCTreeChangedPathValues", objectHashEqual(changes[0], update1) && objectHashEqual(changes[1], update2) && 
            objectHashEqual(changes[2], update3Test));
  return 0;
}

static char* test_library_interface() {  
  LCContextRef context = create_test_context();
  LCRepositoryRef repo1 = LCRepositoryCreate(NULL);
  LCStringRef data1 = LCStringCreate("data1 asflöj alsfjalö");
  LCStringRef data2 = LCStringCreate("data2");
  LCStringRef data3 = LCStringCreate("data3");
  LCStringRef data4 = LCStringCreate("data4 asldafasödflas");
  char *path1 = "tree1/value1";
  char *path2 = "value2";
  char *path3 = "tree1/tree1_1/value3";
    
  {
    // commit data
    LCStageRef stage1 = LCStageCreate();
    LCStageAddEntry(stage1, path1, data1);
    LCStageAddEntry(stage1, path2, data2);
    LCStageAddEntry(stage1, path3, data3);
    
    LCRepositoryCommit(repo1, stage1);
    LCStringRef data1Retrieved = LCRepositoryData(repo1, NULL, path1);
    LCStringRef data2Retrieved = LCRepositoryData(repo1, NULL, path2);
    LCStringRef data3Retrieved = LCRepositoryData(repo1, NULL, path3);
    
    mu_assert("commited objects and verify", (objectCompare(data1Retrieved, data1)==LCEqual) &&
              (objectCompare(data2Retrieved, data2)==LCEqual) &&
              (objectCompare(data3Retrieved, data3)==LCEqual));
  }
  
  LCCommitRef head1 = LCRepositoryHead(repo1);
  char head1Hash[HASH_LENGTH];
  objectHash(head1, head1Hash);

  {
    // another commit
    LCStageRef stage2 = LCStageCreate();
    LCStageAddEntry(stage2, path1, data4);
    LCStageDeletePath(stage2, path3);
    LCRepositoryCommit(repo1, stage2);
    
    LCDataRef data4Retrieved = LCRepositoryData(repo1, NULL, path1);
    LCDataRef data5Retrieved = LCRepositoryData(repo1, NULL, path3);
    mu_assert("perform subsequent commit", (objectCompare(data4Retrieved, data4)==LCEqual) &&
              data5Retrieved == NULL);
    
    
    LCDataRef data6Retrieved = LCRepositoryData(repo1, head1, path1);
    mu_assert("retrieving previous commit data", objectCompare(data6Retrieved, data1)==LCEqual);
  }

  LCCommitRef head2 = LCRepositoryHead(repo1);
  char head2Hash[HASH_LENGTH];
  objectHash(head2, head2Hash);
  
  {
    LCArrayRef diff = LCCommitDiff(head1, head2);
    LCKeyValueRef* changes = LCArrayObjects(diff);
    LCStringRef change1Path = LCKeyValueKey(changes[0]);
    LCObjectRef change1Value = LCKeyValueValue(changes[0]);
    LCStringRef change2Path = LCKeyValueKey(changes[1]);
    LCObjectRef change2Value = LCKeyValueValue(changes[1]);
    bool checkPath1 = LCStringEqualCString(change1Path, path1);
    bool checkValue1 = (objectCompare(change1Value, data4)==LCEqual);
    bool checkPath2 = LCStringEqualCString(change2Path, path3);
    bool checkValue2 = change2Value == NULL;
    mu_assert("LCCommitDiff", checkPath1 && checkValue1 && checkPath2 && checkValue2);
  }
  
  {
    // find parent commit
    LCCommitRef foundHead = LCCommitFindParent(head2, head1);
    mu_assert("LCCommitFindParent", objectHashEqual(foundHead, head1));
  }
  
  LCRepositoryRef repo2 = LCRepositoryCreate(head1);
  {
    // branch repo1
    LCStageRef stage = LCStageCreate();
    LCStageAddEntry(stage, path1, data2);
    LCStageDeletePath(stage, path2);
    LCRepositoryCommit(repo2, stage);
    
  }
  LCCommitRef repo2_head1 = LCRepositoryHead(repo2);
  
  {
    LCCommitRef heads1[] = {head1, head2};
    LCCommitRef commonCommit1 = LCCommitFindCommonParent(heads1, 2);
    mu_assert("LCCommitFindCommonParent direct descendant", objectHashEqual(head1, commonCommit1));
    
    LCCommitRef heads2[] = {repo2_head1, head2};
    LCCommitRef commonCommit2 = LCCommitFindCommonParent(heads2, 2);
    mu_assert("LCCommitFindCommonParent two branches", objectHashEqual(head1, commonCommit2));
  }
  
  {
    LCRepositoryMerge(repo1, repo2, NULL, conflictStrategyKeepLocal);
    LCDataRef data1Retrieved = LCRepositoryData(repo1, NULL, path1);
    LCDataRef data3Retrieved = LCRepositoryData(repo1, NULL, path3);
    mu_assert("LCRepositoryMerge conflicts", (objectCompare(data1Retrieved, data4)==LCEqual) &&
              data3Retrieved == NULL);

    LCRepositoryMerge(repo2, repo1, NULL, conflictStrategyKeepLocal);
    mu_assert("LCRepositoryMerge fast-forward", objectHashEqual(LCRepositoryHead(repo1), LCRepositoryHead(repo2)));
    
    LCRepositoryRef repo3 = LCRepositoryCreate(head2);
    LCRepositoryMerge(repo2, repo3, NULL, conflictStrategyKeepLocal);
    mu_assert("LCRepositoryMerge old repo", objectHashEqual(LCRepositoryHead(repo2), LCRepositoryHead(repo1)));

  }
  
  {
    // test persistence
    LCRepositoryPersist(repo1, context);
    LCRepositoryDeleteCache(repo1, context);
    LCObjectRef lazyData1 = LCRepositoryData(repo1, NULL, path1);
    mu_assert("persisting LCRepository", objectCompare(lazyData1, data4)==LCEqual);
  }
  
  delete_test_context();
  return 0;
}

static char* test_http_interface() {
  LCHttpInterfaceRef http = LCHttpInterfaceCreate(NULL);
  LCHttpInterfaceStart(http, "8080s");
  URL_FILE *url = url_fopen("https://localhost:8080/test", "r");
  LCMutableDataRef data = LCMutableDataCreate(NULL, 0);
  FILE* wstream = createMemoryWriteStream(data, LCMutableDataAppendAlt, NULL);
  pipeURLToFile(url, wstream, 1024);
  fflush(wstream);
  char *test = (char*)LCMutableDataDataRef(data);
  
  mu_assert("test http server", strcmp(test, "/test")==0);
  LCHttpInterfaceStop(http);
  return 0;
}

static char* all_tests() {
  mu_run_test(test_tree);
  mu_run_test(test_tree_operations);
  mu_run_test(test_library_interface);
  mu_run_test(test_http_interface);
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
