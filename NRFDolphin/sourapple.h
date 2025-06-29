/* ____________________________
   This software is licensed under the MIT License:
   https://github.com/cifertech/nrfbox
   ________________________________________ */
   
#ifndef sourapple_H
#define sourapple_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_NeoPixel.h>

void sourappleSetup();
void sourappleLoop();

extern Adafruit_NeoPixel pixels;

#endif
