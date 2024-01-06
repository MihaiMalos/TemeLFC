#include "Automaton.h"

Automaton::Automaton()
	: m_startState("")
{
	// Empty
}

StatesPositionsList Automaton::GetStatesPositions() const
{
	return m_statesPositions;
}

bool Automaton::IsFinalState(const QString& state) const
{
	return m_finalStates.find(state) != m_finalStates.end();
}

bool Automaton::IsFirstState(const QString& state) const
{
	return m_startState == state;
}

std::vector<std::vector<QString>> Automaton::GetTransitions()
{
	return m_transitionsModel.GetTransitions();
}

void Automaton::AddState(const QString& state, const QPointF& position)
{
	m_states.insert(state);
	m_statesPositions.insert({ state, position });
}

void Automaton::MakeStateFinal(const QString& state)
{
	m_finalStates.insert(state);
}

void Automaton::MakeStateStart(const QString& state)
{
	m_startState = state;
}

void Automaton::SetStatePosition(const QString& state, const QPointF& position)
{
	if (m_statesPositions.find(state) != m_statesPositions.end())
	{
		m_statesPositions.at(state) = position;
	}
}

void Automaton::AddModelTransition(const QString& transition)
{
	m_transitionsModel.InsertTransition(transition);
}

AutomatonType Automaton::GetAutomatonType()
{
	return m_transitionsModel.GetAutomatonType();
}

void Automaton::RemoveState(const QString& state)
{
	m_states.erase(state);
	m_statesPositions.erase(state);
	m_finalStates.erase(state);
	m_transitionsModel.RemoveTransitionsOfState(state);
	if (m_startState == state) m_startState = "";
}

void Automaton::RemoveFinalState(const QString& state)
{
	m_finalStates.erase(state);
}

bool Automaton::SaveAutomaton(const QString& fileName) const
{
	return false;
}

bool Automaton::LoadAutomaton(const QString& fileName)
{
	return false;
}
