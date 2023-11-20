#include <iostream>
#include <random>

#include "Grammar.h"

std::istream& operator>>(std::istream& in, Grammar& grammar)
{
	int nrTerminals, nrNonterminals, nrProductions;
	char symbol;

	in >> nrNonterminals;
	for (int index = 0; index < nrNonterminals; index++)
	{
		in >> symbol;
		grammar.m_nonterminals.insert(symbol);
	}

	in >> nrTerminals;
	for (int index = 0; index < nrTerminals; index++)
	{
		in >> symbol;
		grammar.m_terminals.insert(symbol);
	}

	in >> grammar.m_startSymbol;
	in >> nrProductions;

	for (int index = 0; index < nrProductions; index++)
	{
		std::string membruStang, membruDrept;
		in >> membruStang >> membruDrept;
		grammar.m_productions.emplace(membruStang, membruDrept);
	}

	return in;
}
std::ostream& operator<<(std::ostream& out, const Grammar& grammar)
{
	out << "G = ({";
	for (auto& it : grammar.m_nonterminals)
	{
		out << it;
		if (it != *grammar.m_nonterminals.rbegin())
			out << ",";
	}
	out << "},";

	out << "{";
	for (auto it : grammar.m_terminals)
	{
		out << it;
		if (it != *grammar.m_terminals.rbegin())
			out << ",";
	}
	out << "},";

	out << grammar.m_startSymbol << ", P), P continand urmatoarele productii:\n";

	int index = 0;
	for (auto it : grammar.m_productions)
	{
		out << "(" << ++index << ") ";
		out << it.first << " -> " << it.second << std::endl;
	}

	return out;
}

bool Grammar::FirstVerificationLayer()
{
	for (auto terminal : m_terminals)
		if (m_nonterminals.find(terminal) != m_nonterminals.end())
			return false;
	return true;
}
bool Grammar::SecondVerificationLayer()
{
	return m_nonterminals.find(m_startSymbol) == m_nonterminals.end();
}
bool Grammar::ThirdVerificationLayer()
{
	std::regex myPattern(R"([a-z]*[A-Z]+[a-z]*)");
	for (auto production : m_productions)
		if (!std::regex_match(production.first, myPattern))
			return false;
	return true;
}
bool Grammar::FourthVerificationLayer()
{
	std::string symbol(1, m_startSymbol);
	for (auto production : m_productions)
		if (production.first == symbol)
			return true;
	return false;
}
bool Grammar::FifthVerificationLayer()
{
	for (auto production : m_productions)
	{
		std::string firstMember = production.first;
		std::string secondMember = production.second;

		for (char nonterminal : m_nonterminals)
		{
			while(!firstMember.empty() && firstMember.find(nonterminal) != std::string::npos)
				firstMember.erase(firstMember.find(nonterminal));

			while (!secondMember.empty() && secondMember.find(nonterminal) != std::string::npos)
				secondMember.erase(secondMember.find(nonterminal));

			if (firstMember.empty())
				break;
		}

		if (!firstMember.empty())
			return false;

		for (char terminal : m_terminals)
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

bool Grammar::VerifyGrammer()
{
	if (!FirstVerificationLayer() ||
		!SecondVerificationLayer() ||
		!ThirdVerificationLayer() ||
		!FourthVerificationLayer() ||
		!FifthVerificationLayer())
		return false;	
	return true;
}

void Grammar::GenerateWord()
{
	std::string word = std::string(&m_startSymbol);
	std::cout << word;

	std::vector<Production> possibleProductions;
	while (true)
	{
		for (auto production : m_productions)
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

		Production selectedProduction = possibleProductions[distr(eng)];

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
