# The Arduino Weather Station
![](weatherStation.png)
This device displays atmospheric pressure, temperature, and humidity. It also has the capability to send data through a COM port for further analysis.  
To use the Spplotter, you need to define the SPPLOTTER macro in the src/main.h.  
If you want to adjust the altitude above sea level, modify the value of the ALTITUDE constant variable located in src/main.h.  
**Used libraries:**
  * microDS3231 by gyver
  * DHT sensor library by adafruit
  * avr-debugger by jdolinay
  * LiquidCrystal_I2C by marcoschwartz
  * Sparkfun BMP180 by sparkfun
  * Forecaster by gyver
