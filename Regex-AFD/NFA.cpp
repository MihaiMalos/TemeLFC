#include "NFA.h"

NFA::NFA(const std::vector<int>& states, const std::vector<char>& alphabet, const Transitions& transition, int startState, const std::vector<int>& finalStates)
{
}

NFA NFA::op_or(const NFA& other) const
{
	return NFA();
}

NFA NFA::op_concat(NFA other) const
{
	return NFA();
}

NFA NFA::op_kleene() const
{
	return NFA();
}
