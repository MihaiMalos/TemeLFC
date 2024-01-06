#include "NFA.h"

#include <algorithm>

NFA::NFA(const Automaton& automaton)
	: Automaton(automaton)
{
	// Empty
}

void NFA::AddTransition(QString& inputState, char symbol, QString& outputState)
{
	m_transitions[{inputState, symbol}].push_back(outputState);
}

CheckWordOutput NFA::CheckWord(const QString& word) const
{
	std::vector<std::vector<QString>> checkWordOutput;
	std::set<QString> currentStateSet;
	currentStateSet.insert(m_startState);

	for (const auto& symbol : word)
	{
		std::set<QString> nextStateSet;

		for (const auto& state : currentStateSet)
		{
			auto stateRange = m_transitions.find({ state, symbol.toLatin1() });
			if (stateRange != m_transitions.end())
			{
				for (auto outputState : stateRange->second)
				{
					nextStateSet.insert(outputState);
				}
			}
		}
		currentStateSet = nextStateSet;

		std::vector<QString> currentStateVector;
		currentStateVector.insert(currentStateVector.end(), currentStateSet.begin(), currentStateSet.end());
		checkWordOutput.push_back(currentStateVector);
	}

	for (const auto& finalState : m_finalStates)
	{
		if (currentStateSet.find(finalState) != currentStateSet.end())
			return { checkWordOutput, true };
	}
	return {checkWordOutput, false };
}
