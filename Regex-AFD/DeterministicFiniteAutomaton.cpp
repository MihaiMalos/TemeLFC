#include "DeterministicFiniteAutomaton.h"

bool DeterministicFiniteAutomaton::VerifyWord(std::string word)
{
	/*
	
	DON'T FORGET TO TREAT LAMBDA RIGHT!
	
	*/

	if (word.find(&lambda) != std::string::npos && word.size() > 1) return false;

	std::set<char> currentStateSet;
	currentStateSet.insert(m_firstState);

	for (const char& symbol : word)
	{
		std::set<char> nextStateSet;

		for (const char& state : currentStateSet)
		{
			if (m_transitions.find({ state,symbol }) != m_transitions.end())
			{
				auto range = m_transitions.equal_range({ state,symbol });

				//find all transitions possible
				for (auto it = range.first; it != range.second; ++it)
				{
					nextStateSet.insert(it->second);
				}
			}
		}
		currentStateSet = nextStateSet;
	}

	// Something here i think
	for (const auto& finalState : m_finalStates)
	{
		if (currentStateSet.find(finalState) != currentStateSet.end())
			return true;
	}

	return false;
}


bool DeterministicFiniteAutomaton::VerifyAutomaton()
{
	if (VerifyCharacters() == true && VerifyFinalStates() == true && VerifyStartState() == true && VerifyTransitionFunctions() == true)
		return true;
	else return false;

}

bool DeterministicFiniteAutomaton::VerifyCharacters()
{
	for (const char& it : m_alphabet)
		if (m_states.find(it) != m_states.end())
		{
			return false;
		}
	return true;
}
bool DeterministicFiniteAutomaton::VerifyStartState()
{
	if (m_states.find(m_firstState) == m_states.end())
		return false;

	return true;
}
bool DeterministicFiniteAutomaton::VerifyFinalStates()
{
	for (const char& it : m_finalStates)
	{
		if (m_states.find(it) == m_states.end())
			return false;
	}


	return true;
}
bool DeterministicFiniteAutomaton::VerifyTransitionFunctions()
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

std::ostream& operator<<(std::ostream& out, const DeterministicFiniteAutomaton& automaton)
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