#include "DFA.h"

DFA::DFA(
	const std::vector<uint16_t>& states, 
	const std::vector<char>& alphabet,
	const TransitionsDFA& transitions,
	int startState, 
	const std::vector<uint16_t>& finalStates
		) :
	Automaton(states, alphabet, startState, finalStates),
	m_transitions{ transitions }
{
}

bool DFA::CheckWord(std::string word)
{
	int currentState = m_startState;

	for (const char& symbol : word)
	{
		auto state = m_transitions.find({ currentState, symbol });
		if (state != m_transitions.end())
		{
			currentState = state->second;
		}
		else return false;
	}

	if (std::ranges::find(m_finalStates, currentState) == m_finalStates.end())
		return false;

	return true;
}

bool DFA::checkAutomatonValid() const
{
	return checkInputStateInStates() && checkFinalStatesInStates() && checkTransitionsValid();
}

bool DFA::checkInputStateInStates() const
{
	if (std::ranges::find(m_states, m_startState) != m_states.end()) 
		return true;
	return false;
}

bool DFA::checkFinalStatesInStates() const
{
	for (const auto& element : m_finalStates)
	{
		if (std::ranges::find(m_states, element) == m_states.end()) 
			return false;
	}
	return true;
}

bool DFA::checkTransitionsValid() const
{
	for (const auto& transition : m_transitions)
	{
		const auto& [inputState, inputSymbol] = transition.first;
		const auto& outputState = transition.second;

		if (std::ranges::find(m_states, inputState) == m_states.end()) 
			return false;

		if (std::ranges::find(m_states, outputState) == m_states.end()) 
			return false;

		if (std::ranges::find(m_alphabet, inputSymbol) == m_alphabet.end()) 
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