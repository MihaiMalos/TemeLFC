#include "FiniteAutomaton.h"

FiniteAutomaton::FiniteAutomaton(QPointF& screenPos)
	: m_screenPosition(screenPos)
{
	// Empty
}

QPointF FiniteAutomaton::GetPosition() const
{
	return m_screenPosition;
}

void FiniteAutomaton::SetPosition(QPointF& position)
{
	m_screenPosition = position;
}
