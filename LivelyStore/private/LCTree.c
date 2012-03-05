
#include "LCStage.h"

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
  LCStringRef sha;
  LCDictionaryRef childTrees;
  LCDictionaryRef childData;
};

LCType typeTree = {
  .dealloc = LCTreeDealloc
};

LCTreeRef LCTreeCreate(LCDictionaryRef childTrees, LCDictionaryRef childDataSHAs) {
  LCTreeRef newTree = malloc(sizeof(struct LCTree));
  if (newTree != NULL) {
    newTree->info.type = &typeTree;
    if(childTrees) {
      LCRetain(childTrees);      
    } else {
      childTrees = LCDictionaryCreate(NULL, 0);
    }
    if (childDataSHAs) {
      LCRetain(childDataSHAs);
    } else {
      childDataSHAs = LCDictionaryCreate(NULL, 0);
    }
    newTree->childTrees = childTrees;
    newTree->childData = childDataSHAs;
  }
  return newTree;
};

LCDictionaryRef LCTreeChildTrees(LCTreeRef tree) {
  return tree->childTrees;
}

LCDictionaryRef LCTreeChildData(LCTreeRef tree) {
  return tree->childData;
}

LCStringRef LCTreeCreateSerializedString(LCTreeRef tree) {
  size_t bufferSize = childrenSerializationBufferSize(tree) + 1;
  char buffer[bufferSize];
  strcpy(buffer, "");
  
  // write child trees:
  serializeChildTrees(tree->childTrees, buffer);
  strcat(buffer, "\n");
  // write child data:
  serializeChildData(tree->childData, buffer);
  return LCStringCreate(buffer);
}

LCStringRef LCTreeSHA(LCTreeRef tree) {
  if(tree->sha == NULL) {
    LCStringRef serialized = LCTreeCreateSerializedString(tree);
    tree->sha = LCStringCreateSHAString(serialized);
    LCRelease(serialized);
  }
  return tree->sha;
}

LCTreeRef LCTreeCopy(LCTreeRef tree) {
  LCDictionaryRef childTreesCopy = LCDictionaryCopy(tree->childTrees);
  LCDictionaryRef childDataCopy = LCDictionaryCopy(tree->childData);
  LCTreeRef treeCopy = LCTreeCreate(childTreesCopy, childDataCopy);
  LCRelease(childTreesCopy);
  LCRelease(childDataCopy);
  return treeCopy;
}

LCTreeRef LCTreeChildTreeAtKey(LCTreeRef tree, LCStringRef key) {
  return LCDictionaryValueForKey(tree->childTrees, key);
}

LCStringRef LCTreeChildDataAtKey(LCTreeRef tree, LCStringRef key) {
  return LCDictionaryValueForKey(tree->childData, key);
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

LCTreeRef LCTreeCreateTreeUpdatingData(LCTreeRef oldTree, LCMutableArrayRef updatePathValues) {
  LCTreeRef newTree;
  if (oldTree) {
    newTree = LCTreeCopy(oldTree);    
  } else {
    newTree = LCTreeCreate(NULL, NULL);
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
  LCRelease(tree->childTrees);
  LCRelease(tree->childData);
}

// private
size_t childrenSerializationBufferSize(LCTreeRef tree) {
  size_t childrenLength = LCDictionaryLength(tree->childTrees) + LCDictionaryLength(tree->childData);
  size_t sumPathLength = 0;
  LCKeyValueRef* keyValues = LCDictionaryEntries(tree->childTrees);
  for (LCInteger i=0; i<LCDictionaryLength(tree->childTrees); i++) {
    sumPathLength = sumPathLength + LCStringLength(LCKeyValueKey(keyValues[i]));
  }
  keyValues = LCDictionaryEntries(tree->childData);
  for (LCInteger i=0; i<LCDictionaryLength(tree->childData); i++) {
    sumPathLength = sumPathLength + LCStringLength(LCKeyValueKey(keyValues[i]));
  }
  return sumPathLength + childrenLength * (LC_SHA1_HEX_Length + 2);
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
  LCDictionaryAddEntries(tree->childData, (LCKeyValueRef*)LCMutableArrayObjects(dataPathValues), LCMutableArrayLength(dataPathValues));
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
  LCTreeRef newChildTree = LCTreeCreateTreeUpdatingData(currentChildTree, updatePathArrays);
  LCDictionarySetValueForKey(parent->childTrees, key, newChildTree);
  LCRelease(newChildTree);
}
