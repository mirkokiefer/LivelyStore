

#include "LCPeerClient.h"

typedef struct peerClientData* peerClientDataRef;

void peerClientDealloc(LCObjectRef object);

struct peerClientData {
  LCStringRef address;
  LCStringRef login;
  LCStringRef password;
  LCContextRef context;
  CURL *curl_handle;
};

struct LCType typePeerClient = {
  .name = "LCPeerClient",
  .immutable = true,
  .dealloc = peerClientDealloc
};

LCTypeRef LCTypePeerClient = &typePeerClient;

static peerClientDataRef peerClientData(LCPeerClient client) {
  return objectData(client);
}

static peerClientDataRef peerClientInit() {
  peerClientDataRef data = malloc(sizeof(struct peerClientData));
  return data;
}

static LCStringRef peerClientCreateUrlForPath(LCPeerClient client, char *path, char *parameters) {
  peerClientDataRef data = peerClientData(client);
  if (parameters) {
    char* strings[] = {LCStringChars(data->address), path, "?user=", LCStringChars(data->login),
      "&password=", LCStringChars(data->password), "&", parameters};
    return LCStringCreateFromStringArray(strings, 8);
  } else {
    char* strings[] = {LCStringChars(data->address), path, "?user=", LCStringChars(data->login),
      "&password=", LCStringChars(data->password)};
    return LCStringCreateFromStringArray(strings, 6);
  }
}

LCPeerClient LCPeerClientCreate(char *address, char *login, char *password, LCContextRef context) {
  peerClientDataRef data = peerClientInit();
  data->address = LCStringCreate(address);
  data->login = LCStringCreate(login);
  data->password = LCStringCreate(password);
  data->context = context;
  data->curl_handle = curl_easy_init();
  return objectCreate(LCTypePeerClient, data);
}

void LCPeerClientPush(LCPeerClient client, LCCommitRef commit) {
  peerClientDataRef data = peerClientData(client);
  char hash[HASH_LENGTH];
  objectHash(commit, hash);
  FILE *readFile = createMemoryReadStream(NULL, (LCByte*)hash, HASH_LENGTH, false, NULL);

  LCMutableDataRef response = LCMutableDataCreate(NULL, 0);
  FILE *responseFile = createMemoryWriteStream(response, LCMutableDataAppendAlt, NULL);

  LCStringRef url = peerClientCreateUrlForPath(client, "head", NULL);
  curl_easy_setopt(data->curl_handle, CURLOPT_URL, LCStringChars(url));
  curl_easy_setopt(data->curl_handle, CURLOPT_WRITEDATA, responseFile);
  curl_easy_setopt(data->curl_handle, CURLOPT_READDATA, readFile);
  curl_easy_setopt(data->curl_handle, CURLOPT_INFILESIZE_LARGE, sizeof(char*)*(strlen(hash)+1));
  curl_easy_setopt(data->curl_handle, CURLOPT_UPLOAD, 1L);
  curl_easy_perform(data->curl_handle);
  objectRelease(response);
  objectRelease(url);
}

static LCArrayRef peerClientPullMetaDataCreateArrayFromResponse(LCPeerClient client, char *data) {
  FILE *fd = createMemoryReadStream(NULL, (LCByte*)data, strlen(data)+1, false, NULL);
  LCContextRef context = peerClientData(client)->context;
  return objectCreateFromFile(context, fd);
}

void LCPeerClientPullMetaData(LCPeerClient client, char fromCommit[HASH_LENGTH], char toCommit[HASH_LENGTH]) {
  peerClientDataRef data = peerClientData(client);
  LCMutableDataRef response = LCMutableDataCreate(NULL, 0);
  FILE *responseFile = createMemoryWriteStream(response, LCMutableDataAppendAlt, NULL);
  
  char *parameters[] = {"from=", fromCommit, "&to=", toCommit};
  LCStringRef parameterString = LCStringCreateFromStringArray(parameters, 4);
  LCStringRef url = peerClientCreateUrlForPath(client, "commits", LCStringChars(parameterString));
  curl_easy_setopt(data->curl_handle, CURLOPT_URL, LCStringChars(url));
  curl_easy_setopt(data->curl_handle, CURLOPT_WRITEDATA, responseFile);
  curl_easy_perform(data->curl_handle);
  
  LCArrayRef array = peerClientPullMetaDataCreateArrayFromResponse(client, (char*)LCMutableDataDataRef(response));
  for (LCInteger i=0; i<LCArrayLength(array); i++) {
    objectStore(LCArrayObjectAtIndex(array, i), data->context);
  }
  objectRelease(array);

  objectRelease(response);
  objectRelease(parameterString);
  objectRelease(url);
}

static void peerClientPullDataParseResponse(LCPeerClient client, LCByte *data, size_t length) {
  
}

static void peerClientPullData(LCPeerClient client, char dataHash[HASH_LENGTH]) {
  peerClientDataRef data = peerClientData(client);
  char *dataPath = "data/";
  char path[strlen(dataPath)+HASH_LENGTH];
  strcpy(path, dataPath);
  strcat(path, dataHash);
  LCStringRef url = peerClientCreateUrlForPath(client, path, NULL);
  
  LCMutableDataRef response = LCMutableDataCreate(NULL, 0);
  FILE *responseFile = createMemoryWriteStream(response, LCMutableDataAppendAlt, NULL);

  curl_easy_setopt(data->curl_handle, CURLOPT_URL, LCStringChars(url));
  curl_easy_setopt(data->curl_handle, CURLOPT_WRITEDATA, responseFile);
  curl_easy_perform(data->curl_handle);

  peerClientPullDataParseResponse(client, LCMutableDataDataRef(response), LCMutableDataLength(response));
}

void LCPeerClientPullData(LCPeerClient client, char* dataHashs[], size_t length) {
  for (LCInteger i=0; i<length; i++) {
    peerClientPullData(client, dataHashs[i]);
  }
}

void peerClientDealloc(LCPeerClient client) {
  peerClientDataRef data = peerClientData(client);
  objectRelease(data->address);
  objectRelease(data->login);
  objectRelease(data->password);
  curl_easy_cleanup(data->curl_handle);
}

void LCHttpGlobalInit() {
  curl_global_init(CURL_GLOBAL_ALL);
}

void LCHttpGlobalCleanup() {
  curl_global_cleanup();
}