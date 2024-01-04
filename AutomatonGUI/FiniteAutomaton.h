#pragma once

#include <QString>
#include <QPoint>
#include <set>
#include <map>

using StatesPositionsList = std::map<QString, QPointF>;

class FiniteAutomaton
{
public:
	FiniteAutomaton() = default;

	StatesPositionsList GetStatesPositions() const;
	bool IsFinalState(const QString& state) const;

	void AddState(const QString& state, const QPointF& position);
	void MakeStateFinal(const QString& state);
	void SetStatePosition(const QString& state, const QPointF& position);

	void RemoveState(const QString& state);
	void RemoveFinalState(const QString& state);

	virtual bool CheckWord(const QString& word) const = 0;
	virtual bool SaveAutomaton(const QString& fileName) const = 0;
	virtual bool LoadAutomaton(const QString& fileName) = 0;
	virtual ~FiniteAutomaton() = default;

protected:
	const char m_lambda = '-';
	std::set<char> m_alphabet;
	std::set<QString> m_states;
	std::set<QString> m_finalStates;
	StatesPositionsList m_statesPositions;


};

