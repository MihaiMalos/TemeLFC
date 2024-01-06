#include "PDA.h"

PDA::PDA(const Automaton& automaton) : Automaton(automaton)
{
	// Empty
}

void PDA::AddTransition(QString& inputState, char symbol, char stackHead, QString& headReplacement, QString& outputState)
{
	m_transitions[{inputState, symbol, stackHead}].push_back({ headReplacement, outputState });
}

bool PDA::CheckWord(const QString& word)
{
	std::vector<QString> currentStateList;
	std::vector<std::stack<char>> currentStackList;
	currentStateList.push_back(m_startState);
	std::stack<char> startStack;
	startStack.push(m_startStack);
	currentStackList.push_back(startStack);

	for (const auto& symbol : word)
	{
		std::vector<QString> nextStateList;
		std::vector<std::stack<char>> nextStackList;

		for (int index = 0; index < currentStateList.size(); index++)
		{
			auto state = currentStateList[index];
			auto stack = currentStackList[index];

			auto stateRange = m_transitions.find({ state, symbol.toLatin1(), stack.top() });

			if (stateRange != m_transitions.end())
			{
				for (auto& outputState : stateRange->second)
				{
					auto newStack = stack;
					newStack.pop();
					if (outputState.first != m_lambda)
					{
						for (auto it = outputState.first.rbegin(); it != outputState.first.rend(); ++it)
						{
							newStack.push(it->toLatin1());
						}
					}

					nextStateList.push_back(outputState.second);
					nextStackList.push_back(newStack);

					auto lambdaStateRange = m_transitions.find({ outputState.second, m_lambda, newStack.top() });
					if (lambdaStateRange != m_transitions.end())
					{
						for (auto& lambdaState : lambdaStateRange->second)
						{
							auto lambdaNewStack = newStack;
							lambdaNewStack.pop();
							if (lambdaState.first != m_lambda)
							{
								for (auto it = lambdaState.first.rbegin(); it != lambdaState.first.rend(); ++it)
								{
									lambdaNewStack.push(it->toLatin1());
								}
							}
							nextStateList.push_back(lambdaState.second);
							nextStackList.push_back(lambdaNewStack);
						}
					}
				}
			}
		}
		currentStateList = nextStateList;
		currentStackList = nextStackList;
	}

	for (const auto& finalState : m_finalStates)
	{
		for (int index = 0; index < currentStateList.size(); index++)
		{
			const auto& currentState = currentStateList[index];
			const auto& currentStack = currentStackList[index];
			if (currentStack.size() == 1 && currentStack.top() == m_startStack && currentState == finalState)
			{
				return true;
			}
		}
	}
	return false;
}
