
#include "LCStage.h"

void LCTreeDealloc(void* object);
size_t childrenSerializationBufferSize(LCTreeRef tree);
void serializeChildTrees(LCKeyValueRef children[], size_t length, char buffer[]);
void serializeChildData(LCKeyValueRef children[], size_t length, char buffer[]);
void serializePathSHA(LCStringRef path, LCStringRef sha, char buffer[]);

struct LCTree {
  LCObjectInfo info;
  LCStringRef sha;
  size_t childTreesLength;
  size_t childDataLength;
  LCKeyValueRef children[];
};

LCType typeTree = {
  .dealloc = LCTreeDealloc
};

LCTreeRef LCTreeCreate(LCKeyValueRef childTrees[], size_t childTreesLength,
                       LCKeyValueRef childData[], size_t childDataLength) {
  LCTreeRef newTree = malloc(sizeof(struct LCTree)+sizeof(LCKeyValueRef)*(childDataLength+childTreesLength));
  if (newTree != NULL) {
    newTree->info.type = &typeTree;
    for (LCInteger i=0; i<childTreesLength; i++) {
      LCRetain(childTrees[i]);
    }
    for (LCInteger i=0; i<childDataLength; i++) {
      LCRetain(childData[i]);
    }
    memcpy(newTree->children, childTrees, childTreesLength*sizeof(LCKeyValueRef));
    memcpy(&(newTree->children[childTreesLength]), childData, childDataLength*sizeof(LCKeyValueRef));
    newTree->childTreesLength = childTreesLength;
    newTree->childDataLength = childDataLength;
  }
  return newTree;
};

size_t LCTreeChildTreesLength(LCTreeRef tree) {
  return tree->childTreesLength;
}

size_t LCTreeChildDataLength(LCTreeRef tree) {
  return tree->childDataLength;
}

LCKeyValueRef* LCTreeChildTrees(LCTreeRef tree) {
  return tree->children;
}

LCKeyValueRef* LCTreeChildData(LCTreeRef tree) {
  return &(tree->children[tree->childTreesLength]);
}

size_t childrenSerializationBufferSize(LCTreeRef tree) {
  size_t childrenLength = tree->childTreesLength + tree->childDataLength;
  size_t sumPathLength = 0;
  for (LCInteger i=0; i<childrenLength; i++) {
    sumPathLength = sumPathLength + LCStringLength(LCKeyValueKey(tree->children[i]));
  }
  return sumPathLength + childrenLength * (LC_SHA1_HEX_Length + 2);  
}

void serializeChildTrees(LCKeyValueRef children[], size_t length, char buffer[]) {
  LCTreeRef childTree;
  for (LCInteger i=0; i<length; i++) {
    childTree = LCKeyValueValue(children[i]);
    serializePathSHA(LCKeyValueKey(children[i]), LCTreeSHA(childTree), buffer);
  }
}

void serializeChildData(LCKeyValueRef children[], size_t length, char buffer[]) {
  for (LCInteger i=0; i<length; i++) {
    serializePathSHA(LCKeyValueKey(children[i]), LCKeyValueValue(children[i]), buffer);
  }
}

void serializePathSHA(LCStringRef path, LCStringRef sha, char buffer[]) {
  strcat(buffer, LCStringStringRef(path));
  strcat(buffer, " ");
  strcat(buffer, LCStringStringRef(sha));
  strcat(buffer, "\n");
}

LCStringRef LCTreeCreateSerializedString(LCTreeRef tree) {
  size_t bufferSize = childrenSerializationBufferSize(tree) + 1;
  char buffer[bufferSize];
  strcpy(buffer, "");
  
  // write child trees:
  serializeChildTrees(tree->children, tree->childTreesLength, buffer);
  strcat(buffer, "\n");
  // write child data:
  serializeChildData(&(tree->children[tree->childTreesLength]), tree->childDataLength, buffer);
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
  for (LCInteger i=0; i<(tree->childTreesLength+tree->childDataLength); i++) {
    LCRelease(tree->children[i]);
  }
}