
#include "LCCommitStage.h"

void LCTreeBlobs(void* blob, LCBlobRef* buffer);
size_t LCTreeBlobCount(void* blob);
void LCTreeDealloc(void* object);

struct LCTree {
  LCObjectInfo info;
  LCSHARef sha;
  size_t childTreesLength;
  size_t childBlobsLength;
  LCKeyValueSHARef children[];
};

LCHashableObject hashableTree = {
  .blobs = LCTreeBlobs,
  .blobCount = LCTreeBlobCount
};

LCType typeTree = {
  .dealloc = LCTreeDealloc,
  .meta = &hashableTree
};

LCTreeRef LCTreeCreate(LCKeyValueSHARef childTrees[], size_t childTreesLength,
                       LCKeyValueSHARef childBlobs[], size_t childBlobsLength) {
  LCTreeRef newTree = malloc(sizeof(struct LCTree)+sizeof(LCKeyValueSHARef)*(childBlobsLength+childTreesLength));
  if (newTree != NULL) {
    newTree->info.type = &typeTree;
    for (LCInteger i=0; i<childTreesLength; i++) {
      LCRetain(childTrees[i]);
    }
    for (LCInteger i=0; i<childBlobsLength; i++) {
      LCRetain(childBlobs[i]);
    }
    memcpy(newTree->children, childTrees, childTreesLength*sizeof(LCKeyValueSHARef));
    memcpy(&(newTree->children[childTreesLength]), childBlobs, childBlobsLength*sizeof(LCKeyValueSHARef));
    newTree->childTreesLength = childTreesLength;
    newTree->childBlobsLength = childBlobsLength;
  }
  return newTree;
};

size_t LCTreeChildTreesLength(LCTreeRef tree) {
  return tree->childTreesLength;
}

size_t LCTreeChildBlobsLength(LCTreeRef tree) {
  return tree->childBlobsLength;
}

LCKeyValueSHARef* LCTreeChildTrees(LCTreeRef tree) {
  return tree->children;
}

LCKeyValueSHARef* LCTreeChildBlobs(LCTreeRef tree) {
  return &(tree->children[tree->childTreesLength]);
}

size_t LCTreeBlobCount(void* blob) {
  LCTreeRef tree = (LCTreeRef)blob;
  return tree->childTreesLength*2 + tree->childBlobsLength*2;
}

void LCTreeBlobs(void* blob, LCBlobRef* buffer) {
  LCTreeRef tree = (LCTreeRef)blob;
  size_t blobCount = LCTreeBlobCount(blob);
  LCInteger bufferIndex = 0;
  for (LCInteger i=0; i<blobCount/2; i++) {
    LCBlobRef key = LCStringCreateBlob(LCKeyValueSHAKey(tree->children[i]));
    LCBlobRef value = LCSHASHABlob(LCKeyValueSHAValue(tree->children[i]));
    buffer[bufferIndex] = key;
    buffer[bufferIndex+1] = value;
    bufferIndex = bufferIndex+2;
  }
}

LCSHARef LCTreeSHA(LCTreeRef tree) {
  if(tree->sha == NULL) {
    tree->sha = LCSHACreateFromHashableObject(tree);
  }
  return tree->sha;
}

void LCTreeDealloc(void* object) {
  LCTreeRef tree = (LCTreeRef)object;
  for (LCInteger i=0; i<(tree->childTreesLength+tree->childBlobsLength); i++) {
    LCRelease(tree->children[i]);
  }
}