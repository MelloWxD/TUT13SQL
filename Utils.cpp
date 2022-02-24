#include <windows.h>
#include<iostream>
#include "Utils.h"

using namespace std;

void DebugPrint(const string& mssg1, const string& mssg2)
{
	std::cout << (mssg1.c_str());
	std::cout << ("\n");
	if (!mssg2.empty()) {
		std::cout << (mssg2.c_str());
		std::cout << ("\n");
	}
}