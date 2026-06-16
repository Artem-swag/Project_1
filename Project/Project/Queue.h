#pragma once
#include <exception>
#include <string>
#include <iostream>


// СОБСТВЕННЫЙ КЛАСС ИСКЛЮЧЕНИЙ ДЛЯ ОЧЕРЕДИ

// Наследуем std::exception для совместимости со стандартной обработкой


class QueueException : public std::exception {
private:
    std::string message;
public:
    explicit QueueException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};


// ШАБЛОННЫЙ КЛАСС ОЧЕРЕДЬ (СОБСТВЕННАЯ РЕАЛИЗАЦИЯ)

// Реализует структуру данных "очередь" (FIFO - First In, First Out)
// Вместо использования std::queue, реализуем свою очередь на динамических узлах


template <typename T>
class Queue {
private:
    // ВНУТРЕННЯЯ СТРУКТУРА УЗЛА ОДНОСВЯЗНОГО СПИСКА

    struct Node {
        T data;          // Данные, хранящиеся в узле
        Node* next;      // Указатель на следующий узел (nullptr для последнего)
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;          // Указатель на первый элемент очереди (удаление отсюда)
    Node* tail;          // Указатель на последний элемент очереди (добавление сюда)
    size_t count;        // Количество элементов в очереди


    // ВСПОМОГАТЕЛЬНЫЙ МЕТОД ДЛЯ БЕЗОПАСНОЙ ОЧИСТКИ

    // Используется при ошибках для восстановления состояния
    // noexcept гарантирует, что метод не выбросит исключение

    void safeClear() noexcept {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;  // Освобождаем память, предотвращая утечки
        }
        tail = nullptr;
        count = 0;
    }

public:
    // КОНСТРУКТОР: создает пустую очередь


    Queue() : head(nullptr), tail(nullptr), count(0) {}


    // ДЕСТРУКТОР: освобождает всю выделенную память


    ~Queue() {
        safeClear();
    }


    // ЗАПРЕЩАЕМ КОПИРОВАНИЕ (ПРАВИЛО ТРЕХ)

    // Чтобы избежать двойного удаления памяти, запрещаем копирование


    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;


    // ДОБАВЛЕНИЕ ЭЛЕМЕНТА В КОНЕЦ ОЧЕРЕДИ

    // Сложность: O(1) - амортизированная
    // Алгоритм: создаем новый узел, добавляем его после tail, обновляем tail


    void push(const T& value) {
        try {
            Node* newNode = new Node(value);  // Может выбросить std::bad_alloc

            if (empty()) {
                // Очередь была пуста: новый узел становится и head, и tail
                head = tail = newNode;
            }
            else {
                // Присоединяем новый узел в конец
                tail->next = newNode;
                tail = newNode;  // Обновляем хвост
            }
            count++;
        }
        catch (const std::bad_alloc& e) {
            // Обработка ошибки на уровне класса с восстановлением
            safeClear();
            throw QueueException(std::string("Queue::push: ") + e.what());
        }
        catch (const std::exception& e) {
            safeClear();
            throw QueueException(std::string("Queue::push: ") + e.what());
        }
        catch (...) {
            safeClear();
            throw QueueException("Queue::push: неизвестная ошибка");
        }
    }


    // УДАЛЕНИЕ ЭЛЕМЕНТА ИЗ НАЧАЛА ОЧЕРЕДИ

    // Сложность: O(1)
    // Алгоритм: сохраняем head, перемещаем head на следующий узел, удаляем старый


    void pop() {
        if (empty()) {
            throw QueueException("Queue::pop: очередь пуста");
        }

        Node* temp = head;      // Запоминаем удаляемый узел
        head = head->next;      // Перемещаем голову на следующий элемент
        delete temp;            // Освобождаем память
        count--;

        if (empty()) {
            tail = nullptr;     // Очередь стала пустой
        }
    }


    // ДОСТУП К ПЕРВОМУ ЭЛЕМЕНТУ (НЕУДАЛЯЮЩИЙ)

    // Сложность: O(1)


    T& front() {
        if (empty()) {
            throw QueueException("Queue::front: очередь пуста");
        }
        return head->data;
    }

    const T& front() const {
        if (empty()) {
            throw QueueException("Queue::front: очередь пуста");
        }
        return head->data;
    }


    // ПРОВЕРКА НА ПУСТОТУ


    bool empty() const noexcept {
        return head == nullptr;
    }



    // ПОЛУЧЕНИЕ РАЗМЕРА ОЧЕРЕДИ


    size_t size() const noexcept {
        return count;
    }


    // ОЧИСТКА ОЧЕРЕДИ


    void clear() noexcept {
        safeClear();
    }


    // ВОССТАНОВЛЕНИЕ ПОСЛЕ ОШИБКИ


    void recover() noexcept {
        safeClear();
    }
};