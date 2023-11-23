#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

using State = std::pair<std::string, std::string>;
using Transitions = std::map<Input, Output>;
using Input = std::pair<State, char>;
using Output = std::vector<State>;

class FiniteAutomaton
{

	
private:
	std::set<State> m_states;
	State m_firstState;
	std::set<char> m_alphabet;
	std::set<State> m_finalStates;
	Transitions m_transitions;
};

