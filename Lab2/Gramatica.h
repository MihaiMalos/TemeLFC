#pragma once

#include <set>
#include <string>
#include <tuple>

using Productie = std::pair<std::string, std::string>;

class Gramatica
{
public:
	friend std::istream& operator>>(std::istream& in, Gramatica& gramatica);
	friend std::ostream& operator<<(std::ostream& out, const Gramatica& gramatica);
private:
	std::set<char> m_terminale, m_neterminale;
	char m_simbolStart;
	std::set<Productie> m_productii;
};

