#include "../include/api.h"
#include <stdlib.h>

size_t getraw(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  RawData *mem = (RawData *)userp;

  mem->data = realloc(mem->data, mem->size + realsize + 1);
  if (mem->data == NULL) {
    printf("Not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->data[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->data[mem->size] = 0;

  return realsize;
}

ApiData *api_init() {
  ApiData *data = malloc(sizeof(ApiData));

  CURLcode ret;
  CURL *hnd;

  RawData *chunk = malloc(sizeof(RawData));
  chunk->data = malloc(1);
  chunk->size = 0;

  data->rawData = chunk;

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
  curl_easy_setopt(
      hnd, CURLOPT_URL,
      "https://api.open-meteo.com/v1/"
      "forecast\?latitude=50.6337&longitude=5.5675&daily=temperature_2m_min,"
      "temperature_2m_max,precipitation_sum&hourly=temperature_2m,"
      "precipitation_probability,precipitation&current=temperature_2m,"
      "precipitation&timezone=auto");
  curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, getraw);
  curl_easy_setopt(hnd, CURLOPT_WRITEDATA, (void *)chunk);
  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/8.12.1");
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

  /* Here is a list of options the curl code used that cannot get generated
     as source easily. You may choose to either not use them or implement
     them yourself.

  CURLOPT_WRITEDATA was set to an object pointer
  CURLOPT_INTERLEAVEDATA was set to an object pointer
  CURLOPT_WRITEFUNCTION was set to a function pointer
  CURLOPT_READDATA was set to an object pointer
  CURLOPT_READFUNCTION was set to a function pointer
  CURLOPT_SEEKDATA was set to an object pointer
  CURLOPT_SEEKFUNCTION was set to a function pointer
  CURLOPT_ERRORBUFFER was set to an object pointer
  CURLOPT_STDERR was set to an object pointer
  CURLOPT_HEADERFUNCTION was set to a function pointer
  CURLOPT_HEADERDATA was set to an object pointer

  */

  ret = curl_easy_perform(hnd);
  if (ret >= 400 && ret < 600) {
    exit(1);
  }

  curl_easy_cleanup(hnd);
  hnd = NULL;

  return data;
}

void api_free(ApiData *data) {
  free(data->rawData->data);
  free(data->rawData);
  free(data);
}
