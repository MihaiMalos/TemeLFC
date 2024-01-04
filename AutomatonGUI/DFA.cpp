#include "DFA.h"

DFA::DFA(QPointF& screenPos)
	: FiniteAutomaton(screenPos)
{
	// Empty
}

void DFA::AddTransition(char& inputState, char& symbol, char& outputState)
{
	m_transitions[{inputState, symbol}] = outputState;
}

void DFA::RemoveTransition(char& inputState, char symbol)
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
