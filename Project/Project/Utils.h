#pragma once
#include "Hamming.h"
#include <vector>
#include <string>


void waitForEnter();

int
manualInput(int minVal = 1, int maxVal = 90);
int randomInput(int minVal = 1, int maxVal = 90);
int fileInput();


void printResults(const std::vector<unsigned long long>& results);
void saveResultsToFile(const std::vector<unsigned long long>& results);
void verifyNumbers(const std::vector<unsigned long long>& results);


int showMainMenu();