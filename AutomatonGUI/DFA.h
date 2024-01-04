#pragma once

#include "FiniteAutomaton.h"
#include <tuple>
#include <map>
#include <vector>

using DFAInput = std::pair<char, char>;
using DFAOutput = char;
using DFATransitions = std::map<DFAInput, DFAOutput>;

class DFA : public FiniteAutomaton
{
public:
	DFA(QPointF& screenPos);

	// Class own methods
	void AddTransition(char& inputState, char& symbol, char& outputState);
	void RemoveTransition(char& inputState, char symbol);

	// IFiniteAutomaton inherited methods
	bool CheckWord(const QString& word) const override;
	bool SaveAutomaton(const QString& fileName) const override;
	bool LoadAutomaton(const QString& fileName) override;

private:
	DFATransitions m_transitions;
};

