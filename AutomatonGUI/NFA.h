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

	// Class own methods
	void AddTransition(QString& inputState, char& symbol, QString& outputState);
	void RemoveTransition(QString& inputState, char symbol, QString& outputState);

	bool CheckWord(const QString& word) const;

private:
	NFATransitions m_transitions;

};

