
#include "LCFileStoreBackend.h"
#include <sys/stat.h>

typedef struct LCFileStore* LCFileStoreRef;

#define LCDataFolder "data/"
#define LCCommitFolder "commits/"
#define LCTreeFolder "trees/"

#define LCFileEnding ".txt"
#define LCFileNameLength (LC_SHA1_HEX_Length + 4)

#define LCReadBufferSize 1024

void fileStoreStoreData(void* store, LCDataType type, char* sha, unsigned char data[], size_t length);
void fileStoreDeleteData(void* store, LCDataType type, char* sha);
size_t fileStoreGetDataLength(void* store, LCDataType type, char* sha);
void fileStoreGetData(void* store, LCDataType type, char* sha, unsigned char buffer[]);
LCStringRef createFilePath(LCFileStoreRef store, LCDataType type, char* filename);

struct LCFileStore {
  LCObjectInfo info;
  LCStringRef location;
  LCStringRef commitLocation;
  LCStringRef treeLocation;
  LCStringRef dataLocation;
};

static void LCFileStoreDealloc(void* object);

LCType typeFileStore = {
  .dealloc = LCFileStoreDealloc
};

static LCFileStoreRef LCFileStoreCreate(LCStringRef location) {
  LCFileStoreRef newStore = LCNewObject(&typeFileStore, sizeof(struct LCFileStore));
  
  LCStringRef commitFolder = LCStringCreate(LCCommitFolder);
  LCStringRef treeFolder = LCStringCreate(LCTreeFolder);
  LCStringRef dataFolder = LCStringCreate(LCDataFolder);
  LCStringRef commitDir[] = {location, commitFolder};
  LCStringRef treeDir[] = {location, treeFolder};
  LCStringRef dataDir[] = {location, dataFolder};
  
  newStore->location = LCRetain(location);
  newStore->commitLocation = LCStringCreateFromStrings(commitDir, 2);
  newStore->treeLocation = LCStringCreateFromStrings(treeDir, 2);
  newStore->dataLocation = LCStringCreateFromStrings(dataDir, 2);
  makeDirectory(LCStringStringRef(newStore->location));
  makeDirectory(LCStringStringRef(newStore->commitLocation));
  makeDirectory(LCStringStringRef(newStore->treeLocation));
  makeDirectory(LCStringStringRef(newStore->dataLocation));

  LCRelease(dataFolder);
  LCRelease(commitFolder);
  LCRelease(treeFolder);
  return newStore;
};

static void LCFileStoreDealloc(void* object) {
  LCFileStoreRef store = (LCFileStoreRef)object;
  LCRelease(store->location);
  LCRelease(store->commitLocation);
  LCRelease(store->treeLocation);
  LCRelease(store->dataLocation);
}

struct LCStoreBackend* createLCFileStoreBackend(char* location) {
  struct LCStoreBackend* backend = malloc(sizeof(struct LCStoreBackend));
  if (backend != NULL) {
    LCStringRef locationObj = LCStringCreate(location);
    backend->storeObject = LCFileStoreCreate(locationObj);
    LCRelease(locationObj);
    backend->storeCb = fileStoreStoreData;
    backend->deleteCb = fileStoreDeleteData;
    backend->getLengthCb = fileStoreGetDataLength;
    backend->getCb = fileStoreGetData;
  }
  return backend;
}

void freeLCFileStoreBackend(struct LCStoreBackend* backend) {
  LCRelease(backend->storeObject);
  free(backend);
}

void fileStoreStoreData(void* storeObj, LCDataType type, char* sha, unsigned char data[], size_t length) {
  LCFileStoreRef store = (LCFileStoreRef)storeObj;
  LCStringRef filePath = createFilePath(store, type, sha);
  writeToFile(data, length, LCStringStringRef(filePath));
  LCRelease(filePath);
}

void fileStoreDeleteData(void* storeObj, LCDataType type, char* sha) {

}

size_t fileStoreGetDataLength(void* storeObj, LCDataType type, char* sha) {
  LCFileStoreRef store = (LCFileStoreRef)storeObj;
  LCStringRef filePath = createFilePath(store, type, sha);
  struct stat st;
  stat(LCStringStringRef(filePath), &st);
  size_t size = st.st_size;
  LCRelease(filePath);
  return size;
}

void fileStoreGetData(void* storeObj, LCDataType type, char* sha, unsigned char buffer[]) {
  LCFileStoreRef store = (LCFileStoreRef)storeObj;
  LCStringRef filePath = createFilePath(store, type, sha);
  FILE *fp = fopen(LCStringStringRef(filePath), "r");
  size_t size = 0;
  size_t lastRead;
  while((lastRead = fread(buffer, sizeof(unsigned char), LCReadBufferSize, fp)) > 0) {
    size = size + lastRead;
  }
  fclose(fp);
}

LCStringRef createFilePath(LCFileStoreRef store, LCDataType type, char* filename) {
  LCStringRef directory;
  switch (type) {
    case LCData:
      directory = store->dataLocation;
      break;
    case LCCommit:
      directory = store->commitLocation;
      break;
    case LCTree:
      directory = store->treeLocation;
      break;
    default:
      break;
  }
  char buffer[LCStringLength(directory)+LCFileNameLength];
  buffer[0] = '\0';
  strcat(buffer, LCStringStringRef(directory));
  strcat(buffer, filename);
  strcat(buffer, LCFileEnding);
  return LCStringCreate(buffer);
}