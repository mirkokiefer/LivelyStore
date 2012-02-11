
#include "LCCommitStage.h"

struct LCTree {
  LCObjectMeta meta;
  LCSHARef sha;
  LCStringRef name;
  size_t childTreesLength;
  size_t childEntriesLength;
  LCTreeRef* childTrees;
  LCKeyValueRef childEntries[];
};

void LCTreeDealloc(void* object);

LCTreeRef LCTreeCreate(LCStringRef name, LCTreeRef childTrees[], size_t childTreesLength,
                       LCKeyValueRef childEntries[], size_t childEntriesLength) {
  LCTreeRef newTree = malloc(sizeof(struct LCTree)+sizeof(LCKeyValueRef)*childEntriesLength);
  if (newTree != NULL) {
    newTree->meta.dealloc = LCTreeDealloc;
    LCTreeRef* childTreesMemory = malloc(sizeof(LCTreeRef)*childTreesLength);
    if(childTreesMemory) {
      LCRetain(name);
      newTree->name = name;
      for (LCInteger i=0; i<childTreesLength; i++) {
        LCRetain(childTrees[i]);
      }
      for (LCInteger i=0; i<childEntriesLength; i++) {
        LCRetain(childEntries[i]);
      }
      memcpy(childTreesMemory, childTrees, childTreesLength*sizeof(LCTreeRef));
      newTree->childTrees = childTreesMemory;
      memcpy(newTree->childEntries, childEntries, childEntriesLength*sizeof(LCKeyValueRef));
      newTree->childTreesLength = childTreesLength;
      newTree->childEntriesLength = childEntriesLength;
    } else {
      free(newTree);
    }
  }
  return newTree;
};

LCStringRef LCTreeName(LCTreeRef tree) {
  return tree->name;
}

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
  if(tree->sha == NULL) {
    size_t blobCount = tree->childTreesLength*2+tree->childEntriesLength*2;
    LCBlobRef blobs[blobCount];
    LCInteger blobIndex = 0;
    for (LCInteger i=0; i<tree->childTreesLength; i++) {
      LCBlobRef treeName = LCStringCreateBlob(LCTreeName(tree->childTrees[i]));
      LCBlobRef treeSHA = LCSHABlob(LCTreeSHA1(tree->childTrees[i]));
      blobs[blobIndex] = treeName;
      blobs[blobIndex+1] = treeSHA;
      blobIndex = blobIndex+2;
    }
    for (LCInteger i=0; i<tree->childEntriesLength; i++) {
      LCBlobRef key = LCStringCreateBlob(LCKeyValueKey(tree->childEntries[i]));
      LCBlobRef value = LCKeyValueValue((tree->childEntries[i]));
      LCBlobRef valueSHA = LCSHABlob(LCBlobSHA1(value));
      blobs[blobIndex] = key;
      blobs[blobIndex+1] = valueSHA;
      blobIndex = blobIndex+2;
    }
    tree->sha = LCSHACreate(blobs, blobCount);
  };
  return tree->sha;
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