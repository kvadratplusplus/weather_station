// Подключение необходимых библиотек
#include <Arduino.h>
#include <microDS3231.h>
#include "DHT.h"
#include "Adafruit_Sensor.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SFE_BMP180.h>

// Определение констант
#define DHTTYPE DHT22
#define DHTPIN 6
#define CLK 5
#define DIO 4

#define INTERVAL 60000
#define ALTITUDE 47
#define HPA_IN_MMHG 1.333

//#define SPPLOTTER

// Прототипы функций
double getPressure();
float runMiddleArifm(float newVal);

// Создание объектов
DHT dht(DHTPIN, DHTTYPE);
MicroDS3231 rtc;
LiquidCrystal_I2C lcd(0x27,16,2);
SFE_BMP180 barometer;

// Создание переменных
DateTime now;
byte hrs;
byte mins;
#ifndef SPPLOTTER
byte day;
byte month;
#endif

float temp;
int humd;
float pressure;

// Снятие показаний с датчиков
void checkSensors()
{
	temp = dht.readTemperature();
	humd = dht.readHumidity();

	now = rtc.getTime();
	hrs = now.hour;
	mins = now.minute;

#ifndef SPPLOTTER
	day = now.date;
	month = now.month;
#endif

	pressure = getPressure();
	pressure = runMiddleArifm(pressure);
	
}

// Алгоритм округления
float runMiddleArifm(float newVal)
{
	static int t = 0;
	static float vals[10];
	static float average = 0;

	if(average == 0) for(int i = 0; i < 10; i++)
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
					return(P - (ALTITUDE / 12 * HPA_IN_MMHG));
				}
				// Вывод возможных ошибок
				else Serial.println("error retrieving pressure measurement\n");
			}
			else Serial.println("error starting pressure measurement\n");
		}
		else Serial.println("error retrieving temperature measurement\n");
	}
	else Serial.println("error starting temperature measurement\n");
}

// Печать в COM порт
void printSerial()
{
	#ifndef SPPLOTTER
	Serial.print(hrs);
	Serial.print(" ");
	Serial.print(mins);
	Serial.print(" ");

	Serial.print(temp);
	Serial.print(" ");
	Serial.print(humd);
	Serial.print(" ");
	Serial.print(pressure);
	Serial.println(";");
	#endif

	#ifdef SPPLOTTER
	// Тоже самое, только отформатированное для spplotter
	Serial.print("$");
	Serial.print((int)temp);
	Serial.print(" ");
	Serial.print(humd);
	Serial.print(" ");
	Serial.print(int(pressure / 10));
	Serial.println(";");
	#endif
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
	lcd.print(hrs);
	lcd.print(":");
	if(mins < 10)
		lcd.print("0");
	lcd.print(mins);

	lcd.setCursor(9, 1);
	lcd.print("P ");
	lcd.print(pressure / HPA_IN_MMHG, 0);
	lcd.print("mm");
}