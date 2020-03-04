#include "HX711.h"
#include <LiquidCrystal.h> // Подключаем библиотеку работы с символьными дисплеями
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // (RS, E, D4, D5, D6, D7) подключаем выходы дисплея согласно последовательнос
// создание экземпляра объекта

// 13 – пин подключения DT

// 12 – пин подключения SCK

const int LOADCELL_DOUT_PIN = 13;
const int LOADCELL_SCK_PIN = 12;
HX711 scale;

// значение калибровочного коэффициента (делать подбор для своих весов)

float scale_calibration = -1.5456;

// переменные

float weight_units;

float weight_gr;



void setup() {

// Инициализируем последовательный порт

lcd.begin(16, 2); // Инициализируем LCD 1602

lcd.print("calibration");
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
delay (5000);

scale.set_scale();

// Сбрасываем весы на 0

scale.tare();



}



void loop() {
	lcd.clear();

// Устанавливаем калибровочный коэффициент

scale.set_scale(scale_calibration);

// получить данные


weight_units = scale.get_units(), 50;

// если <0 то 0
/*
if (weight_units < 0) {

weight_units = 0.00;

}
*/
// перевод из унций в граммы

weight_gr = weight_units * 0.035274;





lcd.print(weight_gr);

lcd.write(" gr");
delay(1000);
}