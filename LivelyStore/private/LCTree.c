
#include "LCStage.h"

static void treeDeserialize(LCTreeRef tree);

void LCTreeDealloc(void* object);
size_t childrenSerializationBufferSize(LCTreeRef tree);
void serializeChildTrees(LCDictionaryRef childTrees, char buffer[]);
void serializeChildData(LCDictionaryRef childData, char buffer[]);
void serializePathSHA(LCStringRef path, LCStringRef sha, char buffer[]);
LCTreeRef LCTreeChildTreeAtPath(LCTreeRef tree, LCArrayRef pathArray);

void updateChildData(LCTreeRef tree, LCMutableArrayRef dataPaths);
void updateChildTrees(LCTreeRef parent, LCMutableArrayRef childTreeDataDeletes);
void processUpdatesForChildTreeKey(LCTreeRef parent, LCStringRef key, LCMutableArrayRef deletePathArrays);

struct LCTree {
  LCObjectInfo info;
  LCDataStoreRef store;
  LCStringRef sha;
  LCDictionaryRef childTrees;
  LCDictionaryRef childData;
};

LCType typeTree = {
  .dealloc = LCTreeDealloc
};

static LCTreeRef treeCreate() {
  LCTreeRef newTree = malloc(sizeof(struct LCTree));
  if (newTree) {
    newTree->info.type = &typeTree;
  }
  return newTree;
}

static LCDataStoreRef treeStore(LCTreeRef tree) {
  return tree->store;
}

static void treeSetStore(LCTreeRef tree, LCDataStoreRef store) {
  if (store != tree->store) {
    LCRelease(tree->store);
  }
  tree->store = LCRetain(store);
}

static LCStringRef treeSHA(LCTreeRef tree) {
  return tree->sha;
}

static void treeSetSHA(LCTreeRef tree, LCStringRef sha) {
  if (sha != tree->sha) {
    LCRelease(tree->sha);
  }
  tree->sha = LCRetain(sha);
}

static void treeSetChildTrees(LCTreeRef tree, LCDictionaryRef childTrees) {
  if (childTrees != tree->childTrees) {
    LCRelease(tree->childTrees);
  }
  tree->childTrees = LCRetain(childTrees);
}

static LCDictionaryRef treeChildTrees(LCTreeRef tree) {
  if(tree->childTrees == NULL) {
    if (treeSHA(tree)) {
      treeDeserialize(tree);
    } else {
      tree->childTrees = LCDictionaryCreate(NULL, 0);
    }
  }
  return tree->childTrees;
}

static void treeSetChildData(LCTreeRef tree, LCDictionaryRef childData) {
  if (childData != tree->childData) {
    LCRelease(tree->childData);
  }
  tree->childData = LCRetain(childData);
}

static LCDictionaryRef treeChildData(LCTreeRef tree) {
  if (tree->childData == NULL) {
    if (treeSHA(tree)) {
      treeDeserialize(tree);
    } else {
      tree->childData = LCDictionaryCreate(NULL, 0);    
    }
  }
  return tree->childData;
}

static void treeDeserialize(LCTreeRef tree) {
  LCStringRef data = LCDataStoreGetTreeData(treeStore(tree), treeSHA(tree));
  LCArrayRef lines = LCStringCreateTokens(data, '\n');
  LCRelease(data);
  LCStringRef currentLine;
  LCStringRef key;
  LCStringRef sha;
  LCDictionaryRef childTrees;
  LCDictionaryRef childData;
  LCMutableArrayRef keyValues = LCMutableArrayCreate(NULL, 0);
  LCInteger i;
  for (i=0; i<LCArrayLength(lines); i++) {
    currentLine = (LCStringRef)LCArrayObjectAtIndex(lines, i);
    if (LCStringEqualCString(currentLine, "")) {
      childTrees = LCDictionaryCreate((LCKeyValueRef*)LCMutableArrayObjects(keyValues), LCMutableArrayLength(keyValues));
      LCRelease(keyValues);
      keyValues = LCMutableArrayCreate(NULL, 0);
      continue;
    } else {
      char keyBuffer[LCStringLength(currentLine)-LC_SHA1_HEX_Length];
      char shaBuffer[LC_SHA1_HEX_Length];
      sscanf(LCStringStringRef(currentLine), "%s %s", keyBuffer, shaBuffer);
      key = LCStringCreate(keyBuffer);
      sha = LCStringCreate(shaBuffer);
      LCKeyValueRef keyValue = LCKeyValueCreate(key, sha);
      LCMutableArrayAddObject(keyValues, keyValue);
      LCRelease(key);
      LCRelease(sha);
      LCRelease(keyValue);
    }
  }
  childData = LCDictionaryCreate((LCKeyValueRef*)LCMutableArrayObjects(keyValues), LCMutableArrayLength(keyValues));
  LCRelease(lines);
  LCRelease(keyValues);
  treeSetChildTrees(tree, childTrees);
  treeSetChildData(tree, childData);
}

