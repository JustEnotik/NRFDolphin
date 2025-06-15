/* ____________________________
   This software is licensed under the MIT License:
   https://github.com/cifertech/nrfbox
   ________________________________________ */

#include <Arduino.h> 
#include "blejammer.h"
#include "icon.h"
#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 14
#define NUM_PIXELS 1

extern Adafruit_NeoPixel pixels;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

#define CE_PIN_1  5
#define CSN_PIN_1 17

#define CE_PIN_2  16
#define CSN_PIN_2 4

#define CE_PIN_3  15
#define CSN_PIN_3 2

#define MODE_BUTTON 27

RF24 radio1(CE_PIN_1, CSN_PIN_1, 16000000);
RF24 radio2(CE_PIN_2, CSN_PIN_2, 16000000);
RF24 radio3(CE_PIN_3, CSN_PIN_3, 16000000);

enum OperationMode { DEACTIVE_MODE, BLE_MODULE, Bluetooth_MODULE, WiFi_MODULE };
OperationMode currentMode = DEACTIVE_MODE;

int bluetooth_channels[] = {32, 34, 46, 48, 50, 52, 0, 1, 2, 4, 6, 8, 22, 24, 26, 28, 30, 74, 76, 78, 80};
int ble_channels[] = {2, 26, 80};

const byte BLE_channels[] = {2, 26, 80}; // Corresponding BLE advertising channels
byte channelGroup1[] = {2, 5, 8, 11};    // Module 1 frequency group
byte channelGroup2[] = {26, 29, 32, 35}; // Module 2 frequency group
byte channelGroup3[] = {80, 83, 86, 89}; // Module 3 frequency group

volatile bool modeChangeRequested = false;

unsigned long lastJammingTime = 0;
const unsigned long jammingInterval = 10;

unsigned long lastButtonPressTime = 0;
const unsigned long debounceDelay = 500;

void IRAM_ATTR handleButtonPress() {
  unsigned long currentTime = millis();
  if (currentTime - lastButtonPressTime > debounceDelay) {
    modeChangeRequested = true;
    lastButtonPressTime = currentTime;
  }
}

void configureRadio(RF24 &radio, const byte* channels, size_t size) {
  radio.setAutoAck(false);
  radio.stopListening();
  radio.setRetries(0, 0);
  radio.setPALevel(RF24_PA_MAX, true);
  radio.setDataRate(RF24_2MBPS);
  radio.setCRCLength(RF24_CRC_DISABLED);
  radio.printPrettyDetails();

  for (size_t i = 0; i < size; i++) {
    radio.setChannel(channels[i]);
    radio.startConstCarrier(RF24_PA_MAX, channels[i]);
  }
}

void initializeRadiosMultiMode() {
  if (radio1.begin()) {
    configureRadio(radio1, channelGroup1, sizeof(channelGroup1));
  }
  if (radio2.begin()) {
    configureRadio(radio2, channelGroup2, sizeof(channelGroup2));
  }
  if (radio3.begin()) {
    configureRadio(radio3, channelGroup3, sizeof(channelGroup3));
  }
}

void initializeRadios() {
  if (currentMode == BLE_MODULE) {
    if (radio1.begin()) {
      configureRadio(radio1, BLE_channels, sizeof(BLE_channels));
    }
  } else if (currentMode == Bluetooth_MODULE) {
    initializeRadiosMultiMode();
  } else if (currentMode == DEACTIVE_MODE) {
    radio1.setChannel(0);
    radio2.setChannel(0);
    radio3.setChannel(0);
    delay(100);
  }
}

void jammer(RF24 &radio, const byte* channels, size_t size) {
  const char text[] = "xxxxxxxxxxxxxxxx";
  for (size_t i = 0; i < size; i++) {
    radio.setChannel(channels[i]);
    radio.write(&text, sizeof(text));
    //delayMicroseconds(20);
  }
}

