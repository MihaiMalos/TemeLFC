#include "NFA.h"

uint16_t NFA::stateCounter = 0;

NFA::NFA(
	const std::vector<uint16_t>& states, 
	const std::vector<char>& alphabet, 
	const TransitionsNFA& transition, 
	uint16_t startState, 
	const std::vector<uint16_t>& finalStates
		 ) 
	:Automaton(states, alphabet, startState, finalStates),
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
		std::unordered_set<char> alphabetReunion{m_alphabet.begin(), m_alphabet.end()};
		alphabetReunion.insert(other.m_alphabet.begin(), other.m_alphabet.end());
		newAlphabet.insert(newAlphabet.end(), alphabetReunion.begin(), alphabetReunion.end());
	}

	//Transitions
	TransitionsNFA newTransitions{m_transitions};
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

NFA NFA::op_concat(NFA& other) const
{
	//States
	//Two different AFNs always have different states
	std::vector<uint16_t> newStates(m_states);
	newStates.insert(newStates.end(), other.m_states.begin(), other.m_states.end());
	//Remove other's start state
	newStates.erase(std::ranges::find(newStates, other.m_startState));

	//Alphabet
	std::vector<char> newAlphabet;
	{
		std::unordered_set<char> alphabetReunion{ m_alphabet.begin(), m_alphabet.end() };
		alphabetReunion.insert(other.m_alphabet.begin(), other.m_alphabet.end());
		newAlphabet.insert(newAlphabet.end(), alphabetReunion.begin(), alphabetReunion.end());
	}

	//Transitions
	TransitionsNFA newTransitions{m_transitions};

	//Replace other's start state with this's final state
	uint16_t otherStartState = other.m_startState;
	uint16_t thisFinalState = this->m_finalStates[0];

	for (const auto& [otherInput, otherOutput] : other.m_transitions)
	{
		uint16_t othNewState = 
			otherInput.first == otherStartState ? 
			thisFinalState : otherInput.first;

		uint16_t othNewSymbol = otherInput.second;
		for (const auto& othOutputState : otherOutput)
			newTransitions[{othNewState, othNewSymbol}].
			push_back(
				othOutputState == otherStartState ? 
				thisFinalState : othOutputState
					 );
	}

	return { newStates, newAlphabet, newTransitions, m_startState, other.m_finalStates };
}

NFA NFA::op_kleene()
{
	//Generate newStart and newFinal
	uint16_t newStart = genState();
	uint16_t newFinal = genState();
	m_states.push_back(newStart);
	m_states.push_back(newFinal);

	//Add new transitions
	uint16_t currentFinal = m_finalStates[0];
	auto& newTransitions = m_transitions;

	const auto& vectorFinal = newTransitions[{currentFinal, m_lambda}];
	if (std::ranges::find(vectorFinal, m_startState) == vectorFinal.end())
		newTransitions[{currentFinal, m_lambda}].push_back(m_startState);

	newTransitions[{currentFinal, m_lambda}].push_back(newFinal);
	newTransitions[{newStart, m_lambda}].push_back(m_startState);
	newTransitions[{newStart, m_lambda}].push_back(newFinal);

	m_startState = newStart;
	m_finalStates = { newFinal };

	return *this;
}

NFA NFA::FromChar(char c)
{
	uint16_t startState = genState();
	uint16_t finalState = genState();
	return { {startState, finalState}, {c}, { {{startState, c}, {finalState}} }, startState, { finalState } };
}