LCTreeRef LCTreeCreateFromSHA(LCDataStoreRef store, LCStringRef sha) {
  LCTreeRef newTree = treeCreate();
  treeSetSHA(newTree, sha);
  return newTree;
}

LCTreeRef LCTreeCreate(LCDataStoreRef store, LCDictionaryRef childTrees, LCDictionaryRef childDataSHAs) {
  LCTreeRef newTree = treeCreate();
  treeSetStore(newTree, store);
  treeSetChildTrees(newTree, childTrees);
  treeSetChildData(newTree, childDataSHAs);
  return newTree;
};

LCDictionaryRef LCTreeChildTrees(LCTreeRef tree) {
  return treeChildTrees(tree);
}

LCDictionaryRef LCTreeChildData(LCTreeRef tree) {
  return treeChildData(tree);
}

LCStringRef LCTreeCreateSerializedString(LCTreeRef tree) {
  size_t bufferSize = childrenSerializationBufferSize(tree) + 1;
  char buffer[bufferSize];
  strcpy(buffer, "");
  
  // write child trees:
  serializeChildTrees(treeChildTrees(tree), buffer);
  strcat(buffer, "\n");
  // write child data:
  serializeChildData(treeChildData(tree), buffer);
  return LCStringCreate(buffer);
}

LCStringRef LCTreeSHA(LCTreeRef tree) {
  if(treeSHA(tree) == NULL) {
    LCStringRef serialized = LCTreeCreateSerializedString(tree);
    LCStringRef sha = LCStringCreateSHAString(serialized);
    treeSetSHA(tree, sha);
    LCRelease(serialized);
    LCRelease(sha);
  }
  return treeSHA(tree);
}

LCTreeRef LCTreeCopy(LCTreeRef tree) {
  LCDictionaryRef childTreesCopy = LCDictionaryCopy(treeChildTrees(tree));
  LCDictionaryRef childDataCopy = LCDictionaryCopy(treeChildData(tree));
  LCTreeRef treeCopy = LCTreeCreate(treeStore(tree), childTreesCopy, childDataCopy);
  LCRelease(childTreesCopy);
  LCRelease(childDataCopy);
  return treeCopy;
}

LCTreeRef LCTreeChildTreeAtKey(LCTreeRef tree, LCStringRef key) {
  return LCDictionaryValueForKey(treeChildTrees(tree), key);
}

LCStringRef LCTreeChildDataAtKey(LCTreeRef tree, LCStringRef key) {
  return LCDictionaryValueForKey(treeChildData(tree), key);
}

LCTreeRef LCTreeChildTreeAtPath(LCTreeRef tree, LCArrayRef path) {
  LCStringRef first = LCArrayObjectAtIndex(path, 0);
  if (LCArrayLength(path)==1) {
    return LCTreeChildTreeAtKey(tree, first);
  } else {
    LCArrayRef rest = LCArrayCreateSubArray(path, 1, -1);
    LCTreeRef result = LCTreeChildTreeAtPath(tree, rest);
    LCRelease(rest);
    return result;
  }
}

LCStringRef LCTreeChildDataAtPath(LCTreeRef tree, LCArrayRef path) {
  LCStringRef first = LCArrayObjectAtIndex(path, 0);
  if (LCArrayLength(path)==1) {
    return LCTreeChildDataAtKey(tree, first);
  } else {
    LCArrayRef rest = LCArrayCreateSubArray(path, 1, -1);
    LCStringRef result = LCTreeChildDataAtPath(LCTreeChildTreeAtKey(tree, first), rest);
    LCRelease(rest);
    return result;
  }
}

LCTreeRef LCTreeCreateTreeUpdatingData(LCTreeRef oldTree, LCDataStoreRef store, LCMutableArrayRef updatePathValues) {
  LCTreeRef newTree;
  if (oldTree) {
    newTree = LCTreeCopy(oldTree);    
  } else {
    newTree = LCTreeCreate(store, NULL, NULL);
  }
  
  LCMutableArrayRef directDataUpdates = LCMutableArrayCreate(NULL, 0);
  LCMutableArrayRef childTreeDataUpdates = LCMutableArrayCreate(NULL, 0);
  LCKeyValueRef currentPathValue;
  LCArrayRef currentPath;
  LCKeyValueRef directKeyValue;
  for (LCInteger i=0; i<LCMutableArrayLength(updatePathValues); i++) {
    currentPathValue = LCMutableArrayObjectAtIndex(updatePathValues, i);
    currentPath = LCKeyValueKey(currentPathValue);
    if (LCArrayLength(currentPath)==1) {
      directKeyValue = LCKeyValueCreate(LCArrayObjectAtIndex(currentPath, 0), LCKeyValueValue(currentPathValue));
      LCMutableArrayAddObject(directDataUpdates, directKeyValue);
      LCRelease(directKeyValue);
    } else {
      LCMutableArrayAddObject(childTreeDataUpdates, currentPathValue);
    }
  }

  updateChildData(newTree, directDataUpdates);
  updateChildTrees(newTree, childTreeDataUpdates);
  
  LCRelease(directDataUpdates);
  LCRelease(childTreeDataUpdates);
  
  return newTree;
}

