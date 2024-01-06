#pragma once

#include "Automaton.h"
#include <iostream>

using InputDFA = std::pair<uint16_t, char>;
using TransitionsDFA = std::map<InputDFA, uint16_t>;

class DFA : public Automaton
{
public:
	DFA() = default;

	bool VerifyWord(std::string word);

private:
	TransitionsDFA m_transitions;

public:
	friend std::ostream& operator<<(std::ostream& out,
		const DFA& automaton);
};


