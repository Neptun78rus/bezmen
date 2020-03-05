#define fqPin 2 //назначаем пин датчика оборотов
#define LOADCELL_DOUT_PIN 13// 13 – пин подключения DT
#define LOADCELL_SCK_PIN 12// 12 – пин подключения SCK
#include "HX711.h"//библиотека АЦП тензодатчика 
#include <LiquidCrystal.h> // Подключаем библиотеку работы с символьными дисплеями
LiquidCrystal lcd(8, 3, 4, 5, 6, 7); // (RS, E, D4, D5, D6, D7) подключаем выходы дисплея согласно последовательнос
HX711 scale;
float scale_calibration = -15919;// значение калибровочного коэффициента (делать подбор для своих весов)
float weight_units;// переменные вес в унциях
float weight_gr;//вес в граммах

const int K = 1;

volatile unsigned long counter; // Количество отсчётов.
volatile unsigned long mks;     // Время последнего отсчёта.

unsigned long oldTime;  // Время последнего отсчёта в предыдущем вычислении
//***********Функция для обработки прерывания.

void DRIG() {
	// Здесь код прерывания
	mks=micros(); // Момент последнего отсчёта
	counter++;  // Количество отсчётов
}
//*******************************************
void setup() {
	lcd.begin(16, 2); // Инициализируем LCD 1602
	attachInterrupt(digitalPinToInterrupt(fqPin), DRIG, RISING);// Подключаем функцию на прерывание по появлению сигнала на ноге fqPin.
	lcd.print("***** TGE *****");
	scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
	delay (1000);
	scale.set_scale();
	scale.tare(); // Сбрасываем весы на 0
}
void loop() {
	lcd.clear();
	unsigned long rpm;
	// Получаем данные. 
	noInterrupts();
	unsigned long cnt = counter;
	counter = 0;
	unsigned long tmr = mks;
	interrupts();
	// Выводим частоту в оборотах в секунду
	if (cnt > 0) {
		rpm = 1000000UL / ((tmr - oldTime) / cnt) / K;
		oldTime = tmr;
	}
	else {
		rpm = 0;
	}
	lcd.print( rpm*60 );
	lcd.write(" rpm");
	scale.set_scale(scale_calibration);// Устанавливаем калибровочный коэффициент
	weight_units = scale.get_units(),50;// получить данные
	weight_gr = weight_units * 0.521556735;// перевод из унций в граммы
	lcd.setCursor(0,1);
	lcd.print(weight_gr);
	lcd.write(" NM");
	lcd.setCursor(10,1);
	lcd.print (rpm*60*weight_gr/9550,0);
	lcd.write("kW");
	delay(1000);
}