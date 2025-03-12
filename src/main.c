#include "../include/api.h"
#include "../include/display.h"
#include <cjson/cJSON.h>
#include <stdio.h>

int main() {
  ApiData *api = api_init();

  cJSON *data = api_get_json(api);
  disp_current(data);
  printf("\n");
  disp_today(data);
  printf("\n");
  disp_week(data);

  api_free(api);
  return 0;
}
