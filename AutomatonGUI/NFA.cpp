#include "NFA.h"

#include <algorithm>

void NFA::AddTransition(QString& inputState, char& symbol, QString& outputState)
{
	m_transitions[{inputState, symbol}].push_back(outputState);
}

void NFA::RemoveTransition(QString& inputState, char symbol, QString& outputState)
{
	if (auto input = m_transitions.find({ inputState, symbol }); input != m_transitions.end())
	{
		auto& output = input->second;
		auto it = std::erase_if(output, [&outputState](QString& state) { return state == outputState; });
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
