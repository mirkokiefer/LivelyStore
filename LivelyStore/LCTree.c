
#include "LCCommitStage.h"

struct LCTree {
  LCObjectMeta meta;
  size_t childTreesLength;
  size_t childEntriesLength;
  LCSHARef sha;
  LCTreeRef* childTrees;
  LCKeyValueRef childEntries[];
};

void LCTreeDealloc(void* object);

LCTreeRef LCTreeCreate(LCTreeRef childTrees[], size_t childTreesLength, LCKeyValueRef childEntries[], size_t childEntriesLength) {
  LCTreeRef newTree = malloc(sizeof(struct LCTree)+sizeof(LCKeyValueRef)*childEntriesLength);
  if (newTree != NULL) {
    newTree->meta.dealloc = LCTreeDealloc;
    LCTreeRef* childTreesMemory = malloc(sizeof(LCTreeRef)*childTreesLength);
    if(childTreesMemory) {
      for (LCInteger i=0; i<childTreesLength; i++) {
        LCRetain(childTrees[i]);
      }
      for (LCInteger i=0; i<childEntriesLength; i++) {
        LCRetain(childEntries[i]);
      }
      memcpy(childTreesMemory, childTrees, childTreesLength*sizeof(LCTreeRef));
      newTree->childTrees = childTreesMemory;
      memcpy(newTree->childEntries, childEntries, childEntriesLength*sizeof(LCKeyValueRef));
    } else {
      free(newTree);
    }
  }
  return newTree;
};

size_t LCTreeChildTreesLength(LCTreeRef tree) {
  return tree->childTreesLength;
}

size_t LCTreeChildEntriesLength(LCTreeRef tree) {
  return tree->childEntriesLength;
}

LCTreeRef* LCTreeChildTrees(LCTreeRef tree) {
  return tree->childTrees;
}

LCKeyValueRef* LCTreeChildEntries(LCTreeRef tree) {
  return tree->childEntries;
}

LCSHARef LCTreeSHA1(LCTreeRef tree) {
  return NULL;
}

void LCTreeDealloc(void* object) {
  LCTreeRef tree = (LCTreeRef)object;
  for (LCInteger i=0; i<tree->childTreesLength; i++) {
    LCRelease(tree->childTrees[i]);
  }
  for (LCInteger i=0; i<tree->childEntriesLength; i++) {
    LCRelease(tree->childEntries[i]);
  }
  free(tree->childTrees);
}