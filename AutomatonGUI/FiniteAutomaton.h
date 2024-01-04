#pragma once

#include <QString>
#include <QPoint>
#include <set>

class FiniteAutomaton
{
public:
	FiniteAutomaton(QPointF& screenPos);

	QPointF GetPosition() const;
	void SetPosition(QPointF& position);

	void AddState(const char& state);
	void MakeStateFinal(const char& state);

	void RemoveState(const char& state);
	void RemoveFinalState(const char& state);

	virtual bool CheckWord(const QString& word) const = 0;
	virtual bool SaveAutomaton(const QString& fileName) const = 0;
	virtual bool LoadAutomaton(const QString& fileName) = 0;
	virtual ~FiniteAutomaton() = default;

protected:
	const char m_lambda = '-';
	std::set<char> m_alphabet;
	std::set<char> m_states;
	std::set<char> m_finalStates;
	QPointF m_screenPosition;


};

