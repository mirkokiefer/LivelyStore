
#include "LCRepository.h"

typedef struct repositoryData* repositoryDataRef;

static void* repositoryInitData();

struct repositoryData {
  LCCommitRef head;
};

void repositoryDealloc(LCObjectRef object);
void storeDataWithSHAs(LCRepositoryRef store, LCKeyValueRef addPaths[], size_t length, LCKeyValueRef pathSHABuffer[]);
LCTreeRef repositoryBuildTree(LCRepositoryRef store, LCTreeRef current, LCKeyValueRef* addPaths, size_t newLength,
                    LCStringRef* delete, size_t deleteLength);
void repositorySetHead(LCRepositoryRef store, LCCommitRef newHead);

struct LCType typeRepository = {
  .name = "LCRepository",
  .immutable = false,
  .initData = repositoryInitData,
  .dealloc = repositoryDealloc
};

LCTypeRef LCTypeRepository = &typeRepository;

static void* repositoryInitData() {
  repositoryDataRef data = malloc(sizeof(struct repositoryData));
  if (data) {
    data->head = NULL;
  }
  return data;
}

LCRepositoryRef LCRepositoryCreate(LCCommitRef head) {
  repositoryDataRef data = repositoryInitData();
  if (head) {
    data->head = objectRetain(head);
  }
  return objectCreate(LCTypeRepository, data);
};

void LCRepositoryCommit(LCRepositoryRef store, LCStageRef stage) {
  LCKeyValueRef* addPaths = LCStagePathsToAdd(stage);
  size_t addPathsLength = LCStageAddPathsCount(stage);
  LCArrayRef* deletePaths = LCStagePathsToDelete(stage);
  size_t deletePathsLength = LCStageDeletePathsCount(stage);
  
  LCCommitRef head = LCRepositoryHead(store);
  LCTreeRef oldTree = NULL;
  if (head) {
    oldTree = LCCommitTree(head);
  }
  LCTreeRef newTree = repositoryBuildTree(store, oldTree, addPaths, addPathsLength, deletePaths, deletePathsLength);
  
  LCCommitRef newHead;
  if (head) {
    newHead = LCCommitCreate(newTree, &head, 1);
  } else {
    newHead = LCCommitCreate(newTree, NULL, 0);
  }
  repositorySetHead(store, newHead);
  objectRelease(newHead);
}

LCCommitRef LCRepositoryHead(LCRepositoryRef store) {
  repositoryDataRef data = objectData(store);
  return data->head;
}

LCObjectRef LCRepositoryData(LCRepositoryRef store, LCCommitRef commit, char* path) {
  if (!commit) {
    commit = LCRepositoryHead(store);
  }
  LCStringRef pathObj = LCStringCreate(path);
  LCArrayRef pathArray = createPathArray(pathObj);
  LCObjectRef data = LCTreeChildAtPath(LCCommitTree(commit), pathArray);
  objectRelease(pathArray);
  objectRelease(pathObj);
  return data;
}

void LCRepositoryPersist(LCRepositoryRef repo, LCContextRef context) {
  objectStore(LCRepositoryHead(repo), context);
}

void LCRepositoryDeleteCache(LCRepositoryRef repo, LCContextRef context) {
  objectDeleteCache(LCRepositoryHead(repo), context);
}

void repositoryDealloc(LCObjectRef object) {
  objectRelease(LCRepositoryHead(object));
}

LCTreeRef repositoryBuildTree(LCRepositoryRef store, LCTreeRef current, LCKeyValueRef addPaths[], size_t addPathsLength,
                    LCArrayRef* delete, size_t deleteLength) {
  LCMutableArrayRef updates = LCMutableArrayCreate(addPaths, addPathsLength);
  LCKeyValueRef deleteUpdate;
  for (LCInteger i=0; i<deleteLength; i++) {
    deleteUpdate = LCKeyValueCreate(delete[i], NULL);
    LCMutableArrayAddObject(updates, deleteUpdate);
    objectRelease(deleteUpdate);
  }
  LCTreeRef newTree = LCTreeCreateTreeUpdatingData(current, LCMutableArrayObjects(updates), LCMutableArrayLength(updates));
  objectRelease(updates);
  return newTree;
}

void repositorySetHead(LCRepositoryRef store, LCCommitRef newHead) {
  repositoryDataRef data = objectData(store);
  if (newHead != data->head) {
    objectRelease(data->head);
  }
  data->head = objectRetain(newHead);
}

static LCTypeRef livelyStoreStringToType(char *typeString) {
  LCTypeRef types[] = {LCTypeRepository, LCTypeStage, LCTypeCommit, LCTypeTree};
  for (LCInteger i=0; i<4; i++) {
    if (strcmp(typeString, typeName(types[i]))==0) {
      return types[i]; 
    }
  }
  return NULL;
}

LCContextRef createRepositoryContext(LCStoreRef store) {
  stringToType funs[] = {livelyStoreStringToType, coreStringToType};
  return contextCreate(store, funs, 2);
}
