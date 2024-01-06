#include "DFA.h"
#include <QDebug>

void DFA::RemoveState(const QString& state)
{
	Automaton::RemoveState(state);
	for (auto it = m_transitions.begin(); it != m_transitions.end();)
	{
		const auto& [input, outputState] = *it;
		const auto& [inputState, symbol] = input;

		if (inputState == state || outputState == state)
		{
			it = m_transitions.erase(it);
		}
		else ++it;
	}
}

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

