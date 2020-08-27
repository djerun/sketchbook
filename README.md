# Arduino IDE Sketchbook

I said I would upload the code for my LED Hat once I cleaned it up, but thats probably not going to happen so I decided to just dump the whole sketchbook here as is before my tinkering notebooks spinning harddrive decides to give up some day.

Also some things where written with Arduino IDE 1.0.5 (downloaded and installed debian repository)  the rest with Arduino IDE 1.8.6 (downloaded from Arduino website and manually installed)

## Sub Projects

### `36x8`

This is for the first hardware revision of the LED hat that uses 8 strips with 36 WS2812B LEDs each at a density of 60 LEDs per meter on an Arduino Uno later on an Arduino Nano.
Each strip is connected to its own digital output pin so the row of the pixel directly corresponds to the output pin while each row has a chain of 36 pixels.

### `64x8`

This is for the second hardware revision of the LED hat that uses an ESP8266 with two chained 32x8 flexible LED Panels and only uses one digital output for all 512 LEDs and the index of the LEDs in the array zig-zags through the LED Panel column by column so a position mapping function was added. The ESP8266 has issues with the floating point arithmetic used in some animations, some have been adjusted to account for that while others still need to be addressed. The control scheme has shifted from another decive using serial to providing a WIFI access point with one command port and one data port for pixelflut. The command port has no authorization or encryption at all. The increased number of LEDs makes this unable to run on an Arduino Uno or Nano because of the both RAM and ROM requirements which is why the previous version did not have text.

### `LEDStrips`

some random stuff for WS2812B LED strips

### `LED_RING`

some patterns for rings of WS2812B LEDs using potentiometers and buttons to control several parameters

### `TFT`

some tests with the arduino tft touch screen  module one of which evolved to be the controller for the first hardware revision of the LED hat

### others

the rest of the things in here were just some tests, some work some don't some never got to the point of working (like midiPlayerFromSerial)
