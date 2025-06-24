This device have BLE/BLuetooth/Wi-Fi Jammer (Illegaly), Scanner, Analyzer, BLE Spoofer and Sour Apple Spam functions

👀 Project based on nRF-BOX by CiferTech
💩 P. S. Project have shit-code and code by ChatGPT

⚠ If you're running out of memory on the ESP32 for your firmware, go to Tools > Partition Scheme and select "Huge App (3MB No OTA/1MB SPIFFS)".
![image](https://github.com/user-attachments/assets/916e75fb-3d79-4d1f-9d5c-d9b45b98a89c)


📚 Libraries:
Adafruit_NeoPixel, RF24, U8g2

🧮 Connection:

⚠ In the nRF-Box PCB v3, the SELECT and DOWN buttons are swapped.
![photo_2025-06-24_14-04-41](https://github.com/user-attachments/assets/9a66c325-6429-49e9-9632-44f2e17ffe3d)
| **Component**      | **Pin**      | **ESP32 Pin / Value** | **NRF 1** | **NRF 2** | **NRF 3** |
| ------------------ | ------------ | --------------------- | --------- | --------- | --------- |
| **NRF24**          | GND (Pin 1)  | GND                   | GND       | GND       | GND       |
|                    | VCC (Pin 2)  | 3V3                   | 3V3       | 3V3       | 3V3       |
|                    | CE (Pin 3)   |                       | D5        | D16       | D15       |
|                    | CSN (Pin 4)  |                       | D17       | D4        | D2        |
|                    | SCK (Pin 5)  | D18                   | D18       | D18       | D18       |
|                    | MOSI (Pin 6) | D23                   | D23       | D23       | D23       |
|                    | MISO (Pin 7) | D19                   | D19       | D19       | D19       |
| **Screen Power**   | GND          | GND                   |           |           |           |
|                    | VDD/VCC      | 3V3                   |           |           |           |
| **Screen I2C**     | SCK/SCL      | D22                   |           |           |           |
|                    | SDA          | D21                   |           |           |           |
| **Screen Buttons** | UP           | D26                   |           |           |           |
|                    | SELECT       | D32                   |           |           |           |
|                    | DOWN         | D33                   |           |           |           |
|                    | LEFT         | D25                   |           |           |           |
|                    | RIGHT        | D27                   |           |           |           |
| **NeoPixel**       | 5V           | 3V3                   |           |           |           |
|                    | DI           | D14                   |           |           |           |
|                    | GND          | GND                   |           |           |           |

