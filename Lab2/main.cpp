#include <fstream>
#include <iostream>

#include "Grammar.h"
#include "FiniteAutomaton.h"

void Menu(Grammar& gramatica)
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
	Grammar grammar;

	fin >> grammar;
	std::cout << grammar.VerifyGrammer() << std::endl;
	std::cout << grammar.IsRegular() << std::endl;
	std::cout << grammar << std::endl;

	FiniteAutomaton automaton;
	automaton.CreateAutomaton(grammar);
	std::cout << automaton.VerifyAutomaton() << std::endl;
	std::cout << automaton;


}