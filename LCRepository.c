
#include "LCRepository.h"

typedef struct repositoryData* repositoryDataRef;

static void* repositoryInitData();

struct repositoryData {
  LCCommitRef head;
};

void repositoryDealloc(LCObjectRef object);
void storeDataWithSHAs(LCRepositoryRef store, LCKeyValueRef addPaths[], size_t length, LCKeyValueRef pathSHABuffer[]);
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
  LCCommitRef head = LCRepositoryHead(store);
  LCTreeRef oldTree = NULL;
  if (head) {
    oldTree = LCCommitTree(head);
  }
  LCTreeRef newTree = LCTreeCreateTreeUpdatingData(oldTree, LCStageUpdates(stage), LCStageUpdatesLength(stage));

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

static LCStageRef repositoryCreateMergedStage(LCCommitRef base, LCCommitRef local, LCCommitRef foreign,
                                          void *cookie, createResolvedData strategy) {
  LCArrayRef diffLocal = LCCommitDiff(base, local);
  LCArrayRef diffForeign = LCCommitDiff(base, foreign);
  LCMutableDictionaryRef diffMerged = LCMutableDictionaryCreate(LCArrayObjects(diffLocal), LCArrayLength(diffLocal));
  LCKeyValueRef *diffForeignRef = LCArrayObjects(diffForeign);
  for (LCInteger i=0; i<LCArrayLength(diffForeign); i++) {
    LCStringRef path = LCKeyValueKey(diffForeignRef[i]);
    LCObjectRef valueLocal = LCMutableDictionaryValueForKey(diffMerged, path);
    if (valueLocal) {
      LCObjectRef valueForeign = LCKeyValueValue(diffForeignRef[i]);
      LCArrayRef resolved = strategy(cookie, path, valueLocal, valueForeign);
      LCMutableDictionaryAddEntries(diffMerged, LCArrayObjects(resolved), LCArrayLength(resolved));
      objectRelease(resolved);
    } else {
      LCMutableDictionaryAddEntry(diffMerged, diffForeignRef[i]);
    }
  }
  LCStageRef mergeStage = LCStageCreate();
  LCStageAddKeyValues(mergeStage, LCMutableDictionaryEntries(diffMerged), LCMutableDictionaryLength(diffMerged));
  objectRelease(diffMerged);
  objectRelease(diffLocal);
  objectRelease(diffForeign);
  return mergeStage;
}

void LCRepositoryMerge(LCRepositoryRef localRepo, LCRepositoryRef foreignRepo, void *cookie, createResolvedData conflictStrategy) {
  LCCommitRef localHead = LCRepositoryHead(localRepo);
  LCCommitRef foreignHead = LCRepositoryHead(foreignRepo);
  if (!localHead) {
    repositorySetHead(localRepo, foreignHead);
    return;
  }
  if (!foreignHead) {
    return;
  }
  LCCommitRef heads[] = {localHead, foreignHead};
  LCCommitRef commonCommit = LCCommitFindCommonParent(heads, 2);
  if (objectHashEqual(foreignHead, commonCommit)) {
    return;
  }
  if (objectHashEqual(localHead, commonCommit)) {
    repositorySetHead(localRepo, foreignHead);
  } else {
    LCStageRef merged = repositoryCreateMergedStage(commonCommit, localHead, foreignHead, cookie, conflictStrategy);
    LCTreeRef newTree = LCTreeCreateTreeUpdatingData(LCCommitTree(localHead), LCStageUpdates(merged), 
                                                     LCStageUpdatesLength(merged));
    LCCommitRef parents[] = {localHead, foreignHead};
    LCCommitRef newHead = LCCommitCreate(newTree, parents, 2);
    repositorySetHead(localRepo, newHead);
    objectRelease(newHead);
    objectRelease(merged);
  }
}

void repositoryDealloc(LCObjectRef object) {
  objectRelease(LCRepositoryHead(object));
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

LCArrayRef conflictStrategyKeepLocal(void *cookie, LCStringRef path, LCObjectRef localData, LCObjectRef foreignData) {
  LCKeyValueRef resolved = LCKeyValueCreate(path, localData);
  LCArrayRef array = LCArrayCreate(&resolved, 1);
  objectRelease(resolved);
  return array;
}

LCArrayRef conflictStrategyKeepForeign(void *cookie, LCStringRef path, LCObjectRef localData, LCObjectRef foreignData) {
  LCKeyValueRef resolved = LCKeyValueCreate(path, foreignData);
  LCArrayRef array = LCArrayCreate(&resolved, 1);
  objectRelease(resolved);
  return array;
}

LCArrayRef conflictStrategyKeepBoth(void *cookie, LCStringRef path, LCObjectRef localData, LCObjectRef foreignData) {
  char foreignHash[HASH_LENGTH];
  objectHash(foreignData, foreignHash);
  char conflictPath[LCStringLength(path)+1+HASH_LENGTH+1];
  strcpy(conflictPath, LCStringChars(path));
  strcat(conflictPath, "_");
  strcat(conflictPath, foreignHash);
  LCStringRef conflictPathObj = LCStringCreate(conflictPath);
  LCKeyValueRef conflictKeyValue = LCKeyValueCreate(conflictPathObj, foreignData);
  LCKeyValueRef localKeyValue = LCKeyValueCreate(path, localData);
  LCKeyValueRef both[] = {localKeyValue, conflictKeyValue};
  LCArrayRef array = LCArrayCreate(both, 2);
  objectRelease(conflictPathObj);
  objectRelease(conflictKeyValue);
  objectRelease(localKeyValue);
  return array;
}

