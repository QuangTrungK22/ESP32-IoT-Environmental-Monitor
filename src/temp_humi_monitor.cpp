#include "temp_humi_monitor.h"
#include "DHT20.h"
#include "global.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SDA_PIN GPIO_NUM_11
#define SCL_PIN GPIO_NUM_12
DHT20 dht20;
LiquidCrystal_I2C lcd(33, 16, 2); 

void temp_humi_monitor(void *pvParameters) {
    // --- 1. KHỞI TẠO LCD TRƯỚC (QUAN TRỌNG) ---
    // Để LCD khởi tạo Wire mặc định trước, tránh reset cấu hình của mình
    Wire.begin(SDA_PIN, SCL_PIN); 
    lcd.begin(); 
    lcd.backlight();
    lcd.print("Init Sensor...");

    // --- 2. CẤU HÌNH LẠI I2C CHO ĐÚNG CHÂN ---
    // Ghi đè cấu hình chân 11, 12 sau khi LCD đã init xong
    
    
    // --- 3. KHỞI TẠO DHT20 ---
    // Lúc này DHT20 sẽ dùng đúng chân 11, 12 mà ta vừa set
    dht20.begin();

    Serial.println("[Sensor] Init Done.");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    while (1) {
        // --- ĐỌC CẢM BIẾN ---
        int status = dht20.read();
        
        if (status != DHT20_OK) {
            Serial.println("DHT20 Error! Resetting I2C...");
            // Nếu lỗi, thử reset lại I2C
            Wire.begin(SDA_PIN, SCL_PIN);
            dht20.begin();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }

        float temp = dht20.getTemperature();
        float humi = dht20.getHumidity();

        glob_temperature = temp;
        glob_humidity = humi;

        // --- LOGIC PHÂN LOẠI ---
        int tState = 0; 
        if (temp >= 30 && temp < 35) tState = 1; 
        else if (temp >= 35) tState = 2;         

        int hState = 0; 
        if (humi >= 60 && humi < 80) hState = 1; 
        else if (humi >= 80) hState = 2;         
        Serial.printf("Temp: %.1f C, State: %d | Humi: %.1f %%, State: %d\n", temp, tState, humi, hState);
        // Gửi Semaphore (Đã sửa tên biến currentHumiState)
        if (xSensorStateSemaphore != NULL) {
            if (xSemaphoreTake(xSensorStateSemaphore, 100) == pdTRUE) {
                currentTempState = tState;
                currentHumiState = hState; // Đã sửa từ currentHumidState
                xSemaphoreGive(xSensorStateSemaphore);
            }
        }

        // --- HIỂN THỊ LCD ---
        lcd.setCursor(0, 0);
        lcd.print("T:"); lcd.print(temp, 1); lcd.print(" H:"); lcd.print(humi, 0); lcd.print("%");
        
        lcd.setCursor(0, 1);
        if (glob_is_anomaly) {
            lcd.print("AI: ANOMALY!    ");
        } else if (tState == 2) {
            lcd.print("WARN: HIGH TEMP!");
        } else {
            lcd.print("System Normal   ");
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}