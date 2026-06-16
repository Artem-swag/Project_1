#include "Utils.h"
#include "Hamming.h"
#include "Tests.h"
#include <iostream>
#include <clocale>


// ГЛАВНАЯ ФУНКЦИЯ ПРОГРАММЫ

// Реализует циклическое меню с выбором режима работы:
// 1. Рабочий режим - генерация чисел Хэмминга
// 2. Тестовый режим - запуск всех тестов
// 3. Выход из программы


int main() {
    using namespace std;


    setlocale(LC_ALL, "Russian");


    // Основной цикл программы
    while (true) {
        // Вывод главного меню
        cout << "\n";
        cout << "ГЕНЕРАЦИЯ ЧИСЕЛ ХЭММИНГА (множители 2, 3, 5)\n";
        cout << "\n";
        cout << "1. Рабочий режим (генерация чисел)\n";
        cout << "2. Тестовый режим (запуск всех тестов)\n";
        cout << "3. Выход\n";
        cout << "\n";
        cout << "Выберите режим (1-3): ";


        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');


        // Выход из программы
        if (choice == 3) {
            cout << "До свидания!\n";
            break;
        }

        // Запуск тестов
        if (choice == 2) {
            runAllTests();
            waitForEnter();
            continue;
        }

        // Проверка корректности выбора
        if (choice != 1) {
            cout << "Неверный выбор. Попробуйте снова.\n";
            waitForEnter();
            continue;
        }


        // РАБОЧИЙ РЕЖИМ: ГЕНЕРАЦИЯ ЧИСЕЛ ХЭММИНГА

        while (true) {
            // Показываем подменю выбора способа ввода
            int subChoice = showMainMenu();

            // Возврат в главное меню
            if (subChoice == 4) {
                break;
            }

            int n = 0;
            string inputMethod;

            try {
                // Выбор способа ввода значения n
                switch (subChoice) {
                case 1: {
                    inputMethod = "ручной ввод";
                    n = manualInput(1, 90);
                    break;
                }
                case 2: {
                    inputMethod = "случайное число";
                    n = randomInput(1, 90);
                    break;
                }
                case 3: {
                    inputMethod = "чтение из файла";
                    n = fileInput();
                    break;
                }
                default: {
                    cout << "Неверный выбор. Попробуйте снова.\n";
                    waitForEnter();
                    continue;
                }
                }

                // Вывод информации о вычислении
                cout << "\n--- Вычисление чисел Хэмминга ---\n";
                cout << "Способ ввода: " << inputMethod << endl;
                cout << "n = " << n << endl;

                // Вызов основного алгоритма
                vector<unsigned long long> hamming = computeHammingNumbers(n);

                // Предупреждение, если вычислено меньше чисел, чем запрошено
                if (hamming.size() < (size_t)n) {
                    cout << "\n[ПРЕДУПРЕЖДЕНИЕ] Вычислено только " << hamming.size()
                        << " чисел из " << n << " (достигнут предел представления)\n";
                }

                // Вывод результатов на экран
                printResults(hamming);
                // Проверка корректности чисел
                verifyNumbers(hamming);

                // Предложение сохранить результаты в файл
                cout << "Сохранить результаты в файл? (y/n): ";
                char saveChoice;
                cin >> saveChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (saveChoice == 'y' || saveChoice == 'Y') {
                    saveResultsToFile(hamming);
                }

            }
            catch (const InvalidInputException& e) {
                cerr << "\n[ОШИБКА ВВОДА] " << e.what() << endl;
            }
            catch (const OverflowException& e) {
                cerr << "\n[ОШИБКА ПЕРЕПОЛНЕНИЯ] " << e.what() << endl;
                cout << "Попробуйте меньшее значение n (до 90).\n";
            }
            catch (const ComputationException& e) {
                cerr << "\n[ОШИБКА ВЫЧИСЛЕНИЙ] " << e.what() << endl;
            }
            catch (const QueueException& e) {
                cerr << "\n[ОШИБКА ОЧЕРЕДИ] " << e.what() << endl;
            }
            catch (const std::exception& e) {
                cerr << "\n[ОШИБКА] " << e.what() << endl;
            }
            catch (...) {
                cerr << "\n[НЕИЗВЕСТНАЯ ОШИБКА]\n";
            }

            waitForEnter();
        }
    }

    return 0;
}
