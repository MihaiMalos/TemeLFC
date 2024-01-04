#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

#include "AutomatonGUI.h"
#include "DFA.h"


AutomatonGUI::AutomatonGUI(QWidget *parent)
    : QMainWindow(parent)
    , m_automaton{std::make_unique<DFA>()}
{
    resize(1000, 600);
}

AutomatonGUI::~AutomatonGUI()
{

}

void AutomatonGUI::mousePressEvent(QMouseEvent* event)
{
	bool canCreate = true;
	QString toSelect = "";

    auto automatonStates = m_automaton->GetStatesPositions();
	for (const auto& [state, position] : automatonStates)
	{
		if (fabs(position.x() - event->pos().x()) < 2 * kNodeRadius && fabs(position.y() - event->pos().y()) < 2 * kNodeRadius)
			canCreate = false;
		if (fabs(position.x() - event->pos().x()) < kNodeRadius && fabs(position.y() - event->pos().y()) < kNodeRadius)
			toSelect = state;
	}

    if (event->button() == Qt::LeftButton)
    {
        if (canCreate)
        {
            m_automaton->AddState("q" + QString::number(m_stateCounter++), event->pos());
            m_selectedState = "";
        }
        else if (!toSelect.isEmpty())
        {
            m_selectedState = toSelect;
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        if (toSelect.isEmpty()) 
        {
            m_selectedState = "";
        }
        else if (!canCreate)
        {
            if (m_selectedState != toSelect) m_selectedState = toSelect;
            else m_selectedState = "";
        }
    }
    update();
}

void AutomatonGUI::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		auto automatonStates = m_automaton->GetStatesPositions();
        for (const auto& [state, position] : automatonStates)
        {
            if (m_selectedState == state) continue;
            if (fabs(position.x() - event->pos().x()) < 2 * kNodeRadius && fabs(position.y() - event->pos().y()) < 2 * kNodeRadius)
                return;
        }
        m_automaton->SetStatePosition(m_selectedState, event->pos());
        update();
	}
}

void AutomatonGUI::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPen pen;
    QFont font;

    font.setPixelSize(13);
    font.setWeight(QFont::DemiBold);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(font);
    pen.setWidthF(1.5f);
    

    auto automatonStates = m_automaton->GetStatesPositions();
    for (const auto& [state, position] : automatonStates)
    {
        if (m_selectedState == state) pen.setColor(Qt::red);
        else pen.setColor(Qt::black);
        painter.setPen(pen);

        QRectF rect(position.x() - kNodeRadius, position.y() - kNodeRadius, 2 * kNodeRadius, 2 * kNodeRadius);
        painter.drawEllipse(rect);
        painter.drawText(rect, Qt::AlignCenter, state);
    }
}

QString AutomatonGUI::GetCurrentStateNotation()
{
    return "q" + QString::number(m_stateCounter++);
}
