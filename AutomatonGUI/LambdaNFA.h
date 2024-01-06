#pragma once
#include "NFA.h"

class LambdaNFA : public NFA
{
public:
	LambdaNFA() = default;
	LambdaNFA(const Automaton& automaton);

	CheckWordOutput CheckWord(const QString& word) const override;
};

