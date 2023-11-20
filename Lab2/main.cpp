#include <fstream>
#include <iostream>

#include "Gramatica.h"

void Menu(Gramatica& gramatica)
{
		uint8_t type;
		std::cout
			<< "---------------------------------------\n"
			<< "|                Menu                 |\n"
			<< "---------------------------------------\n"
			<< "|     Command    |        Means       |\n"
			<< "---------------------------------------\n"
			<< "|       0        |       Display      |\n"
			<< "|                |     the Grammar    |\n"
			<< "---------------------------------------\n"
			<< "|       1        |     Generate n     |\n"
			<< "|                |  words in Grammar  |\n"
			<< "---------------------------------------\n"
			<< "|       2        |  Make and display  |\n"
			<< "|                |   the Automaton    |\n"
			<< "---------------------------------------\n"
			<< "|                |  Verify a word is  |\n"
			<< "|	    3        |   accepted for an  |\n"
			<< "|	    	     |      Automaton     |\n"
			<< "---------------------------------------\n"
			<< "|                |  Generate word in  |\n"
			<< "|       4        | Grammar and verify |\n"
			<< "|                | accepted for an is |\n"
			<< "|                |      Automaton     |\n"
			<< "---------------------------------------\n"
			<< "|       5        |        Exit        |\n"
			<< "---------------------------------------\n";

		std::cin >> type;
		switch (type)
		{
		case 0:
		{
			std::cout << "\n";
			break;
		}
		case 1:
		{
			std::cout << "\n";
			break;
		}
		case 2:
		{
			std::cout << "\n";
			break;
		}
		case 3:
		{
			std::cout << "\n";
			break;
		}
		case 4:
		{
			std::cout << "\n";
			break;
		}
		default:
			break;
		}
}

int main()
{
	std::ifstream fin("intrare.txt");
	Gramatica gramatica;

	fin >> gramatica;
	std::cout << gramatica << std::endl;

	gramatica.generare();

}