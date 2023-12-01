#include "FiniteAutomaton.h"

FiniteAutomaton::FiniteAutomaton()
{
}
FiniteAutomaton::FiniteAutomaton(std::vector<char> states, char firstState,
	std::set<char> alphabet,char finalStates, Transitions transitions)
	:m_states{ states }, m_firstState{ firstState }, m_alphabet{ alphabet }
	, m_finalStates{ finalStates }, m_transitions{ transitions }
{
}
FiniteAutomaton::~FiniteAutomaton()
{
}

void FiniteAutomaton::CreateAutomaton(Grammar grammar)
{
	if (!grammar.IsRegular()) std::cout << "Not a regular grammar\n";

	this->m_firstState = grammar.GetStart();
	this->m_alphabet = grammar.GetTerminals();
	std::set<Production> products = grammar.GetProductions();

	for (auto& product : products)
	{
		if (std::find(m_states.begin(), m_states.end(), product.first[0]) == m_states.end())
		{
			m_states.push_back(product.first[0]);
		}
		if (product.second.size() == 1) //Case if terminal
		{
			if (!m_finalStates)
			{
				m_finalStates = 'T';
			}
			m_transitions.insert(std::pair<Input, Output>({ product.first[0],product.second[0] }, m_finalStates));
		}
		else
		{
			m_transitions.insert(std::pair<Input, Output>({ product.first[0],product.second[0] }, product.second[1]));
		}
	}
}


bool FiniteAutomaton::VerifyAutomaton()
{
	if (VerifyCharacters() == true && VerifyFinalStates() == true && VerifyStartState() == true && VerifyTransitionFunctions() == true)
		return true;
	else return false;

}

bool FiniteAutomaton::IsDeterministic()
{
	for (auto transition : m_transitions)
	{
		if (m_transitions.count(transition.first) > 1)
		{
			return false;
		}
	}
	return true;
}

bool FiniteAutomaton::VerifyCharacters()
{
	for (const char& it : m_alphabet)
		if (std::find(m_states.begin(), m_states.end(), it) != m_states.end())
		{
			return false;
		}
	return true;
}
bool FiniteAutomaton::VerifyStartState()
{
	if (std::find(m_states.begin(), m_states.end(), m_firstState) == m_states.end())
		return false;

	return true;
}
bool FiniteAutomaton::VerifyFinalStates()
{
	if (std::find(m_states.begin(), m_states.end(), m_finalStates) == m_states.end())
		return false;
		
	return true;
}
bool FiniteAutomaton::VerifyTransitionFunctions()
{
	for (const auto& it : m_transitions)
	{
		if (std::find(m_states.begin(), m_states.end(), it.first.first) == m_states.end())
			return false;
		if (std::find(m_alphabet.begin(), m_alphabet.end(), it.first.second) == m_alphabet.end())
			return false;
		if (std::find(m_states.begin(), m_states.end(), it.second) == m_states.end())
			return false;
	
	}
	return true;
}

std::ostream& operator<<(std::ostream& out, const FiniteAutomaton& automaton)
{
	//TODO
	return out;
}