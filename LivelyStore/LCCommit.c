
#include "LCCommit.h"

struct LCCommit {
  LCObjectMeta meta;
  size_t length;
  LCKeyValueRef keyValues[];
};

void LCCommitDealloc(void* object);

LCCommitRef LCCommitCreate() {
  LCCommitRef newCommit = malloc(sizeof(struct LCCommit));
  if (newCommit != NULL) {
    newCommit->meta.dealloc = LCCommitDealloc;
  }
  return newCommit;
};

void LCCommitAddEntry(LCCommitRef commit, LCKeyValueRef keyValue) {
  realloc(commit, sizeof(struct LCCommit) + (commit->length+1)*sizeof(keyValue));
  commit->keyValues[commit->length] = keyValue;
  commit->length = commit->length + 1;
}

void LCCommitDealloc(void* object) {

}