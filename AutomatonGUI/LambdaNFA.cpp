#include "LambdaNFA.h"

LambdaNFA::LambdaNFA(const Automaton& automaton) : NFA(automaton)
{
	// Empty
}

bool LambdaNFA::CheckWord(const QString& word) const
{
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
				for (auto& outputState : stateRange->second)
				{
					nextStateSet.insert(outputState);

					auto lambdaStateRange = m_transitions.find({ outputState, m_lambda });
					if (lambdaStateRange != m_transitions.end())
					{
						for (auto& lambdaState : lambdaStateRange->second)
						{
							nextStateSet.insert(lambdaState);
						}
					}
				}
			}
		}
		currentStateSet = nextStateSet;
	}

	for (const auto& finalState : m_finalStates)
	{
		if (currentStateSet.find(finalState) != currentStateSet.end())
			return true;
	}
	return false;
}
