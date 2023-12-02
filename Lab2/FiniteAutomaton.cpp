#include "FiniteAutomaton.h"

FiniteAutomaton::FiniteAutomaton()
{
}
FiniteAutomaton::FiniteAutomaton(std::set<char> states, char firstState,
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
	if (!grammar.IsRegular()) 
	{
		std::cout << "Not a regular grammar\n";
		return;
	}

	this->m_firstState = grammar.GetStart();
	this->m_alphabet = grammar.GetTerminals();
	std::set<Production> products = grammar.GetProductions();

	for (auto& product : products)
	{
		if (std::find(m_states.begin(), m_states.end(), product.first[0]) == m_states.end())
		{
			m_states.insert(product.first[0]);
		}
		if (product.second.size() == 1) //Case if terminal
		{
			if (product.first == &m_firstState && product.second == &lambda)
			{
				m_finalStates.insert(m_firstState);
				m_transitions.insert(std::pair<Input, Output>({ product.first[0],product.second[0] }, m_firstState));
			}
			else
			{
				if (m_finalStates.find('T') == m_finalStates.end())
				{
					m_finalStates.insert('T');
					m_states.insert('T');
				}
				m_transitions.insert(std::pair<Input, Output>({ product.first[0],product.second[0] }, 'T'));
			}
		}
		else
		{
			m_transitions.insert(std::pair<Input, Output>({ product.first[0],product.second[0] }, product.second[1]));
		}
	}
}

bool FiniteAutomaton::VerifyWord(std::string word)
{
	std::set<char> currentStateSet;
	currentStateSet.insert(m_firstState);

	for (const char& symbol : word)
	{
		std::set<char> nextStateSet;

		for (const char& state : currentStateSet)
			if (m_transitions.find({ state,symbol }) != m_transitions.end())
			{
				auto range = m_transitions.equal_range({ state,symbol });

				//find all transitions possible
				for (auto it = range.first; it != range.second; ++it)
					nextStateSet.insert(it->second);
			}
		currentStateSet = nextStateSet;
	}

	for (const auto& finalState : m_finalStates)
	{
		if (currentStateSet.find(finalState) != currentStateSet.end())
			return true;
	}

	return false;
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
		if (m_states.find(it) != m_states.end())
		{
			return false;
		}
	return true;
}
bool FiniteAutomaton::VerifyStartState()
{
	if (m_states.find(m_firstState) == m_states.end())
		return false;

	return true;
}
bool FiniteAutomaton::VerifyFinalStates()
{
	for (const char& it : m_finalStates)
	{
		if (m_states.find(it) == m_states.end())
			return false;
	}

		
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
	out << "M = ({";
	for (auto& it : automaton.m_states)
	{
		out << it;
		if (it != *automaton.m_states.rbegin())
			out << ",";
	}
	out << "},";

	out << "{";
	for (auto it : automaton.m_alphabet)
	{
		out << it;
		if (it != *automaton.m_alphabet.rbegin())
			out << ",";
	}
	out << "},";

	out << "delta," << automaton.m_firstState << ",";

	out << "{";
	for (auto it : automaton.m_finalStates)
	{
		out << it;
		if (it != *automaton.m_finalStates.rbegin())
			out << ",";
	}
	out << "}), delta continand urmatoarele tranzitii:\n";

	for (const auto& transition : automaton.m_transitions)
	{
		out << "delta(" << transition.first.first << ", " << transition.first.second << ")";
		out << " = " << transition.second << std::endl;
	}

	return out;
}