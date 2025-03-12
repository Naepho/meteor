#include <cjson/cJSON.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void disp_current(cJSON *data);
void disp_today(cJSON *data);
void disp_week(cJSON *data);
int get_time(cJSON *data);
void clean_str(char *str);
bool is_double(char *str);
char *to_string(double n);
