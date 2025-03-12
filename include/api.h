#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
typedef struct _RawData {
  char *data;
  size_t size;
} RawData;

typedef struct _ApiData {
  RawData *rawData;
} ApiData;

ApiData *api_init();
size_t getraw(void *contents, size_t size, size_t nmemb, void *userp);
void api_free(ApiData *);
