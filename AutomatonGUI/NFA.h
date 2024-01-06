#pragma once

#include "Automaton.h"
#include <tuple>
#include <map>
#include <vector>

using NFAInput = std::pair<QString, char>;
using NFATransitions = std::map<NFAInput, std::vector<QString>>;

class NFA : public Automaton
{
public:
	NFA() = default;
	NFA(const Automaton& automaton);

	// Class own methods
	void AddTransition(QString& inputState, char symbol, QString& outputState);
	virtual bool CheckWord(const QString& word) const;

protected:
	NFATransitions m_transitions;

};

