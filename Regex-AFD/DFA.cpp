#include "DFA.h"

bool DFA::VerifyWord(std::string word)
{
	if (word.find(&lambda) != std::string::npos && word.size() > 1) return false;

	char currentState = m_firstState;

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


bool DFA::VerifyAutomaton()
{
	if (VerifyCharacters() == true && VerifyFinalStates() == true && VerifyStartState() == true && VerifyTransitionFunctions() == true)
		return true;
	else return false;

}

bool DFA::VerifyCharacters()
{
	for (const char& it : m_alphabet)
		if (m_states.find(it) != m_states.end())
		{
			return false;
		}
	return true;
}
bool DFA::VerifyStartState()
{
	if (m_states.find(m_firstState) == m_states.end())
		return false;

	return true;
}
bool DFA::VerifyFinalStates()
{
	for (const char& it : m_finalStates)
	{
		if (m_states.find(it) == m_states.end())
			return false;
	}


	return true;
}
bool DFA::VerifyTransitionFunctions()
{
	for (const auto& it : m_transitions)
	{
		if (std::find(m_states.begin(), m_states.end(), it.first.first) == m_states.end())
			return false;
		if (std::find(m_alphabet.begin(), m_alphabet.end(), it.first.second) == m_alphabet.end())
			return false;
		if (std::find(m_states.begin(), m_states.end(), it.second) == m_states.end())
			return false;

	}
	return true;
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

	out << "delta," << automaton.m_firstState << ",";

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