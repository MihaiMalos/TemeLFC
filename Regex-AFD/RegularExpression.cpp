#include "RegularExpression.h"

bool RegularExpression::IsValid()
{
	if (m_expression.empty())
		return false;

	size_t size{ m_expression.size() };
	std::stack<char> parentheses;

	for (uint16_t index{ 0 }; index < size; ++index)
	{
		if (m_expression[index] == '(')
		{
			parentheses.push(m_expression[index]);
		}

		if (m_expression[index] == ')')
		{
			if (parentheses.empty())
				return false;
			
			parentheses.pop();
		}

		if (m_expression[index] == '|')
		{
			if (index == 0 || index == size - 1)
				return false;

			// we can have ( after | but not )
			if (GetRank(m_expression[index - 1]) != -1 || 
				GetRank(m_expression[index + 1]) > 0   ||
				m_expression[index + 1] == ')' ) 
			{
				return false;
			}
		}

		if (m_expression[index] == '*')
		{
			if (index == 0)
				return false;

			// we could have concatenation after * but m_expression 
			// doesn't contain the concatenation operator
			if (GetRank(m_expression[index - 1]) != -1 ||
				GetRank(m_expression[index + 1]) > 0)
			{
				return false;
			}
		}
	}

	if (!parentheses.empty())
		return false;
	
	return true;
}

NFA RegularExpression::ConvertToAutomaton()
{
	std::string polishNotation = BuildPolishForm();
	std::stack<NFA> automata;
	for (char symbol : polishNotation)
	{
		if (GetRank(symbol) == -1)
		{
			automata.push(NFA::FromChar(symbol));
		}
		else
		{
			switch (symbol)
			{
			case '|':
			{
				NFA secondAutomaton = automata.top();
				automata.pop();
				NFA firstAutomaton = automata.top();
				automata.pop();
				automata.push(firstAutomaton.op_or(secondAutomaton));
				break;
			}
			case '.':
			{
				NFA secondAutomaton = automata.top();
				automata.pop();
				NFA firstAutomaton = automata.top();
				automata.pop();
				automata.push(firstAutomaton.op_concat(secondAutomaton));
				break;
			}
			case '*':
			{
				NFA automaton = automata.top();
				automata.pop();
				automata.push(automaton.op_kleene());
				break;
			}
			default:
				// if the Polish notation is correct, as it should be, there is no need for a default case but still
				break;
			}
		}
	}
	return std::move(automata.top());
}
const uint8_t& RegularExpression::GetRank(const char& op) const
{
	switch (op)
	{
	case '(':
		return 0;
	case '|':
		return 1;
	case  '.':
		return 2;
	case '*':
		return 3;
	default:
		return -1;
	}
}

std::string RegularExpression::BuildPolishForm()
{
	std::stack<char> operators;
	std::string polishForm;
	std::string expression{ AddConcatenation(m_expression) };

	for (const char& symbol : expression)
	{
		if (GetRank(symbol) == -1 && symbol != ')')
		{
			polishForm.push_back(symbol);
		}
		else if (symbol == '(')
		{
			operators.push(symbol);
		}
		else if (symbol == ')')
		{
			while (!operators.empty() && operators.top() != '(')
			{
				polishForm.push_back(operators.top());
				operators.pop();
			}
			operators.pop();
		}
		else
		{
			while (!operators.empty() && GetRank(symbol) <= GetRank(operators.top()))
			{
				polishForm.push_back(operators.top());
				operators.pop();
			}
			operators.push(symbol);
		}
	}
	while (!operators.empty())
	{
		polishForm.push_back(operators.top());
		operators.pop();
	}
	return std::move(polishForm);
}

void RegularExpression::ReduceStars()
{
	m_expression = std::regex_replace(m_expression, std::regex("\\*+"), "*");
}

std::string RegularExpression::AddConcatenation(const std::string& expression) const
{
	std::string modifiedExpression{ expression };
	for (uint16_t index{0}; index < modifiedExpression.size() - 1; ++index)
	{
		std::string symbols("|(");
		symbols += '.';
		if (symbols.find(modifiedExpression[index]) != std::string::npos)
		{
			continue;
		}
		symbols = "|*)";
		symbols += '.';
		if (symbols.find(modifiedExpression[index + 1]) != std::string::npos)
		{
			continue;
		}
		modifiedExpression.insert(index + 1, 1, '.');
	}
	return std::move(modifiedExpression);
}

std::istream& operator>>(std::istream& in, RegularExpression& expression)
{
	in >> expression.m_expression;
	expression.ReduceStars();
	return in;
}

std::ostream& operator<<(std::ostream& out, const RegularExpression& expression)
{
	out << expression.m_expression;
	return out;
}