#include "Gramatica.h"
#include <iostream>

std::istream& operator>>(std::istream& in, Gramatica& gramatica)
{
	int nrTerminale, nrNeterminale, nrProductii;
	char simbol;

	in >> nrNeterminale;
	for (int index = 0; index < nrNeterminale; index++)
	{
		in >> simbol;
		gramatica.m_neterminale.insert(simbol);
	}

	in >> nrTerminale;
	for (int index = 0; index < nrTerminale; index++)
	{
		in >> simbol;
		gramatica.m_terminale.insert(simbol);
	}

	in >> gramatica.m_simbolStart;
	in >> nrProductii;

	for (int index = 0; index < nrProductii; index++)
	{
		std::string membruStang, membruDrept;
		in >> membruStang >> membruDrept;
		gramatica.m_productii.emplace(membruStang, membruDrept);
	}

	return in;
}
std::ostream& operator<<(std::ostream& out, const Gramatica& gramatica)
{
	out << "G = ({";
	for (auto& it : gramatica.m_neterminale)
	{
		out << it;
		if (it != *gramatica.m_neterminale.rbegin())
			out << ",";
	}
	out << "},";

	out << "{";
	for (auto it : gramatica.m_terminale)
	{
		out << it;
		if (it != *gramatica.m_terminale.rbegin())
			out << ",";
	}
	out << "},";

	out << gramatica.m_simbolStart << ", P), P continand urmatoarele productii:\n";

	int index = 0;
	for (auto it : gramatica.m_productii)
	{
		out << "(" << ++index << ") ";
		out << it.first << " -> " << it.second << std::endl;
	}

	return out;
}

bool Gramatica::VerificareCond1()
{
	for (auto terminal : m_terminale)
		if (m_neterminale.find(terminal) != m_neterminale.end())
			return false;
	return true;
}
bool Gramatica::VerificareCond2()
{
	return m_neterminale.find(m_simbolStart) == m_neterminale.end();
}
bool Gramatica::VerificareCond3()
{
	std::regex myPattern(R"([a-z]*[A-Z]+[a-z]*)");
	for (auto productie : m_productii)
		if (!std::regex_match(productie.first, myPattern))
			return false;
	return true;
}
bool Gramatica::VerificareCond4()
{
	std::string simbol(1, m_simbolStart);
	for (auto productie : m_productii)
		if (productie.first == simbol)
			return true;
	return false;
}
bool Gramatica::VerificareCond5()
{
	return false;
}

bool Gramatica::verificare()
{
	if (!VerificareCond1() ||
		!VerificareCond2() ||
		!VerificareCond3() ||
		!VerificareCond4())
		return false;	
	return true;
}

void Gramatica::generare()
{

}
