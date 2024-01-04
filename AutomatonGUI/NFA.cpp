#include "NFA.h"

#include <ranges>
#include <algorithm>

NFA::NFA(QPointF& screenPos)
	: FiniteAutomaton(screenPos)
{
	// Empty
}

void NFA::AddTransition(char& inputState, char& symbol, char& outputState)
{
	m_transitions[{inputState, symbol}].push_back(outputState);
}

void NFA::RemoveTransition(char& inputState, char symbol, char& outputState)
{
	if (auto input = m_transitions.find({ inputState, symbol }); input != m_transitions.end())
	{
		auto& output = input->second;
		std::ranges::remove_if(output, [&outputState](char state) { return state == outputState; });
	}
}

bool NFA::CheckWord(const QString& word) const
{
	return false;
}

bool NFA::SaveAutomaton(const QString& fileName) const
{
	return false;
}

bool NFA::LoadAutomaton(const QString& fileName)
{
	return false;
}
