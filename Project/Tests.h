#pragma once
#include <vector>
#include <string>

struct TestResult {
    std::string testName;
    bool passed;
    std::string message;
};

void runAllTests();
void runQueueTests();
void runAlgorithmTests();
void runExceptionTests();
void runFileTests();
void runEdgeCaseTests();