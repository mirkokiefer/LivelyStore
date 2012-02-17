
#include "LCStage.h"

void LCTreeDealloc(void* object);
size_t childrenSerializationBufferSize(LCTreeRef tree);
void serializeChildren(LCPathDataSHARef children[], size_t length, char buffer[]);

struct LCTree {
  LCObjectInfo info;
  LCStringRef sha;
  size_t childTreesLength;
  size_t childDataLength;
  LCPathDataSHARef children[];
};

LCType typeTree = {
  .dealloc = LCTreeDealloc,
};

LCTreeRef LCTreeCreate(LCPathDataSHARef childTrees[], size_t childTreesLength,
                       LCPathDataSHARef childData[], size_t childDataLength) {
  LCTreeRef newTree = malloc(sizeof(struct LCTree)+sizeof(LCPathDataSHARef)*(childDataLength+childTreesLength));
  if (newTree != NULL) {
    newTree->info.type = &typeTree;
    for (LCInteger i=0; i<childTreesLength; i++) {
      LCRetain(childTrees[i]);
    }
    for (LCInteger i=0; i<childDataLength; i++) {
      LCRetain(childData[i]);
    }
    memcpy(newTree->children, childTrees, childTreesLength*sizeof(LCPathDataSHARef));
    memcpy(&(newTree->children[childTreesLength]), childData, childDataLength*sizeof(LCPathDataSHARef));
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

LCPathDataSHARef* LCTreeChildTrees(LCTreeRef tree) {
  return tree->children;
}

LCPathDataSHARef* LCTreeChildData(LCTreeRef tree) {
  return &(tree->children[tree->childTreesLength]);
}

size_t childrenSerializationBufferSize(LCTreeRef tree) {
  size_t childrenLength = tree->childTreesLength + tree->childDataLength;
  size_t sumPathLength = 0;
  for (LCInteger i=0; i<childrenLength; i++) {
    sumPathLength = sumPathLength + LCStringLength(LCPathDataSHAPath(tree->children[i]));
  }
  return sumPathLength + childrenLength * (LC_SHA1_HEX_Length + 2);  
}

void serializeChildren(LCPathDataSHARef children[], size_t length, char buffer[]) {
  char* path;
  char* sha;
  LCPathDataSHARef dataSHA;
  for (LCInteger i=0; i<length; i++) {
    dataSHA = children[i];
    path = LCStringStringRef(LCPathDataSHAPath(dataSHA));
    sha = LCStringStringRef(LCPathDataSHASHA(dataSHA));
    strcat(buffer, path);
    strcat(buffer, " ");
    strcat(buffer, sha);
    strcat(buffer, "\n");
  }
}

LCStringRef LCTreeCreateSerializedString(LCTreeRef tree) {
  size_t bufferSize = childrenSerializationBufferSize(tree) + 1;
  char buffer[bufferSize];
  strcpy(buffer, "");
  
  // write child trees:
  serializeChildren(tree->children, tree->childTreesLength, buffer);
  strcat(buffer, "\n");
  // write child data:
  serializeChildren(&(tree->children[tree->childTreesLength]), tree->childDataLength, buffer);
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