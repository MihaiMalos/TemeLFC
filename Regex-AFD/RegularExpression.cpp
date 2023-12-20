#include "RegularExpression.h"

const char RegularExpression::k_concatenation{ '.' };

bool RegularExpression::IsValid()
{
	return false;
}

DeterministicFiniteAutomaton RegularExpression::ConvertToAutomaton()
{

}

const uint16_t& RegularExpression::GetRank(const char& op) const
{
	switch (op)
	{
	case '(':
		return 0;
	case '|':
		return 1;
	case k_concatenation: // '.'
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

std::string RegularExpression::AddConcatenation(const std::string& expression) const
{
	std::string modifiedExpression{ expression };
	for (size_t index{0}; index < modifiedExpression.size() - 1; ++index)
	{
		std::string symbols("|(");
		symbols += k_concatenation;
		if (symbols.find(modifiedExpression[index]) != std::string::npos)
		{
			continue;
		}
		symbols = "|*)";
		symbols += k_concatenation;
		if (symbols.find(modifiedExpression[index + 1]) != std::string::npos)
		{
			continue;
		}
		modifiedExpression.insert(index + 1, 1, k_concatenation);
	}
	return std::move(modifiedExpression);
}