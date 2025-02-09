// Program for reading and writing indications from com port to file
// Программа для чтения и записи показаний с com порта в файл
// >comreader.exe <comPortName>

// Code by GigaChat (https://t.me/gigachat_bot)

#include <windows.h>
#include <iostream>
#include <fstream>

int main(int argc, char const *argv[])
{
    const char* portName = argv[1];

    // Открываем COM-порт
    HANDLE hSerial = CreateFileA(portName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error open port: " << GetLastError() << std::endl;
        return 1;
    }

    // Устанавливаем таймауты
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error timeouts: " << GetLastError() << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // Настраиваем параметры связи
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error params: " << GetLastError() << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;     // Скорость передачи (9600 бод)
    dcbSerialParams.ByteSize = 8;            // Количество бит данных
    dcbSerialParams.StopBits = ONESTOPBIT;   // Стоповый бит
    dcbSerialParams.Parity = NOPARITY;       // Без проверки четности

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error configuring params: " << GetLastError() << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // Чтение данных
    char readBuffer[256]; // Буфер для хранения прочитанных данных
    DWORD bytesRead;

    std::ofstream file;

    while (true)
    {
        if (ReadFile(hSerial, readBuffer, sizeof(readBuffer), &bytesRead, NULL))
        {
            if (bytesRead > 0)
            {
                std::cout.write(readBuffer, bytesRead);
                
                file.open("out.txt", std::ios::app);
                file.write(readBuffer, bytesRead);
                file.close();
            }
        }
        else
        {
            std::cerr << "Error read data: " << GetLastError() << std::endl;
            break;
        }
    }

    // Закрытие порта
    CloseHandle(hSerial);
    

    return 0;
}
