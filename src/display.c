#include "../include/display.h"
#include <cjson/cJSON.h>
#include <stdio.h>

void disp_current(cJSON *data) {
  cJSON *current = cJSON_GetObjectItem(data, "current");
  cJSON *current_units = cJSON_GetObjectItem(data, "current_units");

  double temp = cJSON_GetObjectItem(current, "temperature_2m")->valuedouble;
  char *strtemp = to_string(temp);
  double precipitation =
      cJSON_GetObjectItem(current, "precipitation")->valuedouble;
  char *strprecip = to_string(precipitation);

  clean_str(strtemp);
  clean_str(strprecip);

  char *temp_units =
      cJSON_GetObjectItem(current_units, "temperature_2m")->valuestring;
  char *precipitation_units =
      cJSON_GetObjectItem(current_units, "precipitation")->valuestring;

  printf("====================\n");
  printf("Now\n");
  printf("====================\n");
  printf("Temperature : %s%s\n", strtemp, temp_units);
  printf("Precipitation : %s %s\n", strprecip, precipitation_units);

  free(strtemp);
  free(strprecip);
}

void disp_today(cJSON *data) {
  cJSON *hourly = cJSON_GetObjectItem(data, "hourly");
  int hour = get_time(data);

  int counter = 23 - hour;
  double *temps = malloc(sizeof(double) * (counter + 1));
  double *precipitations = malloc(sizeof(double) * (counter + 1));

  cJSON *temp_json = cJSON_GetObjectItem(hourly, "temperature_2m");
  cJSON *precipit_json = cJSON_GetObjectItem(hourly, "precipitation");

  char **temps_str = malloc(sizeof(char *) * (counter + 1));
  char **precipit_str = malloc(sizeof(char *) * (counter + 1));

  for (int i = counter; i >= 0; i--) {
    int index = 23 - i;
    temps[counter - i] = cJSON_GetArrayItem(temp_json, index)->valuedouble;
    precipitations[counter - i] =
        cJSON_GetArrayItem(precipit_json, index)->valuedouble;

    temps_str[counter - i] = to_string(temps[counter - i]);
    precipit_str[counter - i] = to_string(precipitations[counter - i]);

    clean_str(temps_str[counter - i]);
    clean_str(precipit_str[counter - i]);
  }
  cJSON *hourly_units = cJSON_GetObjectItem(data, "hourly_units");

  char *temp_units =
      cJSON_GetObjectItem(hourly_units, "temperature_2m")->valuestring;
  char *precipitation_units =
      cJSON_GetObjectItem(hourly_units, "precipitation")->valuestring;

  printf("====================\n");
  printf("Today\n");
  printf("====================\n");
  for (int i = 0; i <= counter; i++) {
    printf("%dh\t", i + hour);
    printf("Temp : %s%s\t", temps_str[i], temp_units);
    printf("Precipitations : %s %s\n", precipit_str[i], precipitation_units);
  }
}

void disp_week(cJSON *data) {
  cJSON *daily = cJSON_GetObjectItem(data, "daily");
  // int hour = get_time(data);

  // int counter = 23 - hour;
  double *tempsmin = malloc(sizeof(double) * 7);
  double *tempsmax = malloc(sizeof(double) * 7);
  double *precipitations = malloc(sizeof(double) * 7);

  cJSON *tempmin_json = cJSON_GetObjectItem(daily, "temperature_2m_min");
  cJSON *tempmax_json = cJSON_GetObjectItem(daily, "temperature_2m_max");
  cJSON *precipit_json = cJSON_GetObjectItem(daily, "precipitation_sum");

  char **tempsmin_str = malloc(sizeof(char *) * 7);
  char **tempsmax_str = malloc(sizeof(char *) * 7);
  char **precipit_str = malloc(sizeof(char *) * 7);

  for (int i = 0; i < 7; i++) {
    tempsmin[i] = cJSON_GetArrayItem(tempmin_json, i)->valuedouble;
    tempsmax[i] = cJSON_GetArrayItem(tempmax_json, i)->valuedouble;
    precipitations[i] = cJSON_GetArrayItem(precipit_json, i)->valuedouble;

    tempsmin_str[i] = to_string(tempsmin[i]);
    tempsmax_str[i] = to_string(tempsmax[i]);
    precipit_str[i] = to_string(precipitations[i]);

    clean_str(tempsmin_str[i]);
    clean_str(tempsmax_str[i]);
    clean_str(precipit_str[i]);
  }
  cJSON *daily_units = cJSON_GetObjectItem(data, "daily_units");

  char *temp_units =
      cJSON_GetObjectItem(daily_units, "temperature_2m_min")->valuestring;
  char *precipitation_units =
      cJSON_GetObjectItem(daily_units, "precipitation_sum")->valuestring;

  cJSON *date = cJSON_GetObjectItem(daily, "time");

  printf("====================\n");
  printf("Week\n");
  printf("====================\n");
  for (int i = 0; i < 7; i++) {
    char *c_date = cJSON_GetArrayItem(date, i)->valuestring;
    printf("%s\t", c_date);
    printf("Min temp : %s%s\t", tempsmin_str[i], temp_units);
    printf("Max temp : %s%s\t", tempsmax_str[i], temp_units);
    printf("Precipitations : %s %s\n", precipit_str[i], precipitation_units);
  }
}

int get_time(cJSON *data) {
  cJSON *current = cJSON_GetObjectItem(data, "current");
  cJSON *time = cJSON_GetObjectItem(current, "time");

  char *str = time->valuestring;
  char *ptr = str;

  int hour = 0;
  while (*ptr != 'T')
    ptr++;

  if (*(ptr + 2) == ':') {
    hour = (int)(*(ptr + 1) - '0');
  } else {
    hour = (int)(*(ptr + 1) - '0') * 10 + (int)(*(ptr + 2) - '0');
  }

  return hour;
}

char *to_string(double n) {
  int SIZE = 16;
  char *str = malloc(sizeof(char) * SIZE);

  snprintf(str, SIZE, "%lf", n);
  str[15] = 0;

  return str;
}

void clean_str(char *str) {
  bool doub = is_double(str);
  if (!doub)
    return;

  char *ptr = str;
  char *zero = NULL;
  bool after_point = false;
  while (*ptr != 0) {
    if (*ptr == '.')
      after_point = true;

    if (after_point && *ptr == '0' && zero == NULL) {
      zero = ptr;
    }

    if (after_point && *ptr != '0' && zero != NULL) {
      zero = NULL;
    }

    ptr++;
  }

  if (zero != NULL) {
    if (*(zero - 1) == '.')
      *(zero - 1) = 0;
    else
      *zero = 0;
  }

  return;
}

bool is_double(char *str) {
  char *ptr = str;
  while (*ptr != 0) {
    if (*ptr == '.')
      return true;

    ptr++;
  }

  return false;
}
