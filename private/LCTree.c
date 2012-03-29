
#include "LCTree.h"

typedef struct treeData* treeDataRef;

void* treeInitData(void);
static void treeDeserialize(LCTreeRef tree);
void treeDealloc(LCTreeRef object);
void treeWalkChildren(LCObjectRef object, void *cookie, childCallback cb);
void treeStoreChildren(LCObjectRef object, char *key, LCObjectRef objects[], size_t length);

LCTreeRef LCTreeChildTreeAtPath(LCTreeRef tree, LCArrayRef pathArray);

void updateChildData(LCTreeRef tree, LCMutableArrayRef dataPaths);
void updateChildTrees(LCTreeRef parent, LCMutableArrayRef childTreeDataDeletes);
void processUpdatesForChildTreeKey(LCTreeRef parent, LCStringRef key, LCKeyValueRef updatePathArrays[], size_t length);

struct treeData {
  LCMutableDictionaryRef children;
};

struct LCType typeTree = {
  .name = "LCTree",
  .immutable = true,
  .initData = treeInitData,
  .dealloc = treeDealloc,
  .walkChildren = treeWalkChildren,
  .storeChildren = treeStoreChildren
};

LCTypeRef LCTypeTree = &typeTree;

void* treeInitData() {
  treeDataRef data = malloc(sizeof(struct treeData));
  if (data) {
    data->children = LCMutableDictionaryCreate(NULL, 0);
  }
  return data;
}

static treeDataRef treeData(LCTreeRef tree) {
  return objectData(tree);
}

static LCMutableDictionaryRef treeChildren(LCTreeRef tree) {
  return treeData(tree)->children;
}

LCTreeRef LCTreeCreate(LCKeyValueRef children[], size_t childrenLength) {
  treeDataRef data = treeInitData();
  LCMutableDictionaryAddEntries(data->children, children, childrenLength);
  return objectCreate(LCTypeTree, data);
};

size_t LCTreeChildrenLength(LCTreeRef tree) {
  return LCMutableDictionaryLength(treeChildren(tree));
}

LCKeyValueRef* LCTreeChildren(LCTreeRef tree) {
  return LCMutableDictionaryEntries(treeChildren(tree));
}

LCTreeRef LCTreeCopy(LCTreeRef tree) {
  return LCTreeCreate(LCTreeChildren(tree), LCTreeChildrenLength(tree));
}

LCObjectRef LCTreeChildAtKey(LCTreeRef tree, LCStringRef key) {
  return LCMutableDictionaryValueForKey(treeChildren(tree), key);
}

LCObjectRef LCTreeChildAtPath(LCTreeRef tree, LCArrayRef path) {
  LCStringRef first = LCArrayObjectAtIndex(path, 0);
  if (LCArrayLength(path)==1) {
    return LCTreeChildAtKey(tree, first);
  } else {
    LCArrayRef rest = LCArrayCreateSubArray(path, 1, -1);
    LCTreeRef result = LCTreeChildAtPath(LCTreeChildAtKey(tree, first), rest);
    objectRelease(rest);
    return result;
  }
}

LCTreeRef LCTreeCreateTreeUpdatingData(LCTreeRef oldTree, LCKeyValueRef updatePathValues[], size_t updatePathValuesLength) {
  LCTreeRef newTree;
  if (oldTree) {
    newTree = LCTreeCopy(oldTree);    
  } else {
    newTree = LCTreeCreate(NULL, 0);
  }
  
  LCMutableArrayRef directDataUpdates = LCMutableArrayCreate(NULL, 0);
  LCMutableArrayRef childTreeDataUpdates = LCMutableArrayCreate(NULL, 0);
  LCKeyValueRef currentPathValue;
  LCArrayRef currentPath;
  LCKeyValueRef directKeyValue;
  for (LCInteger i=0; i<updatePathValuesLength; i++) {
    currentPathValue = updatePathValues[i];
    currentPath = LCKeyValueKey(currentPathValue);
    if (LCArrayLength(currentPath)==1) {
      directKeyValue = LCKeyValueCreate(LCArrayObjectAtIndex(currentPath, 0), LCKeyValueValue(currentPathValue));
      LCMutableArrayAddObject(directDataUpdates, directKeyValue);
      objectRelease(directKeyValue);
    } else {
      LCMutableArrayAddObject(childTreeDataUpdates, currentPathValue);
    }
  }

  updateChildData(newTree, directDataUpdates);
  updateChildTrees(newTree, childTreeDataUpdates);
  
  objectRelease(directDataUpdates);
  objectRelease(childTreeDataUpdates);
  
  return newTree;
}

static void treeAddKeyValuesToPathValues(LCArrayRef pathArray, LCMutableArrayRef keyValues, LCMutableArrayRef pathValues) {
  for (LCInteger i=0; i<LCMutableArrayLength(keyValues); i++) {
    LCKeyValueRef keyValue = (LCKeyValueRef)LCMutableArrayObjectAtIndex(keyValues, i);
    LCArrayRef newPathArray = LCArrayCreateAppendingObject(pathArray, LCKeyValueKey(keyValue));
    LCKeyValueRef pathValue = LCKeyValueCreate(newPathArray, LCKeyValueValue(keyValue));
    LCMutableArrayAddObject(pathValues, pathValue);
    objectRelease(pathValue);
    objectRelease(newPathArray);
  }
}

