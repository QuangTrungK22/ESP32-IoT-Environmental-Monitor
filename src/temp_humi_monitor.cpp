#include "temp_humi_monitor.h"
#include "DHT20.h"
#include "global.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

DHT20 dht20;
// Địa chỉ LCD: 33 (0x21). Nếu không hiện chữ, hãy thử đổi thành 0x27 hoặc 0x3F
LiquidCrystal_I2C lcd(33, 16, 2); 

void temp_humi_monitor(void *pvParameters) {
    // --- 1. KHỞI TẠO ---
    // Dùng hàm begin của DHT20 hỗ trợ ESP32 (SDA=11, SCL=12)
    dht20.begin(11, 12);
    
    // --- SỬA LỖI TẠI ĐÂY ---
    // Thay lcd.init() bằng lcd.begin() cho đúng thư viện
    lcd.begin(); 
    lcd.backlight(); // Bật đèn nền
    
    Serial.println("[Sensor] Init Done.");
    
    // Đợi cảm biến ổn định
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    while (1) {
        // --- 2. ĐỌC CẢM BIẾN ---
        int status = dht20.read();
        
        // Kiểm tra lỗi đọc cảm biến
        if (status != DHT20_OK) {
            Serial.println("DHT20 Read Error!");
            // Nếu lỗi kết nối, thử khởi tạo lại
            if (status == DHT20_ERROR_CONNECT) {
                 dht20.begin(11, 12);
            }
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }

        float temp = dht20.getTemperature();
        float humi = dht20.getHumidity();

        // --- 3. CẬP NHẬT DỮ LIỆU CHUNG ---
        glob_temperature = temp;
        glob_humidity = humi;

        // --- 4. LOGIC PHÂN LOẠI (Cho Task 1 & 2) ---
        int tState = 0; // Mát
        if (temp >= 30 && temp < 35) tState = 1; // Ấm
        else if (temp >= 35) tState = 2;         // Nóng

        int hState = 0; // Khô
        if (humi >= 60 && humi < 80) hState = 1; // Đủ
        else if (humi >= 80) hState = 2;         // Ướt

        // Gửi tín hiệu an toàn bằng Semaphore
        if (xSensorStateSemaphore != NULL) {
            if (xSemaphoreTake(xSensorStateSemaphore, 100) == pdTRUE) {
                currentTempState = tState;
                currentHumidState = hState;
                xSemaphoreGive(xSensorStateSemaphore);
            }
        }

        // --- 5. HIỂN THỊ LCD & SERIAL ---
        Serial.printf("Humi: %.1f%% - Temp: %.1f C\n", humi, temp);
        
        lcd.setCursor(0, 0);
        lcd.print("T:"); lcd.print(temp, 1); lcd.print("C H:"); lcd.print(humi, 0); lcd.print("%");
        
        lcd.setCursor(0, 1);
        // Hiển thị trạng thái hoặc cảnh báo AI (nếu có Task 5)
        if (glob_is_anomaly) {
            lcd.print("AI: ANOMALY!    ");
        } else if (tState == 2) {
            lcd.print("CANH BAO NONG!  ");
        } else {
            lcd.print("System Normal   ");
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}