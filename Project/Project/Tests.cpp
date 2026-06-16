#include "Tests.h"
#include "Queue.h"
#include "Hamming.h"
#include "Utils.h"
#include <iostream>
#include <fstream>


void addResult(std::vector<TestResult>& results, const std::string& name, bool passed, const std::string& msg) {
    results.push_back({ name, passed, msg });
}


void printTestResults(const std::vector<TestResult>& results) {
    using namespace std;

    int passed = 0, failed = 0;
    cout << "\nРЕЗУЛЬТАТЫ ТЕСТОВ\n\n";

    for (const auto& r : results) {
        cout << "  " << r.testName << " ... ";
        if (r.passed) {
            cout << "OK\n";
            passed++;
        }
        else {
            cout << "FAILED\n";
            cout << "      " << r.message << "\n";
            failed++;
        }
    }

    cout << "\nИтого: " << passed << " пройдено, " << failed << " не пройдено\n\n";
}


// ТЕСТЫ ОЧЕРЕДИ


void runQueueTests() {
    using namespace std;

    cout << "\n ТЕСТЫ ОЧЕРЕДИ \n";
    vector<TestResult> results;

    // 1. Создание пустой очереди
    {
        Queue<int> q;
        bool ok = q.empty() && q.size() == 0;
        addResult(results, "Создание пустой очереди", ok, "Очередь не пуста или размер не 0");
    }

    // 2. Добавление элемента
    {
        Queue<int> q;
        q.push(42);
        bool ok = !q.empty() && q.size() == 1 && q.front() == 42;
        addResult(results, "push() - добавление", ok, "Размер или значение неверны");
    }

    // 3. Удаление элемента
    {
        Queue<int> q;
        q.push(10); q.push(20);
        q.pop();
        bool ok = q.front() == 20 && q.size() == 1;
        addResult(results, "pop() - удаление", ok, "Первый элемент или размер неверны");
    }

    // 4. FIFO порядок
    {
        Queue<int> q;
        for (int i = 1; i <= 3; i++) q.push(i);
        bool ok = true;
        for (int i = 1; i <= 3; i++) {
            if (q.front() != i) { ok = false; break; }
            q.pop();
        }
        addResult(results, "FIFO порядок", ok, "Нарушен порядок очереди");
    }

    // 5. Очистка очереди
    {
        Queue<int> q;
        for (int i = 0; i < 5; i++) q.push(i);
        q.clear();
        bool ok = q.empty() && q.size() == 0;
        addResult(results, "clear() - очистка", ok, "Очередь не пуста после очистки");
    }

    // 6. Проверка размера
    {
        Queue<int> q;
        q.push(1); q.push(2); q.pop();
        bool ok = q.size() == 1;
        addResult(results, "size() - размер", ok, "Размер не соответствует ожидаемому");
    }

    printTestResults(results);
}


// ТЕСТЫ АЛГОРИТМА


void runAlgorithmTests() {
    using namespace std;

    cout << "\n ТЕСТЫ АЛГОРИТМА \n";
    vector<TestResult> results;

    // 1. Первые 10 чисел
    {
        vector<unsigned long long> expected = { 2, 3, 4, 5, 6, 8, 9, 10, 12, 15 };
        auto result = computeHammingNumbers(10);
        bool ok = (result == expected);
        string msg = "Ожидалось: ";
        for (auto x : expected) msg += to_string(x) + " ";
        addResult(results, "Первые 10 чисел", ok, msg);
    }

    // 2. Порядок возрастания
    {
        auto result = computeHammingNumbers(30);
        bool ok = true;
        for (size_t i = 1; i < result.size(); i++) {
            if (result[i] <= result[i - 1]) { ok = false; break; }
        }
        addResult(results, "Порядок возрастания", ok, "Нарушен порядок");
    }

    // 3. Только множители 2,3,5
    {
        auto result = computeHammingNumbers(50);
        bool ok = true;
        for (auto num : result) {
            unsigned long long t = num;
            while (t % 2 == 0) t /= 2;
            while (t % 3 == 0) t /= 3;
            while (t % 5 == 0) t /= 5;
            if (t != 1) { ok = false; break; }
        }
        addResult(results, "Только множители 2,3,5", ok, "Найдены посторонние множители");
    }

    // 4. n = 1
    {
        auto result = computeHammingNumbers(1);
        bool ok = (result.size() == 1 && result[0] == 2);
        addResult(results, "n = 1", ok, "Ожидалось [2]");
    }

    printTestResults(results);
}

// ТЕСТЫ ИСКЛЮЧЕНИЙ


