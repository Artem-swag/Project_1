#pragma once
#include "Queue.h"
#include <vector>
#include <string>
#include <exception>


class HammingException : public std::exception {
private:
    std::string message;
public:
    explicit HammingException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};


class InvalidInputException : public HammingException {
public:
    explicit InvalidInputException(const std::string& msg)
        : HammingException(msg) {}
};


class OverflowException : public HammingException {
public:
    explicit OverflowException(const std::string& msg)
        : HammingException(msg) {}
};


class ComputationException : public HammingException {
public:
    explicit ComputationException(const std::string& msg)
        : HammingException(msg) {}
};


std::vector<unsigned long long> computeHammingNumbers(int n);