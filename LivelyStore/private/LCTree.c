
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
void printTree(void* object, FILE* fd);

struct LCTree {
  LCObjectInfo info;
  LCBackendWrapperRef store;
  LCStringRef sha;
  LCDictionaryRef childTrees;
  LCDictionaryRef childData;
};

LCType typeTree = {
  .dealloc = LCTreeDealloc,
  .compare = LCTreeCompare,
  .print = printTree
};

static LCTreeRef treeCreate() {
  LCTreeRef newTree = LCNewObject(&typeTree, sizeof(struct LCTree));
  newTree->store = NULL;
  newTree->sha = NULL;
  newTree->childTrees = NULL;
  newTree->childData = NULL;
  return newTree;
}

static LCBackendWrapperRef treeStore(LCTreeRef tree) {
  return tree->store;
}

static void treeSetStore(LCTreeRef tree, LCBackendWrapperRef store) {
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
  if (sha) {
    tree->sha = LCRetain(sha);
    LCBackendWrapperPutTreeData(treeStore(tree), sha, LCTreeCreateSerializedString(tree));
  }
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

static void treeDeserializeLines(LCTreeRef tree, LCArrayRef lines, LCDictionaryRef childTrees, LCDictionaryRef childData) {
  // read child trees
  LCInteger i;
  for (i=0; i<LCArrayLength(lines); i++) {
    LCStringRef currentLine = (LCStringRef)LCArrayObjectAtIndex(lines, i);
    if (LCStringEqualCString(currentLine, "")) {
      break;
    } else {
      char keyBuffer[LCStringLength(currentLine)-LC_SHA1_HEX_Length];
      char shaBuffer[LC_SHA1_HEX_Length];
      sscanf(LCStringStringRef(currentLine), "%s %s", keyBuffer, shaBuffer);
      LCStringRef key = LCStringCreate(keyBuffer);
      LCStringRef sha = LCStringCreate(shaBuffer);
      LCTreeRef childTree = LCTreeCreateFromSHA(treeStore(tree), sha);
      LCDictionarySetValueForKey(childTrees, key, childTree);
      LCRelease(key);
      LCRelease(sha);
      LCRelease(childTree);
    }
  }
  // read child data
  for (i=i+1; i<LCArrayLength(lines); i++) {
    LCStringRef currentLine = (LCStringRef)LCArrayObjectAtIndex(lines, i);
    char keyBuffer[LCStringLength(currentLine)-LC_SHA1_HEX_Length];
    char shaBuffer[LC_SHA1_HEX_Length];
    sscanf(LCStringStringRef(currentLine), "%s %s", keyBuffer, shaBuffer);
    LCStringRef key = LCStringCreate(keyBuffer);
    LCStringRef sha = LCStringCreate(shaBuffer);
    LCDictionarySetValueForKey(childData, key, sha);
    LCRelease(key);
    LCRelease(sha);
  }
}

static void treeDeserialize(LCTreeRef tree) {
  LCStringRef data = LCBackendWrapperGetTreeData(treeStore(tree), treeSHA(tree));
  LCArrayRef tokens = LCStringCreateTokens(data, '\n');
  LCArrayRef lines = LCArrayCreateSubArray(tokens, 0, LCArrayLength(tokens)-1);
  LCRelease(data);
  LCRelease(tokens);
  LCDictionaryRef childTrees = LCDictionaryCreate(NULL, 0);
  LCDictionaryRef childData = LCDictionaryCreate(NULL, 0);
  if(LCArrayLength(lines)>1) {
    treeDeserializeLines(tree, lines, childTrees, childData);    
  }
  LCRelease(lines);
  treeSetChildTrees(tree, childTrees);
  treeSetChildData(tree, childData);
}

LCTreeRef LCTreeCreateFromSHA(LCBackendWrapperRef store, LCStringRef sha) {
  LCTreeRef newTree = treeCreate();
  newTree->sha = LCRetain(sha);
  newTree->store = LCRetain(store);
  return newTree;
}

LCTreeRef LCTreeCreate(LCBackendWrapperRef store, LCDictionaryRef childTrees, LCDictionaryRef childDataSHAs) {
  LCTreeRef newTree = treeCreate();
  newTree->store = LCRetain(store);
  newTree->childTrees = LCRetain(childTrees);
  newTree->childData = LCRetain(childDataSHAs);
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
    LCTreeRef childTree = LCTreeChildTreeAtKey(tree, first);
    LCStringRef result;
    if (childTree) {
      result = LCTreeChildDataAtPath(childTree, rest);
    } else {
      result = NULL;
    }
    LCRelease(rest);
    return result;
  }
}

LCTreeRef LCTreeCreateTreeUpdatingData(LCTreeRef oldTree, LCBackendWrapperRef store, LCMutableArrayRef updatePathValues) {
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

void printTree(void* object, FILE* fd) {
  LCTreeRef tree = (LCTreeRef)object;
  LCPrint(LCTreeSHA(tree), fd);
}