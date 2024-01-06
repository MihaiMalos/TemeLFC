#pragma once

#include <QString>
#include <QPoint>
#include <set>
#include <map>

#include "TransitionsModel.h"

using StatesPositionsList = std::map<QString, QPointF>;

class Automaton
{
public:
	Automaton() = default;
	virtual ~Automaton() = default;

	StatesPositionsList GetStatesPositions() const;
	bool IsFinalState(const QString& state) const;
	bool IsFirstState(const QString& state) const;
	std::vector<std::vector<QString>> GetTransitions();

	void AddState(const QString& state, const QPointF& position);
	void MakeStateFinal(const QString& state);
	void MakeStateStart(const QString& state);
	void SetStatePosition(const QString& state, const QPointF& position);
	void AddModelTransition(const QString& transition);
	AutomatonType GetAutomatonType();

	virtual void RemoveState(const QString& state);
	void RemoveFinalState(const QString& state);

	bool SaveAutomaton(const QString& fileName) const;
	bool LoadAutomaton(const QString& fileName);

protected:
	const char m_lambda = '-';
	std::set<char> m_alphabet;
	std::set<QString> m_states;
	std::set<QString> m_finalStates;
	QString m_startState;
	StatesPositionsList m_statesPositions;
	TransitionsModel m_transitionsModel;


};

