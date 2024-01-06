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
	Automaton();
	virtual ~Automaton() = default;

	StatesPositionsList GetStatesPositions() const;
	bool IsFinalState(const QString& state) const;
	bool IsStartState(const QString& state) const;
	std::vector<std::vector<QString>> GetTransitions() const;

	void AddState(const QString& state, const QPointF& position);
	void MakeStateFinal(const QString& state);
	void MakeStateStart(const QString& state);
	void SetStatePosition(const QString& state, const QPointF& position);
	void AddModelTransition(const QString& transition);
	AutomatonType GetAutomatonType();
	void Clear();

	virtual void RemoveState(const QString& state);
	void RemoveFinalState(const QString& state);

	void SaveAutomaton(const QString& fileName) const;
	void LoadAutomaton(const QString& filePath);

protected:
	const char m_lambda = '-';
	std::set<QString> m_states;
	std::set<QString> m_finalStates;
	QString m_startState;
	StatesPositionsList m_statesPositions;
	TransitionsModel m_transitionsModel;


};

