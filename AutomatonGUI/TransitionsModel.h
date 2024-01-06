#pragma once

#include <QString>
#include <regex>
#include <QHash>

enum class AutomatonType
{
	DFA,
	NFA,
	LambdaNFA,
	PDA,
	None,
};

class TransitionsModel
{
public:
	TransitionsModel() = default;
	AutomatonType GetAutomatonType() const;
	void InsertTransition(const QString& transition);
	void RemoveTransitionsOfState(const QString& state);
	std::vector<std::vector<QString>> GetTransitions() const;
	void ModifyTransition(QString transition, int index);

private:
	std::vector<QString> m_transitions;
};

