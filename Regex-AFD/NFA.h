#pragma once
#pragma once
#include "DFA.h"
#include <unordered_set>

using InputNFA = std::pair<uint16_t, char>;
using TransitionsNFA = std::map<InputNFA, std::vector<uint16_t>>;

class NFA : public Automaton
{
public:
	NFA(const std::vector<uint16_t>& states = {}, const std::vector<char>& alphabet = {}, const TransitionsNFA& transition = TransitionsNFA(), uint16_t startState = 0, const std::vector<uint16_t>& finalStates = {});

	NFA op_or(const NFA& other) const;
	NFA op_concat(NFA other) const;
	NFA op_kleene() const;

	//State generation
	static uint16_t stateCounter;
	static uint16_t genState()
	{
		return ++stateCounter;
	}

private:
	TransitionsNFA m_transitions;
};