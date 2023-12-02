#pragma once
#include "Grammar.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

using Input = std::pair<char, char>;
using Output = char;
using Transitions = std::multimap<Input,Output>;

class FiniteAutomaton
{
public:
	FiniteAutomaton();
	FiniteAutomaton(std::set<char> states, char firstState, 
	std::set<char> alphabet, char finalStates, Transitions transitions);
	~FiniteAutomaton();
	bool VerifyAutomaton();
	bool IsDeterministic();
	void CreateAutomaton(Grammar grammar);
	bool VerifyWord(std::string word);
	
private:
	std::set<char> m_states;
	char m_firstState;
	std::set<char> m_alphabet;
	std::set<char> m_finalStates;
	Transitions m_transitions;

private:
	const char lambda = '-';
	bool VerifyCharacters();
	bool VerifyStartState();
	bool VerifyFinalStates();
	bool VerifyTransitionFunctions();

public:
	 friend std::ostream& operator<<(std::ostream& out, 
									 const FiniteAutomaton& automaton);
};

