#pragma once

#include "FiniteAutomaton.h"
#include <tuple>
#include <map>
#include <vector>

using NFAInput = std::pair<char, char>;
using NFAOutput = char;
using NFATransitions = std::map<NFAInput, std::vector<NFAOutput>>;

class NFA : public FiniteAutomaton
{
public:
	NFA(QPointF& screenPos);

	// Class own methods
	void AddTransition(char& inputState, char& symbol, char& outputState);
	void RemoveTransition(char& inputState, char symbol, char& outputState);

	// IFiniteAutomaton inherited methods
	bool CheckWord(const QString& word) const override;
	bool SaveAutomaton(const QString& fileName) const override;
	bool LoadAutomaton(const QString& fileName) override;

private: 
	NFATransitions m_transitions;

};

