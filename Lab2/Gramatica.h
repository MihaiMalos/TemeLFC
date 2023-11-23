#pragma once

#include <set>
#include <string>
#include <tuple>
#include<regex>
#include<vector>

using Productie = std::pair<std::string, std::string>;

class Gramatica
{
public:
	bool verificare();
	void generare();
	bool isRegularGrammar();

public:
	friend std::istream& operator>>(std::istream& in, Gramatica& gramatica);
	friend std::ostream& operator<<(std::ostream& out, const Gramatica& gramatica);

private:
	bool VerificareCond1();
	bool VerificareCond2();
	bool VerificareCond3();
	bool VerificareCond4();
	bool VerificareCond5();

private:
	std::set<char> m_terminale, m_neterminale;
	char m_simbolStart;
	std::set<Productie> m_productii;
};

