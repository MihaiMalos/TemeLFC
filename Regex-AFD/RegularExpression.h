#pragma once
#include <iostream>
#include <regex>
#include <stack>
#include <string>

#include "DeterministicFiniteAutomaton.h"


class RegularExpression
{
public:
	RegularExpression() = default;
	~RegularExpression() = default;

	bool IsValid();
	DeterministicFiniteAutomaton ConvertToAutomaton();
	const uint16_t& GetRank(const char& op) const;
	
	std::string AddConcatenation(const std::string& expression) const;

private:
	std::string m_expression;

private:
	std::string BuildPolishForm();
	static const char k_concatenation;

//public:
//	friend std::istream& operator>>(std::istream& in, RegularExpression& expresion);
//	friend std::ostream& operator<<(std::ostream& out, const RegularExpression& expresion);
};

