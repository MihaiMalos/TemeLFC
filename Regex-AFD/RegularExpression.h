#pragma once
#include <iostream>
#include <regex>
#include <stack>
#include <string>

#include "DFA.h"
#include "NFA.h"


class RegularExpression
{
public:
	RegularExpression() = default;
	~RegularExpression() = default;

	bool IsValid();
	NFA ConvertToAutomaton();
	const uint16_t& GetRank(const char& op) const;
	
	void ReduceStars();
	std::string AddConcatenation(const std::string& expression) const;

private:
	std::string m_expression;

private:
	std::string BuildPolishForm();

public:
	friend std::istream& operator>>(std::istream& in, RegularExpression& expresion);
	friend std::ostream& operator<<(std::ostream& out, const RegularExpression& expresion);
};

