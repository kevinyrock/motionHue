# motionHue
Code for ESP8266 ([Adafruit Huzzah](https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/overview)) to control Phillips Hue lights using [Sparkfun ZX Gesture Sensor](https://www.sparkfun.com/products/12780).  
This code uses I2C for communication which happens to be Pins 4 (SDA) and 5 (SCL) 

# Setup

After cloning this repository:
```bash
pip install -U platformio

cd motionHue

mkdir -p lib/Auth
nano /lib/Auth/authentication.h
```
Inside of the authentication.h file, fill in the following:

```bash
const char* ssid = "YOUR_SSID"
const char* pass = "YOUR_PASSWORD"
```

With that done, simply run platformio from the motionHue root directory:

```bash
platformio run
```

After successful compilation, follow platformio docs for uploading.  Most likely:
```bash
platformio run -t upload
```

#TODO
* Document wiring
* Implement gesture recognition
* Create & Upload 3D enclosure




