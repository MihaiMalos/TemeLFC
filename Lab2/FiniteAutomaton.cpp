#include "FiniteAutomaton.h"

FiniteAutomaton::FiniteAutomaton()
{
}
FiniteAutomaton::FiniteAutomaton(std::set<std::string> states, std::string firstState,
	std::set<char> alphabet, std::set<std::string> finalStates, Transitions transitions)
	:m_states{states} , m_firstState{firstState} , m_alphabet{alphabet} 
	, m_finalStates{finalStates} , m_transitions {transitions}
{
}
FiniteAutomaton::~FiniteAutomaton()
{
}

bool FiniteAutomaton::VerifyAutomaton()
{
	if (VerifyCharacters() == true && VerifyFinalStates() == true && VerifyStartState() == true && VerifyTransitionFunctions() == true)
		return true;
	else return false;

}

std::ostream& operator<<(std::ostream& out,const FiniteAutomaton& automaton)
{
	//TODO
	return out;
}

bool FiniteAutomaton::VerifyCharacters()
{
	for (const char& it : m_alphabet)
	{
		if (std::find(m_states.begin(), m_states.end(), it) != m_states.end())
		{
			return false;
		}
	}
	return true;
}

bool FiniteAutomaton::VerifyStartState()
{

	if (std::find(m_states.begin(), m_states.end(), m_firstState) == m_states.end())
	{
		return false;
	}
	return true;
}

bool FiniteAutomaton::VerifyFinalStates()
{
	for (const auto& it : m_finalStates)
	{
		if (std::find(m_states.begin(), m_states.end(), it) == m_states.end())
		{
			return false;
		}
	}
	return true;
}

bool FiniteAutomaton::VerifyTransitionFunctions()
{
	for (const auto& it : m_transitions)
	{
		if (std::find(m_states.begin(), m_states.end(), it.first.first) == m_states.end())
			return false;
		if (std::find(m_states.begin(), m_states.end(), it.second) == m_states.end())
			return false;
		if (std::find(m_alphabet.begin(), m_alphabet.end(), it.first.second) == m_alphabet.end())
			return false;
	}
	return true;
}