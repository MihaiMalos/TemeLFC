#include <fstream>
#include <iostream>

#include "Grammar.h"
#include "FiniteAutomaton.h"

void Menu(Grammar& grammar)
{
		
		int type;
		FiniteAutomaton automaton;
		automaton.CreateAutomaton(grammar);
			std::cout
				<< "---------------------------------------\n"
				<< "|                Menu                 |\n"
				<< "---------------------------------------\n"
				<< "|     Command    |        Means       |\n"
				<< "---------------------------------------\n"
				<< "|       1        |       Display      |\n"
				<< "|                |     the Grammar    |\n"
				<< "---------------------------------------\n"
				<< "|       2        |     Generate n     |\n"
				<< "|                |  words in Grammar  |\n"
				<< "---------------------------------------\n"
				<< "|       3        |  Make and display  |\n"
				<< "|                |   the Automaton    |\n"
				<< "---------------------------------------\n"
				<< "|                |  Verify a word is  |\n"
				<< "|       4        |   accepted for an  |\n"
				<< "|                |      Automaton     |\n"
				<< "---------------------------------------\n"
				<< "|                |  Generate word in  |\n"
				<< "|       5        | Grammar and verify |\n"
				<< "|                | accepted for an is |\n"
				<< "|                |      Automaton     |\n"
				<< "---------------------------------------\n"
				<< "|       0        |        Exit        |\n"
				<< "---------------------------------------\n";
			do
			{
			std::cout << "\nIntroduce option: ";
			std::cin >> type;
			switch (type)
			{
			case 1:
			{
				std::cout << grammar;
				break;
			}
			case 2:
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
						if (word.empty()) std::cout << "-";
						else std::cout << word;
						if (word != wordTransformations.back())
							std::cout << " -> ";
					}

					std::cout << std::endl;
				}
				break;
			}
			case 3:
			{
				std::cout << automaton;
				break;
			}
			case 4:
			{
				std::string word;

				std::cout << "Introduce word to verify: "; std::cin >> word;
				if (automaton.VerifyWord(word))
				{
					std::cout << "Word is accepted by automaton";
				}
				else
				{
					std::cout << "Word is NOT accepted by automaton";
				}
				break;
			}
			case 5:
			{
				std::string generatedWord = grammar.GenerateWord().back();
				if (generatedWord.empty()) generatedWord = "-";
				std::cout << "The word generated is: " << generatedWord << std::endl;
				if (automaton.VerifyWord(generatedWord))
				{
					std::cout << "Word is accepted by automaton";
				}
				else
				{
					std::cout << "Word is NOT accepted by automaton";
				}


				break;
			}
			default:
				break;
			}
		} while (type != 0);
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