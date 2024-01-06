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

CheckWordOutput DFA::CheckWord(const QString& word) const
{
	std::vector<std::vector<QString>> checkWordOutput;
	QString currentState = m_startState;
	checkWordOutput.push_back({ currentState });

	for (const auto& symbol : word)
	{
		auto state = m_transitions.find({ currentState, symbol.toLatin1()});
		if (state != m_transitions.end())
		{
			currentState = state->second;
			checkWordOutput.push_back({ currentState });
		}
		else return { checkWordOutput, false };
	}

	for (const auto& finalState : m_finalStates)
	{
		if (currentState == finalState)
			return { checkWordOutput, true };
	}

	return { checkWordOutput, false };
}

