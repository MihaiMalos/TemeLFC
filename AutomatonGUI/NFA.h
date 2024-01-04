#pragma once

#include "FiniteAutomaton.h"
#include <tuple>
#include <map>
#include <vector>

using NFAInput = std::pair<QString, char>;
using NFAOutput = QString;
using NFATransitions = std::map<NFAInput, std::vector<NFAOutput>>;

class NFA : public FiniteAutomaton
{
public:
	NFA() = default;

	// Class own methods
	void AddTransition(QString& inputState, char& symbol, QString& outputState);
	void RemoveTransition(QString& inputState, char symbol, QString& outputState);

	// IFiniteAutomaton inherited methods
	bool CheckWord(const QString& word) const override;
	bool SaveAutomaton(const QString& fileName) const override;
	bool LoadAutomaton(const QString& fileName) override;

private: 
	NFATransitions m_transitions;

};

