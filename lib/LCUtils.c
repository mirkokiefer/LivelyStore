
#include "LivelyStoreTests.h"
#include <ftw.h>
#include <unistd.h>

int objectCompare(const void * elem1, const void * elem2);
LCObjectInfo* LCGetObjectInfo(void* object);

LCObjectInfo* LCGetObjectInfo(void* object) {
  return (LCObjectInfo*)object;
}

void* LCNewObject(LCType* type, size_t size) {
  void* newObj = malloc(size);
  if (newObj) {
    LCObjectInfo* info = LCGetObjectInfo(newObj);
    info->rCount = 0;
    info->type = type;
  }
  return newObj;
}

void LCFree(void* memory) {
  //printf("free: %p\n", memory);
  free(memory);
}

void* LCRetain(void* object) {
  if (object) {
    LCObjectInfo* info = LCGetObjectInfo(object);
    info->rCount++;
  }
  return object;
}

void* LCRelease(void* object) {
  if (object) {
    LCObjectInfo* info = LCGetObjectInfo(object);
    info->rCount--;
    if(info->rCount == -1) {
      LCTypeRef type = (*info).type;
      if(type->dealloc) {
        type->dealloc(object);      
      }
      LCFree(object);
    } 
  }
  return object;
}

void LCPrintf(void* object) {
  LCPrint(object, stdout);
}

void LCPrint(void* object, FILE* fd) {
  if (object) {
    LCObjectInfo* info = LCGetObjectInfo(object);
    if (info->type->print) {
      info->type->print(object, fd);
    } else {
      fprintf(fd, "%p", object);
    }
  } else {
    fprintf(fd, "%p", object);
  }
}

LCCompare LCCompareObjects(void* object1, void* object2) {
  if (object1 == NULL) {
    return LCSmaller;
  } else if (object2 == NULL) {
    return LCGreater;
  }
  LCType* type1 = LCGetObjectInfo(object1)->type;
  if(type1->compare == NULL) {
    if(object1 == object2) {
      return LCEqual;
    } else {
      if (object1 > object2) {
        return LCGreater;
      } else {
        return LCSmaller;
      }
    }
  } else {
    return LCGetObjectInfo(object1)->type->compare(object1, object2);    
  }
}

int objectCompare(const void * elem1, const void * elem2) {
  void** object1 = (void**)elem1;
  void** object2 = (void**)elem2;
  LCCompare result = LCCompareObjects(*object1, *object2);
  if (result == LCGreater) {
    return  1; 
  }
  if (result == LCSmaller) {
    return -1; 
  }
  return 0;
}

void LCSortObjects(void* objects[], size_t length) {
  qsort(objects, length, sizeof(void*), objectCompare);
}

LCInteger LCRetainCount(void* object) {
  LCObjectInfo* info = (LCObjectInfo*)object;
  return info->rCount;
}


char hexDigitToASCIChar(char hexDigit) {
  if(hexDigit > 9) {
    hexDigit = hexDigit - 10 + 97; //97 is A
  } else {
    hexDigit = hexDigit + 48; //48 is 0
  }
  return hexDigit;
}

char asciCharToHexDigit(char asciChar) {
  if (asciChar >= 97) { //A = 97
    return asciChar - 97 + 10; // A -> 10, B -> 11...
  } else {
    return asciChar - 48; // if a number char (48=char 0)
  }
}

void byteToHexDigits(LCByte input, char* buffer) {
  buffer[0] = hexDigitToASCIChar(input/16);
  buffer[1] = hexDigitToASCIChar(input%16);
}

LCByte hexDigitsToByte(char* hexDigits) {
  return hexDigits[0]*16 + hexDigits[1];
}

LCStringRef createHexString(LCByte data[], size_t length) {
  char buffer[length*2+1];
  for(LCInteger i=0; i<length; i++) {
    byteToHexDigits(data[i], &buffer[i*2]);
  }
  buffer[length*2] = '\0';
  return LCStringCreate(buffer);
}

LCDataRef createDataFromHexString(LCStringRef hexString) {
  LCByte buffer[LC_SHA1_Length];
  for (LCInteger i=0; i<LC_SHA1_Length; i++) {
    char digit1 = LCStringCharAtIndex(hexString, i*2);
    char digit2 = LCStringCharAtIndex(hexString, (i*2)+1);
    char hexDigits[] = {asciCharToHexDigit(digit1), asciCharToHexDigit(digit2)};
    buffer[i] = hexDigitsToByte(hexDigits);
  }
  return LCDataCreate(buffer, LC_SHA1_Length);
}

LCArrayRef createPathArray(LCStringRef path) {
  return LCStringCreateTokens(path, '/');
}

void writeToFile(LCByte data[], size_t length, char* filePath) {
  FILE *fp = fopen(filePath, "w");
  fwrite(data, sizeof(unsigned char), length, fp);
  fclose(fp);
}

int makeDirectory(char* path) {
  return mkdir(path, S_IRWXU);
}

static int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
  int rv = remove(fpath);
  
  if (rv)
    perror(fpath);
  
  return rv;
}

int deleteDirectory(char *path) {
  return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

LCStringRef getHomeFolder() {
  struct passwd *passwdEnt = getpwuid(getuid());
  return LCStringCreate(passwdEnt->pw_dir);
}