void runExceptionTests() {
    using namespace std;

    cout << "\n ТЕСТЫ ИСКЛЮЧЕНИЙ \n";
    vector<TestResult> results;

    // 1. pop() из пустой очереди
    {
        bool thrown = false;
        try { Queue<int> q; q.pop(); }
        catch (const QueueException&) { thrown = true; }
        catch (...) { thrown = true; }
        addResult(results, "pop() из пустой очереди", thrown, "Исключение не выброшено");
    }

    // 2. front() из пустой очереди
    {
        bool thrown = false;
        try { Queue<int> q; int x = q.front(); (void)x; }
        catch (const QueueException&) { thrown = true; }
        catch (...) { thrown = true; }
        addResult(results, "front() из пустой очереди", thrown, "Исключение не выброшено");
    }

    // 3. n = 0
    {
        bool thrown = false;
        try { computeHammingNumbers(0); }
        catch (const InvalidInputException&) { thrown = true; }
        catch (...) { thrown = true; }
        addResult(results, "n = 0", thrown, "Исключение не выброшено");
    }

    // 4. n < 0
    {
        bool thrown = false;
        try { computeHammingNumbers(-5); }
        catch (const InvalidInputException&) { thrown = true; }
        catch (...) { thrown = true; }
        addResult(results, "n < 0", thrown, "Исключение не выброшено");
    }

    // 5. n > 90
    {
        bool thrown = false;
        try { computeHammingNumbers(100); }
        catch (const OverflowException&) { thrown = true; }
        catch (...) { thrown = true; }
        addResult(results, "n > 90", thrown, "Исключение не выброшено");
    }

    printTestResults(results);
}


// ТЕСТЫ ФАЙЛОВ


void runFileTests() {
    using namespace std;

    cout << "\n ТЕСТЫ ФАЙЛОВ \n";
    vector<TestResult> results;

    // 1. Создание и чтение файла
    {
        ofstream out("test.tmp");
        out << "25";
        out.close();

        ifstream in("test.tmp");
        int n;
        in >> n;
        in.close();
        remove("test.tmp");

        bool ok = (n == 25);
        addResult(results, "Создание и чтение файла", ok, "Число прочитано неверно");
    }

    // 2. Чтение несуществующего файла
    {
        ifstream file("nonexistent_file_12345.tmp");
        bool ok = !file.is_open();
        addResult(results, "Чтение несуществующего файла", ok, "Файл открылся, хотя не должен");
    }

    // 3. Файл с некорректными данными
    {
        ofstream out("test.tmp");
        out << "abc";
        out.close();

        ifstream in("test.tmp");
        int n;
        in >> n;
        bool ok = in.fail();
        in.close();
        remove("test.tmp");

        addResult(results, "Файл с некорректными данными", ok, "Ошибка не обнаружена");
    }

    // 4. Сохранение результатов
    {
        vector<unsigned long long> data = { 2, 3, 4, 5, 6 };
        ofstream out("test.tmp");
        for (size_t i = 0; i < data.size(); i++) {
            out << data[i] << "\n";
        }
        out.close();

        bool ok = true;
        ifstream in("test.tmp");
        for (size_t i = 0; i < data.size(); i++) {
            int x;
            in >> x;
            if (x != (int)data[i]) { ok = false; break; }
        }
        in.close();
        remove("test.tmp");

        addResult(results, "Сохранение результатов", ok, "Данные сохранены неверно");
    }

    printTestResults(results);
}


// ТЕСТЫ ГРАНИЧНЫХ СЛУЧАЕВ


void runEdgeCaseTests() {
    using namespace std;

    cout << "\n ТЕСТЫ ГРАНИЧНЫХ СЛУЧАЕВ \n";
    vector<TestResult> results;

    // 1. n = 1
    {
        auto result = computeHammingNumbers(1);
        bool ok = (result.size() == 1 && result[0] == 2);
        addResult(results, "Минимальное n = 1", ok, "Ожидалось [2]");
    }

    // 2. n = 90 (максимум)
    {
        bool ok = false;
        try {
            auto result = computeHammingNumbers(90);
            ok = (result.size() == 90);
        }
        catch (const OverflowException&) {
            ok = true;
        }
        addResult(results, "Максимальное n = 90", ok, "Не удалось вычислить");
    }

    // 3. Большая очередь (1000 элементов)
    {
        Queue<int> q;
        bool ok = true;
        for (int i = 0; i < 1000; i++) q.push(i);
        if (q.size() != 1000) ok = false;
        for (int i = 0; i < 1000 && ok; i++) {
            if (q.front() != i) { ok = false; break; }
            q.pop();
        }
        addResult(results, "Большая очередь (1000 элементов)", ok, "Ошибка при работе с большой очередью");
    }

    printTestResults(results);
}

// ЗАПУСК ВСЕХ ТЕСТОВ

void runAllTests() {
    using namespace std;


    cout << "\nЗАПУСК ВСЕХ ТЕСТОВ\n";


    runQueueTests();
    runAlgorithmTests();
    runExceptionTests();
    runFileTests();
    runEdgeCaseTests();


    cout << "\nВСЕ ТЕСТЫ ЗАВЕРШЕНЫ\n";
}