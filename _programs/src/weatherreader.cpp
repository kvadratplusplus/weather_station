// Additional program for calculate medium indications
// Дополнительная программа для вычисления средних показаний
// >weatherreader.exe <fileToRead>

// Подключение необходимых библиотек
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Файлы
ifstream inputFile;
ofstream outFile;

// Вектора для хранения показаний
vector<float> tempArr;
vector<int> humdArr;
vector<float> presArr;

// Функция для расчета средних показаний
void countMiddle()
{
	// Переменные для средних показаний
    float tempMed = 0.0;
    float humdMed = 0.0;
    float pressureMed = 0.0;

	// Сложение всех элементов векторов
    for(int i = 0; i < tempArr.size(); i++)
    {
        tempMed += tempArr.at(i);
        humdMed += humdArr.at(i);
        pressureMed += presArr.at(i);
    }

	// Деление на размер
    tempMed /= tempArr.size();
    humdMed /= humdArr.size();
    pressureMed /= presArr.size();

    outFile.open("medium.txt", ios::app);

	// Печать в файл
    outFile << "Medium indications" << endl;
    outFile << "Medium temp: " << tempMed << " C" << endl;
    outFile << "Medium humd: " << humdMed << " %" << endl;
    outFile << "Medium pres: " << pressureMed << " hPa" << endl;
    outFile << "Medium pres: " << pressureMed / 1.3333 << " mmHG" << endl << endl;

    outFile.close();
}

// Основная функция
int main(int argc, char const *argv[])
{
	// Переменные
    int mins = 0;
    int hour = 0;

    float temp = 0.0;
    int humd = 0.0;
    float pres = 0.0;

    inputFile.open(argv[1]);

	// Чтение файла
    while(inputFile >> hour >> mins >> temp >> humd >> pres)
    {
        tempArr.push_back(temp);
        humdArr.push_back(humd);
        presArr.push_back(pres);
    }

    countMiddle();

    inputFile.close();

	// Выход из программы
    return 0;
}