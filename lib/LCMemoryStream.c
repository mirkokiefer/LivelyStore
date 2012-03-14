
#include "LCMemoryStream.h"
#include "unistd.h"

void memoryStreamDealloc(void* object);
void memoryStreamPrint(void* object, FILE* fd);
static LCBool memoryStreamResizeBuffer(LCMemoryStreamRef stream, size_t length);
int memoryStreamWrite(void *cookie, const char *data, int length);
fpos_t memoryStreamSeek(void *cookie, fpos_t offset, int whence);
int memoryStreamRead(void *cookie, char *buffer, int length);
int memoryStreamClose(void *cookie);

struct LCMemoryStream {
  LCObjectInfo info;
  char* buffer;
  size_t bufferLength;
  size_t dataWritten;
};

struct cookie {
  LCMemoryStreamRef stream;
  size_t position;
};

LCType typeMemoryStream = {
  .dealloc = memoryStreamDealloc,
  .print = memoryStreamPrint
};

LCMemoryStreamRef LCMemoryStreamCreate() {
  LCMemoryStreamRef newStream = LCNewObject(&typeMemoryStream, sizeof(struct LCMemoryStream));
  newStream->dataWritten = 0;
  memoryStreamResizeBuffer(newStream, 10);
  return newStream;
}

FILE* LCMemoryStreamFile(LCMemoryStreamRef streamObj) {
  // on linux use fopencookie
  struct cookie *cookie = malloc(sizeof(struct cookie));
  if (cookie) {
    cookie->stream = LCRetain(streamObj);
    cookie->position = 0;
    return funopen(cookie, memoryStreamRead, memoryStreamWrite, memoryStreamSeek, memoryStreamClose);
  } else {
    return NULL;
  }
}

size_t LCMemoryStreamLength(LCMemoryStreamRef streamObj) {
  return streamObj->dataWritten;
}

char* LCMemoryStreamData(LCMemoryStreamRef streamObj) {
  return streamObj->buffer;
}

void memoryStreamDealloc(void* object) {
  LCMemoryStreamRef streamObj = (LCMemoryStreamRef)object;
  free(streamObj->buffer);
}

void memoryStreamPrint(void* object, FILE* fd) {
  LCMemoryStreamRef streamObj = (LCMemoryStreamRef)object;
  fwrite(streamObj->buffer, sizeof(char), streamObj->dataWritten, fd);
}

static LCBool memoryStreamResizeBuffer(LCMemoryStreamRef stream, size_t length) {
  void* buffer = realloc(stream->buffer, sizeof(char) * length);
  if(buffer) {
    stream->buffer = buffer;
    stream->bufferLength = length;
    return true;
  } else {
    return false;
  }
}

int memoryStreamWrite(void *cookie, const char *data, int length) {
  struct cookie *cookieStruct = (struct cookie*)cookie;
  LCMemoryStreamRef streamObj = cookieStruct->stream;
  if (cookieStruct->position+length >= streamObj->bufferLength) {
    memoryStreamResizeBuffer(streamObj, (streamObj->bufferLength+length)*2);
  }
  memcpy(&(streamObj->buffer[cookieStruct->position]), data, sizeof(char)*length);
  cookieStruct->position = cookieStruct->position + length;
  if (cookieStruct->position > streamObj->dataWritten) {
    streamObj->dataWritten = cookieStruct->position;
  }
  return length;
}

fpos_t memoryStreamSeek(void *cookie, fpos_t offset, int whence) {
  struct cookie *cookieStruct = (struct cookie*)cookie;
  LCMemoryStreamRef streamObj = cookieStruct->stream;
  switch (whence) {
    case SEEK_SET:
      cookieStruct->position = offset;
      break;
    case SEEK_CUR:
      cookieStruct->position = cookieStruct->position + offset;
    case SEEK_END:
      cookieStruct->position = streamObj->dataWritten + offset;
    default:
      break;
  }
  if (cookieStruct->position >= streamObj->bufferLength) {
    size_t oldLength = streamObj->bufferLength;
    memoryStreamResizeBuffer(streamObj, cookieStruct->position*2);
    for (LCInteger i=oldLength; i<cookieStruct->position; i++) {
      streamObj->buffer[i] = 0;
    }
  }
  return cookieStruct->position;
}

int memoryStreamRead(void *cookie, char *buffer, int length) {
  struct cookie *cookieStruct = (struct cookie*)cookie;
  LCMemoryStreamRef streamObj = cookieStruct->stream;
  if (cookieStruct->position >= streamObj->dataWritten) {
    return 0;
  }
  if ((cookieStruct->position + length) >= streamObj->dataWritten) {
    length = streamObj->dataWritten - cookieStruct->position;
  }
  memcpy(buffer, &(streamObj->buffer[cookieStruct->position]), sizeof(char)*length);
  cookieStruct->position = cookieStruct->position + length;
  return length;
}

int memoryStreamClose(void *cookie) {
  struct cookie *cookieStruct = (struct cookie*)cookie;
  LCRelease(cookieStruct->stream);
  return 0;
}