void updateOLED() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);

  u8g2.setCursor(0, 10);
  u8g2.print("Mode ");
  u8g2.print(": ");
  u8g2.setCursor(40, 10);
  u8g2.print(currentMode == WiFi_MODULE ? "Wi-Fi" : currentMode == BLE_MODULE ? "BLE" : currentMode == Bluetooth_MODULE ? "Bluetooth" : "OFF");

  u8g2.setCursor(0, 35);
  u8g2.print("NRF 1: ");
  u8g2.setCursor(40, 35);
  u8g2.print(radio1.isChipConnected() ? "Yes" : "No");

  u8g2.setCursor(0, 50);
  u8g2.print("NRF 2: ");
  u8g2.setCursor(40, 50);
  u8g2.print(radio2.isChipConnected() ? "Yes" : "No");

  u8g2.setCursor(0, 64);
  u8g2.print("NRF 3: ");
  u8g2.setCursor(40, 64);
  u8g2.print(radio3.isChipConnected() ? "Yes" : "No");

  u8g2.drawXBMP(currentMode != DEACTIVE_MODE ? 75 : 73, currentMode != DEACTIVE_MODE ? 9 : 14, currentMode != DEACTIVE_MODE ? 51 : 49, currentMode != DEACTIVE_MODE ? 64 : 54, currentMode != DEACTIVE_MODE ? dolphin_think : dolphin_cry);

  u8g2.sendBuffer();
}

void checkModeChange() {
  if (modeChangeRequested) {
    modeChangeRequested = false;
    currentMode = static_cast<OperationMode>((currentMode + 1) % 4);
    initializeRadios();
    updateOLED();
  }
}

void blejammerSetup() {
  Serial.begin(115200);

  esp_bt_controller_deinit();
  esp_wifi_stop();
  esp_wifi_deinit();
  esp_wifi_disconnect();

  u8g2.begin();

  pinMode(MODE_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MODE_BUTTON), handleButtonPress, FALLING);

  initializeRadios();
  updateOLED();

  pixels.begin();
  //pixels.setBrightness(255);  // можешь увеличить
  pixels.setPixelColor(0, pixels.Color(8, 8, 8));  // белый — пока OFF
  pixels.show();
}

void blejammerLoop() {
  checkModeChange();

  if (currentMode == BLE_MODULE) {
    int randomIndex = random(0, sizeof(ble_channels) / sizeof(ble_channels[0]));
    int channel = ble_channels[randomIndex];
    radio1.setChannel(channel);
    radio2.setChannel(channel);
    radio3.setChannel(channel);

  } else if (currentMode == Bluetooth_MODULE) {
    int randomIndex = random(0, sizeof(bluetooth_channels) / sizeof(bluetooth_channels[0]));
    int channel = bluetooth_channels[randomIndex];
    radio1.setChannel(channel);
    radio2.setChannel(channel);
    radio3.setChannel(channel);

  } else if (currentMode == WiFi_MODULE) {
    static size_t index = 0;
    static unsigned long lastChange = 0;
    unsigned long now = millis();

    if (now - lastChange >= jammingInterval) {
      int channel = bluetooth_channels[index];
      radio1.setChannel(channel);
      radio2.setChannel(channel);
      radio3.setChannel(channel);

      const char junk[] = "xxxxxxxxxxxxxxxx";
      radio1.write(&junk, sizeof(junk));
      radio2.write(&junk, sizeof(junk));
      radio3.write(&junk, sizeof(junk));

      index = (index + 1) % (sizeof(bluetooth_channels) / sizeof(bluetooth_channels[0]));
      lastChange = now;
    }
  }


  // <-- вот сюда выносим общую отрисовку пикселя
  if (currentMode == DEACTIVE_MODE) {
    pixels.setPixelColor(0, pixels.Color(8, 8, 8));  // OFF — белый
  } else {
    pixels.setPixelColor(0, pixels.Color(16, 0, 0));  // активный джамминг — красный
  }
  pixels.setBrightness(255); // максимально ярко
  pixels.show();
}

