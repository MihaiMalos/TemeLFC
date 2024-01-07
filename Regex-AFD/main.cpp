#include <fstream>
#include "RegularExpression.h"

void menu(RegularExpression& regex)
{
	std::cout << "Optiuni:\n";
	std::cout << "1 - afisarea automatului\n";
	std::cout << "2 - afisarea expresiei regulate\n";
	std::cout << "3 - verificarea unui cuvant in automat\n";
	std::cout << "0 - inchiderea meniului\n";
	DFA deterministicAutomaton{ regex.ConvertToAutomaton() };
	bool quit = false;
	while (!quit)
	{
		std::cout << "Introduceti o optiune: ";
		std::string option;
		std::cin >> option;
		if (option.size() > 1) // this could be done better
		{
			option[0] = 'x';
		}
		switch (option[0])
		{
		case '1':
			std::cout << deterministicAutomaton << "\n\n";
			break;
		case '2':
			std::cout << regex << "\n\n";
			break;
		case '3':
		{
			std::cout << "Introduceti un cuvant: ";
			std::string word;
			std::cin >> word;
			if (deterministicAutomaton.VerifyWord(word))
			{
				std::cout << "acceptat\n\n";
			}
			else
			{
				std::cout << "nu este acceptat\n\n";
			}
			break;
		}
		case '0':
			quit = true;
			break;
		default:
			std::cout << "Alegeti o optiune valida!\n\n";
			break;
		}
	}
}

int main()
{
	std::ifstream file("regexFile.txt");
	RegularExpression regex;
	file >> regex;
	file.close();
	if (regex.IsValid())
	{
		menu(regex);
	}
	else
	{
		std::cout << "Expresia citita nu este valida!";
	}
	return 0;
}