static LCArrayRef treeCreateChangedPathValuesArray(LCArrayRef pathArray, LCObjectRef original, LCObjectRef new) {
  LCArrayRef changedKeyValues = LCMutableDictionaryCreateChangesArray(treeChildren(original), treeChildren(new));
  LCMutableArrayRef result = LCMutableArrayCreate(NULL, 0);
  for (LCInteger i=0; i<LCArrayLength(changedKeyValues); i++) {
    LCKeyValueRef keyValue = LCArrayObjectAtIndex(changedKeyValues, i);
    LCStringRef key = LCKeyValueKey(keyValue);
    LCObjectRef newValue = LCKeyValueValue(keyValue);
    LCObjectRef oldValue = LCTreeChildAtKey(original, key);
    LCArrayRef newPathArray = LCArrayCreateAppendingObject(pathArray, key);
    if (objectType(newValue) != LCTypeTree || !newValue || !oldValue) {
      LCKeyValueRef pathValue = LCKeyValueCreate(newPathArray, newValue);
      LCMutableArrayAddObject(result, pathValue);
      objectRelease(pathValue);
    } else {
      LCArrayRef childResult = treeCreateChangedPathValuesArray(newPathArray, oldValue, LCKeyValueValue(keyValue));
      LCMutableArrayAddObjects(result, LCArrayObjects(childResult), LCArrayLength(childResult));
      objectRelease(childResult);
    }
  }
  objectRelease(changedKeyValues);
  return result;
}

LCArrayRef LCTreeChangedPathValues(LCTreeRef originalTree, LCTreeRef newTree) {
  LCArrayRef pathArray = LCArrayCreate(NULL, 0);
  LCArrayRef array = treeCreateChangedPathValuesArray(pathArray, originalTree, newTree);
  objectRelease(pathArray);
  return array;
}

void treeDealloc(LCTreeRef tree) {
  objectRelease(treeChildren(tree));
}

void updateChildData(LCTreeRef tree, LCMutableArrayRef dataPathValues) {
  LCMutableDictionaryAddEntries(treeChildren(tree), LCMutableArrayObjects(dataPathValues), LCMutableArrayLength(dataPathValues));
}

void updateChildTrees(LCTreeRef parent, LCMutableArrayRef childTreeDataUpdates) {
  if (LCMutableArrayLength(childTreeDataUpdates) > 0) {
    LCMutableArraySort(childTreeDataUpdates);
    LCKeyValueRef* childTreeDataUpdatesRef = (LCKeyValueRef*)LCMutableArrayObjects(childTreeDataUpdates);
    LCArrayRef currentPath = LCKeyValueKey(childTreeDataUpdatesRef[0]);
    LCStringRef currentKey = LCArrayObjectAtIndex(currentPath, 0);
    LCArrayRef currentChildPath = LCArrayCreateSubArray(currentPath, 1, -1);
    LCKeyValueRef newChildPathValue = LCKeyValueCreate(currentChildPath, LCKeyValueValue(childTreeDataUpdatesRef[0]));
    LCStringRef previousKey;
    LCMutableArrayRef currentUpdatePaths = LCMutableArrayCreate(&newChildPathValue, 1);
    for (LCInteger i=1; i<LCMutableArrayLength(childTreeDataUpdates); i++) {
      currentPath = LCKeyValueKey(childTreeDataUpdatesRef[i]);
      previousKey = currentKey;
      currentKey = LCArrayObjectAtIndex(currentPath, 0);
      objectRelease(currentChildPath);
      objectRelease(newChildPathValue);
      currentChildPath = LCArrayCreateSubArray(currentPath, 1, -1);
      newChildPathValue = LCKeyValueCreate(currentChildPath, LCKeyValueValue(childTreeDataUpdatesRef[i]));
      if (LCStringEqual(currentKey, previousKey)) {
        LCMutableArrayAddObject(currentUpdatePaths, newChildPathValue);
      } else {
        processUpdatesForChildTreeKey(parent, currentKey, LCMutableArrayObjects(currentUpdatePaths),
                                      LCMutableArrayLength(currentUpdatePaths));
        objectRelease(currentUpdatePaths);
        currentUpdatePaths = LCMutableArrayCreate(&newChildPathValue, 1);
      }
    }
    processUpdatesForChildTreeKey(parent, currentKey, LCMutableArrayObjects(currentUpdatePaths), LCMutableArrayLength(currentUpdatePaths));
  }
}

void processUpdatesForChildTreeKey(LCTreeRef parent, LCStringRef key, LCKeyValueRef updatePathArrays[], size_t length) {
  LCTreeRef currentChildTree = LCTreeChildAtKey(parent, key);
  LCTreeRef newChildTree = LCTreeCreateTreeUpdatingData(currentChildTree, updatePathArrays, length);
  LCMutableDictionarySetValueForKey(treeChildren(parent), key, newChildTree);
  objectRelease(newChildTree);
}

void treeWalkChildren(LCObjectRef object, void *cookie, childCallback cb) {
  LCKeyValueRef *children = LCTreeChildren(object);
  for (LCInteger i=0; i<LCTreeChildrenLength(object); i++) {
    char *key = LCStringChars(LCKeyValueKey(children[i]));
    LCTreeRef value = LCKeyValueValue(children[i]);
    cb(cookie, key, &value, 1, false);
  }
}

void treeStoreChildren(LCObjectRef tree, char *key, LCObjectRef objects[], size_t length) {
  LCObjectRef value = objects[0];
  LCStringRef keyObj = LCStringCreate(key);
  LCMutableDictionarySetValueForKey(treeChildren(tree), keyObj, value);
  objectRelease(keyObj);
}
