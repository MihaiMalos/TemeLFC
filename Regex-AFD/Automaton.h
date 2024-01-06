#pragma once
#include <vector>
#include <string>
#include <map>

class Automaton
{
public:
	Automaton(
		const std::vector<uint16_t>& states = {}, 
		const std::vector<char>& alphabet = {}, 
		uint16_t startState = 0, 
		const std::vector<uint16_t>& finalStates = {}
			 );

	virtual ~Automaton() = default;


protected:
	const char m_lambda = '-';
	std::vector<char> m_alphabet;
	std::vector<uint16_t> m_states;
	std::vector<uint16_t> m_finalStates;
	uint16_t m_startState;

};