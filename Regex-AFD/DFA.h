#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

using Input = std::pair<char, char>;
using Output = char;
using Transitions = std::map<Input, Output>;

class DFA
{
public:
	DFA() = default;
	~DFA() = default;
	bool VerifyAutomaton();
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
		const DFA& automaton);
};


