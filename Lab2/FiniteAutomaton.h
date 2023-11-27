#pragma once
#include"Grammar.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

using Input = std::pair<std::string, char>;
using Output = std::vector<std::string>;
using Transitions = std::map<Input, Output>;

class FiniteAutomaton
{
public:
	FiniteAutomaton();
	FiniteAutomaton(std::set<std::string> states, std::string firstState, 
	std::set<char> alphabet, std::set<std::string> finalStates, Transitions transitions);
	~FiniteAutomaton();
	bool VerifyAutomaton();
	//void CreateAutomaton(Grammar grammar);
	
private:
	std::set<std::string> m_states;
	std::string m_firstState;
	std::set<char> m_alphabet;
	std::set<std::string> m_finalStates;
	Transitions m_transitions;

private:
	bool VerifyCharacters();
	bool VerifyStartState();
	bool VerifyFinalStates();
	bool VerifyTransitionFunctions();


public:
	 friend std::ostream& operator<<(std::ostream& out, 
									 const FiniteAutomaton& automaton);
};

