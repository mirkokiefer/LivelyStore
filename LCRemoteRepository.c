
#include "LCRemoteRepository.h"

typedef struct remoteRepoData* remoteRepoDataRef;

static void* remoteRepoInit();

struct remoteRepoData {
  LCStringRef url;
  LCStringRef login;
  LCStringRef password;
};

void remoteRepoDealloc(LCObjectRef object);

struct LCType typeRemoteRepo = {
  .name = "LCRemoteRepository",
  .immutable = true,
  .initData = remoteRepoInit,
  .dealloc = remoteRepoDealloc
};

LCTypeRef LCTypeRemoteRepository = &typeRemoteRepo;

static void* remoteRepoInit() {
  remoteRepoDataRef data = malloc(sizeof(struct remoteRepoData));
  if (data) {
    data->url = NULL;
    data->login = NULL;
    data->password = NULL;
  }
  return data;
}

static remoteRepoDataRef remoteRepoData(LCRemoteRepositoryRef repo) {
  return objectData(repo);
}

LCRemoteRepositoryRef LCRemoteRepositoryCreate(char *url, char *login, char *password) {
  remoteRepoDataRef data = remoteRepoInit();
  data->url = LCStringCreate(url);
  data->login = LCStringCreate(login);
  data->password = LCStringCreate(password);
  return objectCreate(LCTypeRemoteRepository, data);
}

char* LCRemoteRepositoryUrl(LCRemoteRepositoryRef repo) {
  return LCStringChars(remoteRepoData(repo)->url);
}

char* LCRemoteRepositoryLogin(LCRemoteRepositoryRef repo) {
  return LCStringChars(remoteRepoData(repo)->login);
}

char* LCRemoteRepositoryPassword(LCRemoteRepositoryRef repo) {
  return LCStringChars(remoteRepoData(repo)->password);
}

int LCRemoteRepositoryURLForPath(LCRemoteRepositoryRef repo, char *path, char buffer[], size_t length) {
  strcpy(buffer, LCRemoteRepositoryUrl(repo));
  strcat(buffer, path);
  strcat(buffer, "?user=");
  strcat(buffer, LCRemoteRepositoryLogin(repo));
  strcat(buffer, "&password=");
  strcat(buffer, LCRemoteRepositoryPassword(repo));
  return 0;
}

void remoteRepoDealloc(LCRemoteRepositoryRef repo) {
  remoteRepoDataRef data = remoteRepoData(repo);
  objectRelease(data->url);
  objectRelease(data->login);
  objectRelease(data->password);
}