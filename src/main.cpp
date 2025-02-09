#include "main.h"

void setup()
{
	// Запуск датчиков
  	dht.begin();
	//rtc.begin();
	//rtc.setTime(COMPILE_TIME);
	lcd.init();
	barometer.begin();
	cond.setH(ALTITUDE);
	Serial.begin(9600);

	// Натройка дисплея
	lcd.backlight();
	lcd.setCursor(0, 0);
	lcd.print("Weather station");
	lcd.setCursor(0, 1);
	lcd.print("kvadrat");
	delay(2000);
	lcd.clear();

}

void loop()
{
	// Основной цикл
	checkSensors();
	printSerial();
	printLCD();
	delay(INTERVAL);
}