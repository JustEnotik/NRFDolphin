/* ____________________________
   This software is licensed under the MIT License:
   https://github.com/cifertech/nrfbox
   ________________________________________ */
   
#ifndef spoofer_H
#define spoofer_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <U8g2lib.h>
#include <Adafruit_NeoPixel.h>

void spooferSetup();
void spooferLoop();

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern Adafruit_NeoPixel pixels;

#endif
