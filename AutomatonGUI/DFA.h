#pragma once

#include "Automaton.h"
#include <tuple>
#include <map>
#include <vector>

using DFAInput = std::pair<QString, char>;
using DFATransitions = std::map<DFAInput, QString>;

class DFA : public Automaton
{
public:
	DFA() = default;
	DFA(const Automaton& automaton);

	void AddTransition(QString inputState, char symbol, QString outputState);
	CheckWordOutput CheckWord(const QString& word) const;


private:
	DFATransitions m_transitions;
};

