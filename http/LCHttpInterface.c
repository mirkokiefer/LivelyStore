
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

LCHttpInterfaceRef LCHttpInterfaceCreate(authorizeFun authorizationHandler) {
  httpInterfaceDataRef data = httpInterfaceInit();
  return objectCreate(LCTypeHttpInterface, data);
}

static void http_repo_action(char *repo, char *action, struct mg_connection *conn, const struct mg_request_info *request_info) {
  mg_printf(conn, "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "%s", "todo");
}

static void http_test_request(struct mg_connection *conn) {
  mg_printf(conn, "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "%s", "/test");
}

static void *mg_callback(enum mg_event event, struct mg_connection *conn, const struct mg_request_info *request_info) {
  if (event == MG_NEW_REQUEST) {
    LCStringRef uri = LCStringCreate(request_info->uri);
    LCArrayRef uriComps = createPathArray(uri);
    if (LCArrayLength(uriComps) < 3) {
      http_test_request(conn);
    } else {
      char *repo = LCStringChars(LCArrayObjectAtIndex(uriComps, 1));
      char *action = LCStringChars(LCArrayObjectAtIndex(uriComps, 2));
      http_repo_action(repo, action, conn, request_info);
    }
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

void LCHttpInterfaceSendNewHead(LCHttpInterfaceRef http, LCRemoteRepositoryRef remote, LCRepositoryRef localRepo, char commit[HASH_LENGTH]) {
  
}

void LCHttpInterfacePullMetaData(LCHttpInterfaceRef http, LCRemoteRepositoryRef remote, char fromCommit[HASH_LENGTH],
                                   char toCommit[HASH_LENGTH], LCStoreRef store);
void LCHttpInterfacePullData(LCHttpInterfaceRef http, LCRemoteRepositoryRef remote, char* data[], LCStoreRef store);



