#include <fstream>
#include <iostream>

#include "Grammar.h"
#include "FiniteAutomaton.h"

void Menu(Grammar& grammar)
{
		
		int type;
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
				<< "|       3        |   accepted for an  |\n"
				<< "|                |      Automaton     |\n"
				<< "---------------------------------------\n"
				<< "|                |  Generate word in  |\n"
				<< "|       4        | Grammar and verify |\n"
				<< "|                | accepted for an is |\n"
				<< "|                |      Automaton     |\n"
				<< "---------------------------------------\n"
				<< "|       5        |        Exit        |\n"
				<< "---------------------------------------\n";
			do
			{
			std::cout << "\nIntroduce option: ";
			std::cin >> type;
			switch (type)
			{
			case 0:
			{
				std::cout << grammar;
				break;
			}
			case 1:
			{
				int n, index = 0;
				std::cout << "n = "; std::cin >> n;
				std::set<std::string> words;

				
				while (words.size() != n)
				{
					auto wordTransformations = grammar.GenerateWord();

					if (words.find(wordTransformations.back()) == words.end())
					{
						words.insert(wordTransformations.back());
					}
					else continue;

					std::cout << "[" << ++index << "] ";
					for (auto word : wordTransformations)
					{
						std::cout << word;
						if (word != wordTransformations.back())
							std::cout << " -> ";
					}

					std::cout << std::endl;
				}
				break;
			}
			case 2:
			{
				FiniteAutomaton automaton;
				automaton.CreateAutomaton(grammar);
				std::cout << automaton;
				break;
			}
			case 3:
			{
				FiniteAutomaton automaton;
				automaton.CreateAutomaton(grammar);
				std::cout << automaton.VerifyWord("aabc");
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
		} while (type != 5);
}

int main()
{
	std::ifstream fin("intrare.txt");
	Grammar grammar;

	fin >> grammar;

	if (grammar.IsRegular() && grammar.VerifyGrammer())
	{
		Menu(grammar);
	}


}