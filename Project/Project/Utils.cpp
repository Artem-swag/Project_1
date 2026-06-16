#include "Utils.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <fstream>
#include <random>


void waitForEnter() {
    using namespace std;
    if (cin.fail()) {
        cin.clear();
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\nНажмите Enter для продолжения...";
    char ch;
    while (cin.get(ch) && ch != '\n') {
    }
}


int manualInput(int minVal, int maxVal) {
    using namespace std;
    int n;
    while (true) {
        cout << "Введите количество чисел (от " << minVal << " до " << maxVal << "): ";

        if (cin >> n) {
            if (n >= minVal && n <= maxVal) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return n;
            }
            else {
                cout << "Ошибка: число должно быть в диапазоне [" << minVal << ", " << maxVal << "]\n";
            }
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: введите целое число.\n";
        }
    }
}


int randomInput(int minVal, int maxVal) {
    using namespace std;
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(minVal, maxVal);

    int n = dis(gen);
    cout << "Сгенерировано случайное число: " << n << endl;
    return n;
}


int fileInput() {
    using namespace std;
    string filename;

    cout << "Введите имя файла для чтения (например, input.txt): ";
    getline(cin, filename);

    if (filename.empty()) {
        filename = "input.txt";
        cout << "Имя файла не введено. Используется файл по умолчанию: " << filename << endl;
    }

    ifstream file(filename);

    if (!file.is_open()) {
        throw InvalidInputException("Не удалось открыть файл: " + filename);
    }

    int n;
    file >> n;

    if (file.fail()) {
        throw InvalidInputException("Файл '" + filename + "' не содержит корректного числа");
    }

    char leftover;
    if (file >> leftover) {
        throw InvalidInputException("Файл '" + filename + "' содержит лишние символы после числа");
    }

    file.close();

    if (n < 1 || n > 90) {
        throw InvalidInputException("Число в файле должно быть в диапазоне 1-90 (текущее: " + to_string(n) + ")");
    }

    cout << "Прочитано из файла '" << filename << "': " << n << endl;
    return n;
}


void printResults(const std::vector<unsigned long long>& results) {
    using namespace std;

    if (results.empty()) {
        cout << "Нет данных для вывода.\n";
        return;
    }

    cout << "\nЧисла Хэмминга (только множители 2, 3, 5)\n";
    cout << "Найдено чисел: " << results.size() << "\n\n";

    cout << left << setw(8) << "N" << "Значение\n";

    for (size_t i = 0; i < results.size(); ++i) {
        cout << setw(8) << (i + 1) << results[i] << "\n";

        if ((i + 1) % 20 == 0 && i + 1 < results.size()) {
            cout << "Нажмите Enter для продолжения...";
            cin.get();
        }
    }
}


void saveResultsToFile(const std::vector<unsigned long long>& results) {
    using namespace std;
    string filename;

    cout << "Введите имя файла для сохранения (например, result.txt): ";
    getline(cin, filename);

    if (filename.empty()) {
        filename = "hamming_results.txt";
        cout << "Имя файла не введено. Используется файл по умолчанию: " << filename << endl;
    }

    ofstream file(filename);

    if (!file.is_open()) {
        throw ComputationException("Не удалось создать файл: " + filename);
    }

    file << "Числа Хэмминга (множители 2, 3, 5)\n";
    file << "Всего чисел: " << results.size() << "\n";


    file << left << setw(8) << "N" << "Значение\n";

    for (size_t i = 0; i < results.size(); ++i) {
        file << setw(8) << (i + 1) << results[i] << "\n";
    }

    file.close();
    cout << "Результаты сохранены в файл: " << filename << endl;
}


// ПРОВЕРКА КОРРЕКТНОСТИ ЧИСЕЛ

// Алгоритм проверки: последовательно делим число на 2, 3 и 5
// Если после всех делений остается 1 - число состоит только из множителей 2,3,5
// Если остается другое число - есть посторонние множители


void verifyNumbers(const std::vector<unsigned long long>& results) {
    using namespace std;

    cout << "\nПроверка корректности\n";

    size_t countToShow = min((size_t)10, results.size());

    for (size_t i = 0; i < countToShow; ++i) {
        unsigned long long num = results[i];
        unsigned long long temp = num;

        // Последовательно удаляем множители 2, 3 и 5
        while (temp % 2 == 0) temp /= 2;
        while (temp % 3 == 0) temp /= 3;
        while (temp % 5 == 0) temp /= 5;

        // Если осталась 1 - число корректно
        cout << num << " -> " << (temp == 1 ? "[OK]" : "[FAIL]") << "\n";
    }

    if (results.size() > 10) {
        cout << "... и еще " << (results.size() - 10) << " чисел(а)\n";
    }
    cout << endl;
}


int showMainMenu() {
    using namespace std;
    int choice;

    cout << "\n";
    cout << "ГЕНЕРАЦИЯ ЧИСЕЛ ХЭММИНГА (множители 2, 3, 5)\n";
    cout << "\n";
    cout << "1. Ввод n вручную\n";
    cout << "2. Случайное n (от 1 до 90)\n";
    cout << "3. Чтение n из файла (имя файла вводится вручную)\n";
    cout << "4. Назад в главное меню\n";
    cout << "\n";
    cout << "Выберите способ ввода (1-4): ";

    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return choice;
}