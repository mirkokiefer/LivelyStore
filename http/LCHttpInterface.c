
#include "LCHttpInterface.h"
#include "mongoose.h"


typedef struct httpInterfaceData* httpInterfaceDataRef;

struct httpInterfaceData {
  struct mg_context *mg_context;
  LCMutableDictionaryRef registeredRepos;
};

struct LCType typeHttpInterface = {
  .name = "LCHttpInterface",
  .immutable = true,
};

LCTypeRef LCTypeHttpInterface = &typeHttpInterface;

static httpInterfaceDataRef httpInterfaceData(LCHttpInterfaceRef http) {
  return objectData(http);
}

static httpInterfaceDataRef httpInterfaceInit() {
  httpInterfaceDataRef data = malloc(sizeof(struct httpInterfaceData));
  if (data) {
    data->mg_context = NULL;
    data->registeredRepos = LCMutableDictionaryCreate(NULL, 0);
  }
  return data;
}

LCHttpInterfaceRef LCHttpInterfaceCreate() {
  httpInterfaceDataRef data = httpInterfaceInit();
  return objectCreate(LCTypeHttpInterface, data);
}

static void *mg_callback(enum mg_event event,
                         struct mg_connection *conn,
                         const struct mg_request_info *request_info) {
  if (event == MG_NEW_REQUEST) {
    // Echo requested URI back to the client
    mg_printf(conn, "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/plain\r\n\r\n"
              "%s", request_info->uri);
    return "";  // Mark as processed
  } else {
    return NULL;
  }
}

void LCHttpInterfaceStart(LCHttpInterfaceRef http, char *port) {
  const char *options[] = {
    "listening_ports", port,
    "ssl_certificate", "resources/ssl_cert.pem", NULL
  };
  httpInterfaceData(http)->mg_context = mg_start(&mg_callback, NULL, options);
}

void LCHttpInterfaceStop(LCHttpInterfaceRef http) {
  mg_stop(httpInterfaceData(http)->mg_context);
}

void LCHttpInterfaceRegisterRepository(LCHttpInterfaceRef http, LCRepositoryRef repo, char *name) {
  LCStringRef nameString = LCStringCreate(name);
  LCMutableDictionarySetValueForKey(httpInterfaceData(http)->registeredRepos, nameString, repo);
  objectRelease(nameString);
}

void LCHttpInterfaceSendPullRequest(LCHttpInterfaceRef http, char *remoteRepo, char *localRepoName, char commit[HASH_LENGTH]) {
  
}

void LCHttpInterfaceLoadData(LCHttpInterfaceRef http, char *remoteRepo, char fromCommit[HASH_LENGTH], 
                             char toCommit[HASH_LENGTH], LCStoreRef store) {
  
}


