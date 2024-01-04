#pragma once

#include "FiniteAutomaton.h"
#include <tuple>
#include <map>
#include <vector>

using DFAInput = std::pair<QString, char>;
using DFAOutput = QString;
using DFATransitions = std::map<DFAInput, DFAOutput>;

class DFA : public FiniteAutomaton
{
public:
	DFA() = default;

	// Class own methods
	void AddTransition(QString& inputState, char& symbol, QString& outputState);
	void RemoveTransition(QString& inputState, char symbol);

	// IFiniteAutomaton inherited methods
	bool CheckWord(const QString& word) const override;
	bool SaveAutomaton(const QString& fileName) const override;
	bool LoadAutomaton(const QString& fileName) override;

private:
	DFATransitions m_transitions;
};

