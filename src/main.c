#include "../include/api.h"
#include <stdio.h>

int main() {
  ApiData *api = api_init();

  fprintf(stdout, api->rawData->data);

  api_free(api);
  return 0;
}
