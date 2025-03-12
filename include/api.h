#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

typedef struct _RawData {
  char *data;
  size_t size;
} RawData;

typedef struct _ApiData {
  RawData *rawData;
  cJSON *json;
} ApiData;

ApiData *api_init();
cJSON *api_get_json(ApiData *data);
void api_free(ApiData *data);

size_t getraw(void *contents, size_t size, size_t nmemb, void *userp);
