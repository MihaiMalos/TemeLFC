#include <iostream>
#include <random>
#include <queue>

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
		std::string firstMember, secondMember;
		in >> firstMember >> secondMember;
		grammar.m_productions.emplace(firstMember, secondMember);
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
	{
		if (m_nonterminals.find(terminal) != m_nonterminals.end())
			return false;
	}
	return true;
}
bool Grammar::SecondVerificationLayer()
{
	return m_nonterminals.find(m_startSymbol) != m_nonterminals.end();
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
	for (auto production : m_productions)
		if (production.first == &m_startSymbol)
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
				firstMember.erase(firstMember.find(nonterminal),1);

			while (!secondMember.empty() && secondMember.find(nonterminal) != std::string::npos)
				secondMember.erase(secondMember.find(nonterminal),1);

			if (firstMember.empty() && secondMember.empty())
				break;
		}

		for (char terminal : m_terminals)
		{
			while (!firstMember.empty() && firstMember.find(terminal) != std::string::npos)
				firstMember.erase(firstMember.find(terminal),1);

			while (!secondMember.empty() && secondMember.find(terminal) != std::string::npos)
				secondMember.erase(secondMember.find(terminal),1);

			if (firstMember.empty() && secondMember.empty())
				break;
		}

		if (!firstMember.empty() || !secondMember.empty() && secondMember != &lambda)
			return false;
	}

	return true;
}


bool Grammar::IsRegular()
{
	for (const auto& product : m_productions)
	{
		const std::string& firstMember = product.first;
		const std::string& secondMember = product.second;

		bool isFirstTerminal = m_terminals.find(secondMember[0]) != m_terminals.end();
		bool isFirstNonTerminal = m_nonterminals.find(secondMember[0]) != m_nonterminals.end();
		bool isSecondNonTerminal = m_nonterminals.find(secondMember[1]) != m_nonterminals.end();

		if (secondMember.size() == 1 && isFirstNonTerminal)
		{
			return false;
		}

		if (secondMember.size() > 2 || (secondMember.size() == 2 && (!isFirstTerminal || !isSecondNonTerminal)))
		{
			return false;
		}

		if (firstMember.size() != 1 || m_nonterminals.find(firstMember[0]) == m_nonterminals.end())
		{
			return false;
		}
	}

	for (const auto& product : m_productions)
	{
		if (product.second == &lambda)
		{
			const char& nonterminal = product.first[0];
			for (const auto& otherProduct : m_productions)
			{
				if (otherProduct.second.find(m_startSymbol) != std::string::npos)
				{
					return false;
				}
			}
		}
	}

	return true;
}

char Grammar::GetStart()
{
	return m_startSymbol;
}

std::set<char> Grammar::GetTerminals()
{
	return m_terminals;
}

std::set<Production> Grammar::GetProductions()
{
	return m_productions;
}

bool Grammar::VerifyGrammer()
{
	if (!FirstVerificationLayer() || !SecondVerificationLayer() ||
		!ThirdVerificationLayer() || !FourthVerificationLayer() ||
		!FifthVerificationLayer())
		return false;	
	return true;
}

std::vector<std::string> Grammar::GenerateWord()
{
	while (true)
	{
		bool validWord = true;
		std::vector<std::string> wordTransformations;
		std::string word = &m_startSymbol;

		wordTransformations.push_back(word);

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

			if (possibleProductions.empty())
			{
				for (auto nonterminal : m_nonterminals)
				{
					if (word.find(nonterminal, 0) != std::string::npos)
					{
						validWord = false;
					}
				}
				if (!validWord) wordTransformations.clear();
				break;
			}

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

			if (selectedProduction.second == &lambda)
				selectedProduction.second = std::string("");

			word.replace(selectedPosition, selectedProduction.first.size(), selectedProduction.second);

			wordTransformations.push_back(word);


			possibleProductions.clear();
		}

		if (validWord)
		{
			return wordTransformations;
		}

	}
}
