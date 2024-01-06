#include "DFA.h"
#include <QDebug>

DFA::DFA(const Automaton& automaton)
	: Automaton(automaton)
{
	// Empty
}

void DFA::AddTransition(QString inputState, char symbol, QString outputState)
{
	m_transitions.insert({ {inputState, symbol}, outputState });
}

bool DFA::CheckWord(const QString& word) const
{
	QString currentState = m_startState;

	for (const auto& symbol : word)
	{
		auto state = m_transitions.find({ currentState, symbol.toLatin1()});
		if (state != m_transitions.end())
		{
			currentState = state->second;
		}
		else return false;
	}

	for (const auto& finalState : m_finalStates)
	{
		if (currentState == finalState)
			return true;
	}

	return false;
}

