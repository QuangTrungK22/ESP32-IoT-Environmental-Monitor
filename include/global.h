#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

extern float glob_temperature;
extern float glob_humidity;

extern String WIFI_SSID;
extern String WIFI_PASS;
extern String CORE_IOT_TOKEN;
extern String CORE_IOT_SERVER;
extern String CORE_IOT_PORT;

extern boolean isWifiConnected;
extern SemaphoreHandle_t xBinarySemaphoreInternet;

extern int cloudLedState;
extern SemaphoreHandle_t xCloudStateSemaphore;
extern int currentTempState;
extern int currentHumidState;
//semaphore bao ve 2 bien tren
extern SemaphoreHandle_t xSensorStateSemaphore;
extern bool glob_is_anomaly;
#endif