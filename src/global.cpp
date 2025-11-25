#include "global.h"
float glob_temperature = 0;
float glob_humidity = 0;

String WIFI_SSID;
String WIFI_PASS;
String CORE_IOT_TOKEN;
String CORE_IOT_SERVER;
String CORE_IOT_PORT;

String ssid = "ESP32-YOUR NETWORK HERE!!!";
String password = "12345678";
String wifi_ssid = "abcde";
String wifi_password = "123456789";
boolean isWifiConnected = false;
SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();
int cloudLedState = 0;
SemaphoreHandle_t xCloudStateSemaphore = NULL;
// --- KHỞI TẠO BIẾN TASK 1 & 2 ---
int currentTempState = 0;
int currentHumidState = 0;
SemaphoreHandle_t xSensorStateSemaphore = NULL;
// Biến giữ chỗ cho TinyML (Luôn false vì chưa làm Task 5)
float glob_ml_result = 0.0;
bool glob_is_anomaly = false;
bool glob_anomaly = false; // Biến giữ trạng thái anomaly để gửi web và IoT