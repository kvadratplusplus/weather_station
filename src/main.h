// Подключение необходимых библиотек
#include <Arduino.h>
#include <microDS3231.h>
#include "DHT.h"
#include "Adafruit_Sensor.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SFE_BMP180.h>
#include "Forecaster.h"

// Определение констант
#define DHTTYPE DHT22
#define DHTPIN 6
#define CLK 5
#define DIO 4

#define INTERVAL 60000
#define ALTITUDE 30
#define HPA_IN_MMHG 1.333

// Прототипы функций
double getPressure();
float runMiddleArifm(float newVal);

// Создание объектов
DHT dht(DHTPIN, DHTTYPE);
MicroDS3231 rtc;
LiquidCrystal_I2C lcd(0x27,16,2);
SFE_BMP180 barometer;
Forecaster cond;

// Создание переменных
DateTime now;
byte hrs;
byte mins;
byte day;
byte month;

int humd;		// относительная влажность %
float temp;		// температура гр. Цельсия
float pressure; // гПа
float presZam;  // гПа * 100
float presLCD;  // мм рт. ст.

// Снятие показаний с датчиков
void checkSensors()
{
	static uint32_t tmr;

	temp = dht.readTemperature();
	humd = dht.readHumidity();

	now = rtc.getTime();
	hrs = now.hour;
	mins = now.minute;
	day = now.date;
	month = now.month;

	pressure = getPressure();
	pressure = runMiddleArifm(pressure);

	presZam = (pressure * 100);
	presLCD = (pressure - ((ALTITUDE * HPA_IN_MMHG) / 12));

  if (millis() - tmr >= 30*60*1000ul)
  {
    tmr = millis();
    // каждые 30 минут передаём текущее давление (Па) и температуру (С) с датчика
	cond.setMonth(month);
    cond.addP(presZam, temp);
  }
}

// Алгоритм округления
float runMiddleArifm(float newVal)
{
	static int t = 0;
	static float vals[10];
	static float average = 0;

	if(average == 0)
		for(int i = 0; i < 10; i++)
		{
			vals[i] = newVal;
			average += vals[i];
		}

	if (++t >= 10)
		t = 0;
	average -= vals[t];
	average += newVal;
	vals[t] = newVal;

	return ((float)average / 10);
}

// Получение давления
double getPressure()
{
	char status;
	double T,P;

	status = barometer.startTemperature();

	if (status != 0)
	{
		delay(status);
		status = barometer.getTemperature(T);

		if (status != 0)
		{
			status = barometer.startPressure(3);

			if (status != 0)
			{
				delay(status);
				status = barometer.getPressure(P,T);

				if (status != 0)
				{
					return P;
				}
				// Вывод возможных ошибок
				else Serial.println(F("error retrieving pressure measurement\n"));
			}
			else Serial.println(F("error starting pressure measurement\n"));
		}
		else Serial.println(F("error retrieving temperature measurement\n"));
	}
	else Serial.println(F("error starting temperature measurement\n"));
}

// Печать в COM порт
void printSerial()
{
	Serial.print(hrs);
	Serial.print(" ");
	Serial.print(mins);
	Serial.print(" ");

	Serial.print(temp);
	Serial.print(" ");
	Serial.print(humd);
	Serial.print(" ");
	Serial.print(pressure);
	Serial.print(" ");
	Serial.print(cond.getCast());
	Serial.println(";");
}

// Вывод информации на LCD дисплей
void printLCD()
{
	lcd.setCursor(0, 0);
	lcd.print(F("T "));
	lcd.print(temp, 1);
	lcd.print(F("C"));

	lcd.setCursor(0, 1);
	lcd.print(F("H "));
	lcd.print(humd);
	lcd.print(F("%"));

	lcd.setCursor(11, 0);
	if(hrs < 10)
		lcd.print("0");
	lcd.print(hrs);
	lcd.print(":");
	if(mins < 10)
		lcd.print("0");
	lcd.print(mins);

	lcd.setCursor(11, 1);
	lcd.print("P ");
	lcd.print(presLCD / HPA_IN_MMHG, 0);

	lcd.setCursor(6, 1);
	lcd.print("Z ");
	lcd.print((int)cond.getCast());
}