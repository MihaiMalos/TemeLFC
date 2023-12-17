#pragma once

#include <string>
#include "DeterministicFiniteAutomaton.h"


class RegularExpression
{
public:
	RegularExpression() = default;
	~RegularExpression() = default;

	bool IsValid();
	DeterministicFiniteAutomaton ConvertToAutomaton();


private:
	std::string m_expression;
};

