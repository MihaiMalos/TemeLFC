#pragma once
#include "Automaton.h"
#include <stack>
#include <ranges>

using PDAInput = std::tuple<QString, char, char>;
using PDAOutput = std::vector<std::pair<QString, QString>>;
using PDATransitions = std::map<PDAInput, PDAOutput>;

class PDA : public Automaton
{
public:
	PDA(const Automaton& automaton);

	void AddTransition(QString& inputState, char symbol, char stackHead, QString& headReplacement, QString& outputState);
	CheckWordOutput CheckWord(const QString& word);

private:
	void LamdaExtend(std::vector<QString>& states, std::vector<std::stack<char>>& stack);

	PDATransitions m_transitions;
	const char m_startStack = '$';
};

