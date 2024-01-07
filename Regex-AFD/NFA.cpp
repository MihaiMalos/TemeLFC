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

NFA NFA::FromChar(char character)
{
	uint16_t startState = genState();
	uint16_t finalState = genState();
	return { {startState, finalState}, {character}, { {{startState, character}, {finalState}} }, startState, { finalState } };
}

DFA NFA::TransformInDFA() const
{
	std::vector<std::unordered_set<uint16_t>> newStates;
	TransitionsDFA  newTransitions;
	std::queue<uint16_t> queue;

	//Add new start element to queue and newStates (always is node with index 1) 
	{
		std::unordered_set<uint16_t> newStartState{ m_startState };
		lambdaExtend(newStartState);
		newStates.push_back(newStartState);
		queue.push(0);
	}

	//While there are elements in queue
	while (!queue.empty())
	{
		//Get current element
		uint16_t newIndex = queue.front();
		const std::unordered_set<uint16_t> currentState = newStates[newIndex];
		queue.pop();

		//See where it goes with every symbol
		for (char symbol : m_alphabet)
		{
			//Create new state, add all AFN states in which we end up from currentState
			std::unordered_set<uint16_t> newState;
			for (const auto& oldState : currentState) {
				try {
					for (const auto& element : m_transitions.at({ oldState, symbol }))
						newState.insert(element);
				}
				catch (...) {}
			}

			//Ignore newState if it's empty
			if (newState.empty())
				continue;

			//Also add all old states that are lambda connected
			lambdaExtend(newState);

			//Search the newly generated state in AFD states, check if it exists
			bool alreadyExisting{ false };
			for (uint16_t i{0}; i < newStates.size(); ++i)
			{
				if (newStates[i] == newState)
				{
					//If it does, add new transition
					newTransitions[{ newIndex + 1, symbol }] =  i + 1;
					alreadyExisting = true;
					break;
				}
			}

			//If it doesn't, generate state, add it to queue and then add transition
			if (!alreadyExisting)
			{
				newStates.push_back(newState);
				newTransitions[{ newIndex + 1, symbol }] = newStates.size();
				queue.push(newStates.size() - 1);
			}
		}
	}

	//Rename all new states, they should be numbers
	std::vector<uint16_t> dfaStates;
	for (uint16_t i = 1; i <= newStates.size(); i++)
		dfaStates.push_back(i);

	//Find what new states should be final states
	std::vector<uint16_t> dfaFinalStates;
	for (uint16_t i = 0; i < newStates.size(); i++)
	{
		//If a new AFD state contains an old AFN state that is final, then AFD state should be final
		bool newStateIsFinalState{ false };
		for (auto element : newStates[i])
		{
			if (std::find(m_finalStates.begin(), m_finalStates.end(), element) != m_finalStates.end())
			{
				newStateIsFinalState = true;
				break;
			}
		}

		if (newStateIsFinalState)
			dfaFinalStates.push_back(i + 1);
	}

	return DFA{
		dfaStates,
		m_alphabet,
		newTransitions,
		1,
		dfaFinalStates
	};
}

void NFA::lambdaExtend(std::unordered_set<uint16_t>& us) const
{
	while (true)
	{
		std::unordered_set<uint16_t> newUs{ us };
		for (const auto& element : us)
		{
			//Find all lambda transitions for each AFN state and add them to AFD state, if it exists
			try {
				for (const auto& lambda : m_transitions.at({ element, m_lambda }))
					newUs.insert(lambda);
			}
			catch (...) {}
		}

		//If there haven't been any modifications stop
		if (us == newUs)
			return;

		//Else keep going
		us = newUs;
	}
}