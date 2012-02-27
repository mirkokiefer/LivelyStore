
#include "LCStage.h"

void LCTreeDealloc(void* object);
size_t childrenSerializationBufferSize(LCTreeRef tree);
void serializeChildTrees(LCMutableArrayRef childTrees, char buffer[]);
void serializeChildData(LCMutableArrayRef childData, char buffer[]);
void serializePathSHA(LCStringRef path, LCStringRef sha, char buffer[]);
LCTreeRef LCTreeChildTreeAtPath(LCTreeRef tree, LCArrayRef pathArray);

struct LCTree {
  LCObjectInfo info;
  LCStringRef sha;
  LCMutableArrayRef childTrees;
  LCMutableArrayRef childData;
};

LCType typeTree = {
  .dealloc = LCTreeDealloc
};

LCTreeRef LCTreeCreate(LCArrayRef childTrees, LCArrayRef childDataSHAs) {
  LCTreeRef newTree = malloc(sizeof(struct LCTree));
  if (newTree != NULL) {
    newTree->info.type = &typeTree;
    newTree->childTrees = LCArrayCreateMutableArray(childTrees);
    newTree->childData = LCArrayCreateMutableArray(childDataSHAs);
  }
  return newTree;
};

LCMutableArrayRef LCTreeChildTrees(LCTreeRef tree) {
  return tree->childTrees;
}

LCMutableArrayRef LCTreeChildData(LCTreeRef tree) {
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

void LCTreeDealloc(void* object) {
  LCTreeRef tree = (LCTreeRef)object;
  LCRelease(tree->childTrees);
  LCRelease(tree->childData);
}

// private
size_t childrenSerializationBufferSize(LCTreeRef tree) {
  size_t childrenLength = LCMutableArrayLength(tree->childTrees) + LCMutableArrayLength(tree->childData);
  size_t sumPathLength = 0;
  for (LCInteger i=0; i<LCMutableArrayLength(tree->childTrees); i++) {
    sumPathLength = sumPathLength + LCStringLength(LCKeyValueKey(LCMutableArrayObjectAtIndex(tree->childTrees, i)));
  }
  for (LCInteger i=0; i<LCMutableArrayLength(tree->childData); i++) {
    sumPathLength = sumPathLength + LCStringLength(LCKeyValueKey(LCMutableArrayObjectAtIndex(tree->childData, i)));
  }
  return sumPathLength + childrenLength * (LC_SHA1_HEX_Length + 2);  
}

void serializeChildTrees(LCMutableArrayRef childTrees, char buffer[]) {
  LCKeyValueRef childKeyTree;
  LCTreeRef childTree;
  for (LCInteger i=0; i<LCMutableArrayLength(childTrees); i++) {
    childKeyTree = LCMutableArrayObjectAtIndex(childTrees, i);
    childTree = LCKeyValueValue(childKeyTree);
    serializePathSHA(LCKeyValueKey(childKeyTree), LCTreeSHA(childTree), buffer);
  }
}

void serializeChildData(LCMutableArrayRef childData, char buffer[]) {
  LCKeyValueRef childKeyData;
  for (LCInteger i=0; i<LCMutableArrayLength(childData); i++) {
    childKeyData = LCMutableArrayObjectAtIndex(childData, i);
    serializePathSHA(LCKeyValueKey(childKeyData), LCKeyValueValue(childKeyData), buffer);
  }
}

void serializePathSHA(LCStringRef path, LCStringRef sha, char buffer[]) {
  strcat(buffer, LCStringStringRef(path));
  strcat(buffer, " ");
  strcat(buffer, LCStringStringRef(sha));
  strcat(buffer, "\n");
}
