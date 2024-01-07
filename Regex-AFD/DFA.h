#pragma once

#include "Automaton.h"
#include <iostream>

using InputDFA = std::pair<uint16_t, char>;
using TransitionsDFA = std::map<InputDFA, uint16_t>;

class DFA : public Automaton
{
public:
	DFA(
		const std::vector<uint16_t>& states = {},
		const std::vector<char>& alphabet = {}, 
		const TransitionsDFA& transitions = {},
		int startState = 0, 
		const std::vector<uint16_t>& finalStates = {}
		);

	bool CheckWord(std::string word);
	bool checkAutomatonValid() const;

private:
	TransitionsDFA m_transitions;

	bool checkInputStateInStates() const;
	bool checkFinalStatesInStates() const;
	bool checkTransitionsValid() const;

public:
	friend std::ostream& operator<<(std::ostream& out,
		const DFA& automaton);
};


