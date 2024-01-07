#include "LambdaNFA.h"

LambdaNFA::LambdaNFA(const Automaton& automaton) : NFA(automaton)
{
	// Empty
}

CheckWordOutput LambdaNFA::CheckWord(const QString& word) const
{
	std::vector<std::vector<QString>> checkWordOutput;
	std::set<QString> currentStateSet;
	currentStateSet.insert(m_startState);
	LamdaExtend(currentStateSet);

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
				}
			}
			LamdaExtend(nextStateSet);
		}

		std::vector<QString> currentStateVector;
		currentStateVector.insert(currentStateVector.end(), currentStateSet.begin(), currentStateSet.end());
		checkWordOutput.push_back(currentStateVector);

		currentStateSet = nextStateSet;
	}

	for (const auto& finalState : m_finalStates)
	{
		if (currentStateSet.find(finalState) != currentStateSet.end())
			return {checkWordOutput, true};
	}
	return { checkWordOutput, false };
}

void LambdaNFA::LamdaExtend(std::set<QString>& states) const
{
	while (true)
	{
		std::set<QString> newStates = states;
		for (auto state : states)
		{
			auto lambdaStateRange = m_transitions.find({ state, m_lambda });
			if (lambdaStateRange != m_transitions.end())
			{
				for (auto& lambdaState : lambdaStateRange->second)
				{
					newStates.insert(lambdaState);
				}
			}
		}
		if (newStates == states) return;

		states = newStates;
	}
}
