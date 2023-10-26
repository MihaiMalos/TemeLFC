#include <fstream>
#include <iostream>

#include "Gramatica.h"

int main()
{
	std::ifstream fin("intrare.txt");
	Gramatica gramatica;

	fin >> gramatica;
	std::cout << gramatica;
}