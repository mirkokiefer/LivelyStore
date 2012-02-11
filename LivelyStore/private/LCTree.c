
#include "LCCommitStage.h"

struct LCTree {
  LCObjectInfo info;
  LCSHARef sha;
  size_t childTreesLength;
  size_t childBlobsLength;
  LCKeyValueSHARef children[];
};

void LCTreeDealloc(void* object);

LCType typeTree = {
  .dealloc = LCTreeDealloc
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

LCSHARef LCTreeSHA1(LCTreeRef tree) {
  if(tree->sha == NULL) {
    size_t blobCount = tree->childTreesLength*2 + tree->childBlobsLength*2;
    LCBlobRef blobs[blobCount];
    LCInteger blobIndex = 0;
    for (LCInteger i=0; i<blobCount/2; i++) {
      LCBlobRef key = LCStringCreateBlob(LCKeyValueSHAKey(tree->children[i]));
      LCBlobRef value = LCSHABlob(LCKeyValueSHAValue(tree->children[i]));
      blobs[blobIndex] = key;
      blobs[blobIndex+1] = value;
      blobIndex = blobIndex+2;
    }
    tree->sha = LCSHACreate(blobs, blobCount);
  };
  return tree->sha;
}

void LCTreeDealloc(void* object) {
  LCTreeRef tree = (LCTreeRef)object;
  for (LCInteger i=0; i<(tree->childTreesLength+tree->childBlobsLength); i++) {
    LCRelease(tree->children[i]);
  }
}