#pragma once

#include <set>
#include <string>
#include <tuple>
#include<regex>

using Production = std::pair<std::string, std::string>;

class Grammar
{
public:
	bool VerifyGrammer();
	void GenerateWord();

public:
	friend std::istream& operator>>(std::istream& in, Grammar& grammar);
	friend std::ostream& operator<<(std::ostream& out, const Grammar& grammar);

private:
	bool FirstVerificationLayer();
	bool SecondVerificationLayer();
	bool ThirdVerificationLayer();
	bool FourthVerificationLayer();
	bool FifthVerificationLayer();

private:
	std::set<char> m_terminals, m_nonterminals;
	char m_startSymbol;
	std::set<Production> m_productions;
};

