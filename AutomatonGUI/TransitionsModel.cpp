#include "TransitionsModel.h"

AutomatonType TransitionsModel::GetAutomatonType() const
{
	auto transitions = GetTransitions();
	std::unordered_map<QString, QString> transitionsMap;

	bool PDA = false;
	bool NFA = false;
	bool lambdaNFA = false;
	bool finiteAutomaton = false;
	for (auto transition : transitions)
	{
		if (transition.size() == 5) PDA = true;
		if (transition.size() == 3) finiteAutomaton = true;
		if (transition[1] == "-") lambdaNFA = true;
		const auto& key = transition[0] + " " + transition[1];
		if (transitionsMap.find(key) != transitionsMap.end()) NFA = true;
		transitionsMap.emplace(key, transition[2]);
	}
	if (PDA) return finiteAutomaton ? AutomatonType::None : AutomatonType::PDA;
	if (lambdaNFA) return AutomatonType::LambdaNFA;
	if (NFA) return AutomatonType::NFA;
	return AutomatonType::DFA;
}

void TransitionsModel::InsertTransition(const QString& transition)
{
	m_transitions.push_back(transition);
}

void TransitionsModel::RemoveTransitionsOfState(const QString& state)
{
	auto transitions = GetTransitions();
	int index = 0;
	for (auto it = transitions.begin(); it != transitions.end(); it++)
	{
		const auto& inputState = (*it)[0];
		const auto& outputState = (*it)[it->size() - 1];

		if (inputState == state || outputState == state)
		{
			m_transitions.erase(m_transitions.begin() + index);
		}
		else 
		{
			++index;
		}
	}
}

std::vector<std::vector<QString>> TransitionsModel::GetTransitions() const
{
	std::regex wordRegex("\\S+");
	std::vector<std::vector<QString>> transitions;
	for (auto transition : m_transitions)
	{
		std::string word = transition.toStdString();
		std::sregex_iterator it(word.begin(), word.end(), wordRegex);
		std::sregex_iterator end;
		std::vector<QString> words;

		while (it != end)
		{
			words.push_back(QString::fromStdString(it->str()));
			++it;
		}

		transitions.push_back(words);
	}

	return transitions;
}

void TransitionsModel::ModifyTransition(QString transition, int index)
{
	m_transitions[index] = transition;
}

