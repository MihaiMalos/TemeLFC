#include "FiniteAutomaton.h"

StatesPositionsList FiniteAutomaton::GetStatesPositions() const
{
	return m_statesPositions;
}

bool FiniteAutomaton::IsFinalState(const QString& state) const
{
	return m_finalStates.find(state) != m_finalStates.end();
}

void FiniteAutomaton::AddState(const QString& state, const QPointF& position)
{
	m_states.insert(state);
	m_statesPositions.insert({ state, position });
}

void FiniteAutomaton::MakeStateFinal(const QString& state)
{
	m_finalStates.insert(state);
}

void FiniteAutomaton::SetStatePosition(const QString& state, const QPointF& position)
{
	if (m_statesPositions.find(state) != m_statesPositions.end())
	{
		m_statesPositions.at(state) = position;
	}
}

void FiniteAutomaton::RemoveState(const QString& state)
{
	m_states.erase(state);
}

void FiniteAutomaton::RemoveFinalState(const QString& state)
{
	m_finalStates.erase(state);
}
