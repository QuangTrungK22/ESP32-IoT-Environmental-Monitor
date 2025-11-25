#include "led_blinky.h"
#include "global.h"
void led_blinky(void *pvParameters){
  pinMode(LED_GPIO, OUTPUT);
  int tempMode = 0;
  
  while(1) {      
    // đọc trạng thái nhiệt độ 
    if(xSensorStateSemaphore != NULL) {
        if (xSemaphoreTake(xSensorStateSemaphore, 100) == pdTRUE) {
            tempMode = currentTempState;
            xSemaphoreGive(xSensorStateSemaphore);
        }
    }
    switch(tempMode){
      case 0: // mát: nháy chậm (2s)
        digitalWrite(LED_GPIO, HIGH);  // turn the LED ON
        vTaskDelay(100);
        digitalWrite(LED_GPIO, LOW);  // turn the LED OFF
        vTaskDelay(1900);
        break;
      case 1: // ấm nháy chậm (1s)
        digitalWrite(LED_GPIO, HIGH);
        vTaskDelay(500);
        digitalWrite(LED_GPIO, LOW);
        vTaskDelay(500);
        break;
      case 2: // nóng: nháy nhanh (0.5s)
        digitalWrite(LED_GPIO, HIGH);
        vTaskDelay(250);
        digitalWrite(LED_GPIO, LOW);
        vTaskDelay(250);
        break;
    }
    
  }
}