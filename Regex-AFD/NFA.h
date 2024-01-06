#pragma once
#pragma once
#include "DFA.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct hash_pair {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const
	{
		auto hash1 = std::hash<T1>{}(p.first);
		auto hash2 = std::hash<T2>{}(p.second);

		if (hash1 != hash2) {
			return hash1 ^ hash2;
		}

		// If hash1 == hash2, their XOR is zero.
		return hash1;
	}
};

class NFA 
{
	using Input = std::pair<char, char>;
	using Output = char;
	using Transitions = std::map<Input, Output>;

public:
	NFA(const std::vector<int>& states = {}, const std::vector<char>& alphabet = {}, const Transitions& transition = Transitions(), int startState = 0, const std::vector<int>& finalStates = {});

	//Operator for OR, asserts AFNs are different
	NFA op_or(const NFA& other) const;
	//Operator for CONCATENATION, asserts AFNs are different
	NFA op_concat(NFA other) const;
	//Operator for KLEENE closure, asserts AFNs are different
	NFA op_kleene() const;

private:
	std::set<char> m_states;
	char m_firstState;
	std::set<char> m_alphabet;
	std::set<char> m_finalStates;
	Transitions m_transitions;
};