void LCTreeDealloc(void* object) {
  LCTreeRef tree = (LCTreeRef)object;
  treeSetStore(tree, NULL);
  treeSetSHA(tree, NULL);
  treeSetChildTrees(tree, NULL);
  treeSetChildData(tree, NULL);
}

// private
size_t childrenSerializationBufferSize(LCTreeRef tree) {
  size_t childrenLength = LCDictionaryLength(treeChildTrees(tree)) + LCDictionaryLength(treeChildData(tree));
  size_t sumPathLength = 0;
  LCKeyValueRef* keyValues = LCDictionaryEntries(treeChildTrees(tree));
  for (LCInteger i=0; i<LCDictionaryLength(treeChildTrees(tree)); i++) {
    sumPathLength = sumPathLength + LCStringLength(LCKeyValueKey(keyValues[i]));
  }
  keyValues = LCDictionaryEntries(treeChildData(tree));
  for (LCInteger i=0; i<LCDictionaryLength(treeChildData(tree)); i++) {
    sumPathLength = sumPathLength + LCStringLength(LCKeyValueKey(keyValues[i]));
  }
  return sumPathLength + childrenLength * (LC_SHA1_HEX_Length + 1);
}

void serializeChildTrees(LCDictionaryRef childTrees, char buffer[]) {
  LCTreeRef childTree;
  LCKeyValueRef* keyValues = LCDictionaryEntries(childTrees);
  for (LCInteger i=0; i<LCDictionaryLength(childTrees); i++) {
    childTree = LCKeyValueValue(keyValues[i]);
    serializePathSHA(LCKeyValueKey(keyValues[i]), LCTreeSHA(childTree), buffer);
  }
}

void serializeChildData(LCDictionaryRef childData, char buffer[]) {
  LCKeyValueRef* keyValues = LCDictionaryEntries(childData);
  for (LCInteger i=0; i<LCDictionaryLength(childData); i++) {
    serializePathSHA(LCKeyValueKey(keyValues[i]), LCKeyValueValue(keyValues[i]), buffer);
  }
}

void serializePathSHA(LCStringRef path, LCStringRef sha, char buffer[]) {
  strcat(buffer, LCStringStringRef(path));
  strcat(buffer, " ");
  strcat(buffer, LCStringStringRef(sha));
  strcat(buffer, "\n");
}

void updateChildData(LCTreeRef tree, LCMutableArrayRef dataPathValues) {
  LCDictionaryAddEntries(treeChildData(tree), (LCKeyValueRef*)LCMutableArrayObjects(dataPathValues), LCMutableArrayLength(dataPathValues));
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
    LCMutableArrayRef currentUpdatePaths = LCMutableArrayCreate((void**)&newChildPathValue, 1);
    for (LCInteger i=1; i<LCMutableArrayLength(childTreeDataUpdates); i++) {
      currentPath = LCKeyValueKey(childTreeDataUpdatesRef[i]);
      previousKey = currentKey;
      currentKey = LCArrayObjectAtIndex(currentPath, 0);
      LCRelease(currentChildPath);
      LCRelease(newChildPathValue);
      currentChildPath = LCArrayCreateSubArray(currentPath, 1, -1);
      newChildPathValue = LCKeyValueCreate(currentChildPath, LCKeyValueValue(childTreeDataUpdatesRef[i]));
      if (LCStringEqual(currentKey, previousKey)) {
        LCMutableArrayAddObject(currentUpdatePaths, newChildPathValue);
      } else {
        processUpdatesForChildTreeKey(parent, currentKey, currentUpdatePaths);
        LCRelease(currentUpdatePaths);
        currentUpdatePaths = LCMutableArrayCreate((void**)&newChildPathValue, 1);
      }
    }
    processUpdatesForChildTreeKey(parent, currentKey, currentUpdatePaths);
  }
}

void processUpdatesForChildTreeKey(LCTreeRef parent, LCStringRef key, LCMutableArrayRef updatePathArrays) {
  LCTreeRef currentChildTree = LCTreeChildTreeAtKey(parent, key);
  LCTreeRef newChildTree = LCTreeCreateTreeUpdatingData(currentChildTree, treeStore(parent), updatePathArrays);
  LCDictionarySetValueForKey(treeChildTrees(parent), key, newChildTree);
  LCRelease(newChildTree);
}
