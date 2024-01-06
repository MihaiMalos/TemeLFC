#include "DFA.h"

bool DFA::VerifyWord(std::string word)
{
	if (word.find(&m_lambda) != std::string::npos && word.size() > 1) return false;

	char currentState = m_startState;

	for (const char& symbol : word)
	{
		auto state = m_transitions.find({ currentState, symbol });
		if (state != m_transitions.end())
		{
			currentState = state->second;
		}
		else return false;
	}

	for (const auto& finalState : m_finalStates)
	{
		if (currentState == finalState)
			return true;
	}

	return false;
}



std::ostream& operator<<(std::ostream& out, const DFA& automaton)
{
	out << "M = ({";
	for (auto& it : automaton.m_states)
	{
		out << it;
		if (it != *automaton.m_states.rbegin())
			out << ",";
	}
	out << "},";

	out << "{";
	for (auto it : automaton.m_alphabet)
	{
		out << it;
		if (it != *automaton.m_alphabet.rbegin())
			out << ",";
	}
	out << "},";

	out << "delta," << automaton.m_startState << ",";

	out << "{";
	for (auto it : automaton.m_finalStates)
	{
		out << it;
		if (it != *automaton.m_finalStates.rbegin())
			out << ",";
	}
	out << "}), delta continand urmatoarele tranzitii:\n";

	for (const auto& transition : automaton.m_transitions)
	{
		out << "delta(" << transition.first.first << ", " << transition.first.second << ")";
		out << " = " << transition.second << std::endl;
	}

	return out;
}