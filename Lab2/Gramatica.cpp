#include <iostream>
#include <random>

#include "Gramatica.h"

std::istream& operator>>(std::istream& in, Gramatica& gramatica)
{
	int nrTerminale, nrNeterminale, nrProductii;
	char simbol;

	in >> nrNeterminale;
	for (int index = 0; index < nrNeterminale; index++)
	{
		in >> simbol;
		gramatica.m_neterminale.insert(simbol);
	}

	in >> nrTerminale;
	for (int index = 0; index < nrTerminale; index++)
	{
		in >> simbol;
		gramatica.m_terminale.insert(simbol);
	}

	in >> gramatica.m_simbolStart;
	in >> nrProductii;

	for (int index = 0; index < nrProductii; index++)
	{
		std::string membruStang, membruDrept;
		in >> membruStang >> membruDrept;
		gramatica.m_productii.emplace(membruStang, membruDrept);
	}

	return in;
}
std::ostream& operator<<(std::ostream& out, const Gramatica& gramatica)
{
	out << "G = ({";
	for (auto& it : gramatica.m_neterminale)
	{
		out << it;
		if (it != *gramatica.m_neterminale.rbegin())
			out << ",";
	}
	out << "},";

	out << "{";
	for (auto it : gramatica.m_terminale)
	{
		out << it;
		if (it != *gramatica.m_terminale.rbegin())
			out << ",";
	}
	out << "},";

	out << gramatica.m_simbolStart << ", P), P continand urmatoarele productii:\n";

	int index = 0;
	for (auto it : gramatica.m_productii)
	{
		out << "(" << ++index << ") ";
		out << it.first << " -> " << it.second << std::endl;
	}

	return out;
}

bool Gramatica::VerificareCond1()
{
	for (auto terminal : m_terminale)
		if (m_neterminale.find(terminal) != m_neterminale.end())
			return false;
	return true;
}
bool Gramatica::VerificareCond2()
{
	return m_neterminale.find(m_simbolStart) == m_neterminale.end();
}
bool Gramatica::VerificareCond3()
{
	std::regex myPattern(R"([a-z]*[A-Z]+[a-z]*)");
	for (auto productie : m_productii)
		if (!std::regex_match(productie.first, myPattern))
			return false;
	return true;
}
bool Gramatica::VerificareCond4()
{
	std::string simbol(1, m_simbolStart);
	for (auto productie : m_productii)
		if (productie.first == simbol)
			return true;
	return false;
}
bool Gramatica::VerificareCond5()
{
	for (auto productie : m_productii)
	{
		std::string firstMember = productie.first;
		std::string secondMember = productie.second;

		for (char neterminal : m_neterminale)
		{
			while(!firstMember.empty() && firstMember.find(neterminal) != std::string::npos)
				firstMember.erase(firstMember.find(neterminal));

			while (!secondMember.empty() && secondMember.find(neterminal) != std::string::npos)
				secondMember.erase(secondMember.find(neterminal));

			if (firstMember.empty())
				break;
		}

		if (!firstMember.empty())
			return false;

		for (char terminal : m_terminale)
		{
			while (!secondMember.empty() && secondMember.find(terminal) != std::string::npos)
				secondMember.erase(secondMember.find(terminal));

			if (secondMember.empty())
				break;
		}

		if (!secondMember.empty())
			return false;
	}

	return true;
}

bool Gramatica::verificare()
{
	if (!VerificareCond1() ||
		!VerificareCond2() ||
		!VerificareCond3() ||
		!VerificareCond4() ||
		!VerificareCond5())
		return false;	
	return true;
}

void Gramatica::generare()
{
	std::string word = std::string(&m_simbolStart);
	std::cout << word;

	std::vector<Productie> possibleProductions;
	while (true)
	{
		for (auto production : m_productii)
		{
			if (word.find(production.first, 0) != std::string::npos)
			{
				possibleProductions.push_back(production);
			}
		}

		if (possibleProductions.empty()) break;
		std::cout << " -> ";

		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(0, possibleProductions.size() - 1);

		Productie selectedProduction = possibleProductions[distr(eng)];

		std::vector<int> possiblePositions;

		int currentPosition = word.find(selectedProduction.first, 0);
		while (currentPosition != std::string::npos)
		{
			possiblePositions.push_back(currentPosition);
			currentPosition = word.find(selectedProduction.first, currentPosition + 1);
		}

		distr = std::uniform_int_distribution<>(0, possiblePositions.size() - 1);

		int selectedPosition = possiblePositions[distr(eng)];
		word.replace(selectedPosition, selectedProduction.first.size(), selectedProduction.second);
		std::cout << word;

		possibleProductions.clear();
	}


}

bool Gramatica::isRegularGrammar()
{
	for (const auto& productie : m_productii)
	{
		const std::string& membruStang = productie.first;
		const std::string& membruDrept = productie.second;

		if (membruDrept.size() > 2 || (membruDrept.size() == 2 && m_neterminale.find(membruDrept[1]) == m_neterminale.end()))
		{
			return false;
		}

		if (membruStang.size() != 1 || m_neterminale.find(membruStang[0]) == m_neterminale.end())
		{
			return false;
		}
	}

	for (const auto& productie : m_productii)
	{
		if (productie.second == "&") // "&" represent epsilon
		{
			const char& neterminal = productie.first[0];
			for (const auto& altaProductie : m_productii)
			{
				if (altaProductie.first[0] == neterminal && altaProductie.second != "&")
				{
					return false;
				}
			}
		}
	}

	return true;
}
