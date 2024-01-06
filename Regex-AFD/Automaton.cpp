#include "Automaton.h"

Automaton::Automaton(const std::vector<uint16_t>& states, const std::vector<char>& alphabet, uint16_t startState, const std::vector<uint16_t>& finalStates)
	:m_states{states}, 
	m_alphabet{alphabet},
	m_startState{startState}, 
	m_finalStates{finalStates}
{
}
