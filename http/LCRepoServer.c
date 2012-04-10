
#include "LCRepoServer.h"
#include "mongoose.h"


typedef struct repoServerData* repoServerDataRef;

struct repoServerData {
  struct mg_context *mg_context;
  LCMutableDictionaryRef registeredRepos;
};

struct LCType typeRepoServer = {
  .name = "LCRepoServer",
  .immutable = true,
};

LCTypeRef LCTypeRepoServer = &typeRepoServer;

static repoServerDataRef repoServerData(LCRepoServer http) {
  return objectData(http);
}

static repoServerDataRef repoServerInit() {
  repoServerDataRef data = malloc(sizeof(struct repoServerData));
  if (data) {
    data->mg_context = NULL;
    data->registeredRepos = LCMutableDictionaryCreate(NULL, 0);
  }
  return data;
}

LCRepoServer LCRepoServerCreate(authorizeFun authorizationHandler) {
  repoServerDataRef data = repoServerInit();
  return objectCreate(LCTypeRepoServer, data);
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

void LCRepoServerStart(LCRepoServer http, char *port) {
  const char *options[] = {
    "listening_ports", port,
    "ssl_certificate", "resources/ssl_cert.pem", NULL
  };
  repoServerData(http)->mg_context = mg_start(&mg_callback, NULL, options);
}

void LCRepoServerStop(LCRepoServer http) {
  mg_stop(repoServerData(http)->mg_context);
}

void LCRepoServerRegisterRepository(LCRepoServer http, LCRepositoryRef repo, char *name) {
  LCStringRef nameString = LCStringCreate(name);
  LCMutableDictionarySetValueForKey(repoServerData(http)->registeredRepos, nameString, repo);
  objectRelease(nameString);
}

