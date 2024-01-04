#include "DFA.h"

void DFA::AddTransition(QString& inputState, char& symbol, QString& outputState)
{
	m_transitions[{inputState, symbol}] = outputState;
}

void DFA::RemoveTransition(QString& inputState, char symbol)
{
	m_transitions.erase({ inputState, symbol });
}

bool DFA::CheckWord(const QString& word) const
{
	return false;
}

bool DFA::SaveAutomaton(const QString& fileName) const
{
	return false;
}

bool DFA::LoadAutomaton(const QString& fileName)
{
	return false;
}
