#include "NFA.h"

NFA::NFA(const std::vector<uint16_t>& states, const std::vector<char>& alphabet, const TransitionsNFA& transition, uint16_t startState, const std::vector<uint16_t>& finalStates) :
	Automaton(states, alphabet, startState, finalStates),
	m_transitions{transition}
{
}

NFA NFA::op_or(const NFA& other) const
{
	//States
	std::vector<uint16_t> newStates(m_states);
	newStates.insert(newStates.end(), other.m_states.begin(), other.m_states.end());

	//Alphabet
	std::vector<char> newAlphabet;
	{
		std::unordered_set<char> alphaReunion(m_alphabet.begin(), m_alphabet.end());
		alphaReunion.insert(other.m_alphabet.begin(), other.m_alphabet.end());
		newAlphabet.insert(newAlphabet.end(), alphaReunion.begin(), alphaReunion.end());
	}

	//Transitions
	TransitionsNFA newTransitions(m_transitions);
	for (const auto& trans : other.m_transitions)
		newTransitions[trans.first] = trans.second;

	//Add two new states for start and end
	uint16_t newStartState = genState();
	uint16_t newEndState = genState();
	newStates.push_back(newStartState);
	newStates.push_back(newEndState);

	//Add new transitions
	newTransitions[{ newStartState, m_lambda}].push_back(m_startState);
	newTransitions[{ newStartState, m_lambda}].push_back(other.m_startState);
	newTransitions[{ m_finalStates[0], m_lambda}].push_back(newEndState);
	newTransitions[{ other.m_finalStates[0], m_lambda}].push_back(newEndState);

	return { newStates, newAlphabet, newTransitions, newStartState, {newEndState} };
}

NFA NFA::op_concat(NFA other) const
{
	return NFA();
}

NFA NFA::op_kleene() const
{
	return NFA();
}
