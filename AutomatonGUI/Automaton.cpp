#include "Automaton.h"
#include <QFile>
#include <QTextStream>

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

bool Automaton::IsStartState(const QString& state) const
{
	return m_startState == state;
}

std::vector<std::vector<QString>> Automaton::GetTransitions() const
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

void Automaton::Clear()
{
	m_transitionsModel.Clear();
	m_states.clear();
	m_statesPositions.clear();
	m_finalStates.clear();
	m_startState = "";
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

void Automaton::SaveAutomaton(const QString& filePath) const
{
	QFile file(filePath);
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&file);
	out << "States:\n";
	for (auto state : m_statesPositions)
	{
		const auto& [stateString, pos] = state;
		out << stateString << " " << pos.x() << " " << pos.y() << " ";
		if (IsStartState(stateString)) out << "S ";
		if (IsFinalState(stateString)) out << "F ";
		out << "\n";
	}
	
	out << "Transitions:\n";

	for (const auto& transition : GetTransitions())
	{
		for (auto element : transition)
		{
			out << element << " ";
		}
		out << "\n";
	}

}

void Automaton::LoadAutomaton(const QString& filePath)
{
	Clear();
	QFile file(filePath);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream in(&file);
	QString line;
	line = in.readLine();
	while ((line = in.readLine()) != "Transitions:")
	{
		QStringList words = line.split(" ", Qt::SkipEmptyParts);
		AddState(words[0], QPointF(words[1].toFloat(), words[2].toFloat()));
		if (words.size() == 4) words[3] == "F" ? MakeStateFinal(words[0]) : MakeStateStart(words[0]);
		if (words.size() == 5)
		{
			MakeStateStart(words[0]);
			MakeStateFinal(words[0]);
		}
	}

	while (!(line = in.readLine()).isNull())
	{
		AddModelTransition(line);
	}
}
