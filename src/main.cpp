#include "global.h"

#include "led_blinky.h"
#include "neo_blinky.h"
#include "temp_humi_monitor.h"
#include "mainserver.h"
#include "tinyml.h"
#include "coreiot.h"

// include task
#include "task_check_info.h"
#include "task_toogle_boot.h"
#include "task_wifi.h"
#include "task_webserver.h"
#include "task_core_iot.h"

void setup()
{
  Serial.begin(115200);
  check_info_File(0);
  // --- 1. KHỞI TẠO SEMAPHORE (THÊM VÀO ĐÂY) ---
  if (xBinarySemaphoreInternet == NULL) {
      xBinarySemaphoreInternet = xSemaphoreCreateBinary();
      xSemaphoreGive(xBinarySemaphoreInternet);
  }
  
  xCloudStateSemaphore = xSemaphoreCreateBinary();
  if (xCloudStateSemaphore != NULL) {
      xSemaphoreGive(xCloudStateSemaphore);
  }
  // ---------------------------------------------
  // --- KHỞI TẠO SEMAPHORE CHO CẢM BIẾN (THÊM MỚI) ---
  xSensorStateSemaphore = xSemaphoreCreateBinary();
  if (xSensorStateSemaphore != NULL) {
      xSemaphoreGive(xSensorStateSemaphore); // Mở khóa ngay
  }


  xTaskCreate(led_blinky, "Task LED Blink", 2048, NULL, 2, NULL);
  xTaskCreate(neo_blinky, "Task NEO Blink", 2048, NULL, 2, NULL);
  xTaskCreate(temp_humi_monitor, "Task TEMP HUMI Monitor", 4096,NULL, 2, NULL);
  //xTaskCreate(main_server_task, "Task Main Server" ,8192  ,NULL  ,2 , NULL);
  xTaskCreate(main_server_task, "main_server_task", 8192, NULL, 2, NULL);
  xTaskCreate( tiny_ml_task, "Tiny ML Task" ,8192  ,NULL  ,2 , NULL);
  //xTaskCreate(coreiot_task, "CoreIOT Task" ,4096  ,NULL  ,2 , NULL);
  //xTaskCreate(Task_Toogle_BOOT, "Task_Toogle_BOOT", 4096, NULL, 2, NULL);
}

void loop()
{
  // if (check_info_File(1))
  // {
  //   if (!Wifi_reconnect())
  //   {
  //     Webserver_stop();
  //   }
  //   else
  //   {
  //     //CORE_IOT_reconnect();
  //   }
  // }
  // Webserver_reconnect();
  // vTaskDelay(10); // Thêm delay nhẹ để tránh watchdog nếu loop chạy quá nhanh
}