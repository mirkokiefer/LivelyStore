
#include "LCStage.h"

void LCTreeDealloc(void* object);
size_t childrenSerializationBufferSize(LCTreeRef tree);
void serializeChildTrees(LCDictionaryRef childTrees, char buffer[]);
void serializeChildData(LCDictionaryRef childData, char buffer[]);
void serializePathSHA(LCStringRef path, LCStringRef sha, char buffer[]);
LCTreeRef LCTreeChildTreeAtPath(LCTreeRef tree, LCArrayRef pathArray);

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
    LCRetain(childTrees);
    LCRetain(childDataSHAs);
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

/*LCTreeRef LCTreeChildTreeAtPath(LCTreeRef tree, LCArrayRef pathArray) {
  LCStringRef subpath;
  LCTreeRef childTreeAtPath;
  for (LCInteger i=0; i<tree->childTreesLength; i++) {
    subpath = LCKeyValueKey(tree->children[i]);
    if(LCStringEqual(subpath, LCArrayObjectAtIndex(pathArray, 0))) {
      childTreeAtPath = LCKeyValueValue(tree->children[i]);
    }
  }
  if(childTreeAtPath) {
    if (LCArrayLength(pathArray) == 1) {
      return childTreeAtPath;
    } else {
      LCArrayRef subPathArray = LCArrayCreateSubArray(pathArray, 2, -1);
      childTreeAtPath = LCTreeChildTreeAtPath(childTreeAtPath, subPathArray);
      LCRelease(subPathArray);
      return childTreeAtPath;
    }
  } else {
    return NULL;
  }
}*/


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
  return LCTreeCreate(LCDictionaryCopy(tree->childTrees), LCDictionaryCopy(tree->childData));
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
