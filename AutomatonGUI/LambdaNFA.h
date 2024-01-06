#pragma once
#include "NFA.h"

class LambdaNFA : public NFA
{
public:
	LambdaNFA() = default;
	LambdaNFA(const Automaton& automaton);

	bool CheckWord(const QString& word) const override;
};

