#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>

#include "AutomatonGUI.h"
#include "DFA.h"
#include "NFA.h"


AutomatonGUI::AutomatonGUI(QWidget *parent)
    : QMainWindow(parent)
    , m_automaton{std::make_shared<Automaton>()}
{
    InitTransitionDialog();
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
		if (fabs(position.x() - event->pos().x()) < 2 * kCircleRadius && fabs(position.y() - event->pos().y()) < 2 * kCircleRadius)
			canCreate = false;
		if (fabs(position.x() - event->pos().x()) < kCircleRadius && fabs(position.y() - event->pos().y()) < kCircleRadius)
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
            if (m_selectedState.isEmpty()) m_selectedState = toSelect;
            else
            {
                m_transitionDialog->exec();
                if (!m_selectedTransition.isEmpty())
                {
                    m_automaton->AddModelTransition(m_selectedState + " " + m_selectedTransition + " " + toSelect);
                    qDebug() << (int) m_automaton->GetAutomatonType();
                }
                m_selectedState = "";
                m_selectedTransition = "";
            }
        }
    }
    else if (event->button() == Qt::MiddleButton && !toSelect.isEmpty())
    {
        m_automaton->IsFinalState(toSelect)
            ? m_automaton->RemoveFinalState(toSelect)
            : m_automaton->MakeStateFinal(toSelect);
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
            if (fabs(position.x() - event->pos().x()) < 2 * kCircleRadius && fabs(position.y() - event->pos().y()) < 2 * kCircleRadius)
                return;
        }
        m_automaton->SetStatePosition(m_selectedState, event->pos());
        update();
	}
}

void AutomatonGUI::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete && !m_selectedState.isEmpty())
    {
        m_automaton->RemoveState(m_selectedState);
        m_selectedState = "";
    }
    if (event->key() == Qt::Key_S && !m_selectedState.isEmpty())
    {
        m_automaton->MakeStateStart(m_selectedState);
        m_selectedState = "";
    }
	if (event->key() == Qt::Key_F && !m_selectedState.isEmpty())
	{
		m_automaton->IsFinalState(m_selectedState)
			? m_automaton->RemoveFinalState(m_selectedState)
			: m_automaton->MakeStateFinal(m_selectedState);
		m_selectedState = "";
	}
	if (event->key() == Qt::Key_Escape)
	{
		m_selectedState = "";
	}
    update();
}

void AutomatonGUI::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPen pen;
    QFont font;

    font.setPixelSize(13);
    font.setWeight(QFont::DemiBold);
    pen.setWidthF(1.5f);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(font);

    auto automatonStates = m_automaton->GetStatesPositions();
    for (const auto& [state, position] : automatonStates)
    {
        if (m_selectedState == state) pen.setColor(Qt::red);
        else pen.setColor(Qt::black);
        painter.setPen(pen);

        QRectF smallCircle(position.x() - kCircleRadius, position.y() - kCircleRadius, 2 * kCircleRadius, 2 * kCircleRadius);
        painter.drawEllipse(smallCircle);

        if (m_automaton->IsFinalState(state)) 
        {
            QRectF bigCircle;
            bigCircle.setSize(smallCircle.size() * 1.25f);
            bigCircle.moveCenter(smallCircle.center());
            painter.drawEllipse(bigCircle);

            if (m_automaton->IsFirstState(state))
            {
                // code to be added
            }
        }
        else if (m_automaton->IsFirstState(state))
        {
            // code to be added
        }
        painter.drawText(smallCircle, Qt::AlignCenter, state);
    }

    pen.setColor(Qt::black);
    painter.setPen(pen);

    auto automatonTransitions = m_automaton->GetTransitions();
    for (auto transition : automatonTransitions)
    {
        const auto& firstState = automatonStates.find(transition[0]);
        const auto& secondState = automatonStates.find(transition[transition.size()-1]);
        if (firstState == automatonStates.end() || secondState == automatonStates.end()) continue;
        if (firstState != secondState)
        {
            const auto& [firstStateName, firstStatePos] = *firstState;
            const auto& [secondStateName, secondStatePos] = *secondState;

            const auto& centerToBorderOffset = PointTranslation(firstStatePos, secondStatePos);
            const auto startPoint = firstStatePos + centerToBorderOffset;
            const auto endPoint = secondStatePos - centerToBorderOffset;

            QPointF controlPoint(startPoint.x() + (endPoint.x() - startPoint.x()) / 2.0f, startPoint.y() - (endPoint.y() - startPoint.y()) / 2.0f);
            QPainterPath path;
            path.moveTo(startPoint);
            path.quadTo(controlPoint, endPoint);
            painter.drawPath(path);

			static const int arrowOffset = 30;
			double angle = atan(CalculateSlope(controlPoint, secondStatePos)) * 180 / 3.14;
			if (firstStatePos.x() < secondStatePos.x()) angle -= 180;
			QLineF angleline;
			angleline.setP1(endPoint);
			angleline.setLength(kCircleRadius);

			pen.setColor(Qt::darkCyan);
			painter.setPen(pen);
			angleline.setAngle(angle + arrowOffset);
			painter.drawLine(angleline);
			angleline.setAngle(angle - arrowOffset);
			painter.drawLine(angleline);
			pen.setColor(Qt::black);
			painter.setPen(pen);
        }
        else
        {
            // arrow inside point
        }

    }
}

QString AutomatonGUI::GetCurrentStateNotation()
{
    return "q" + QString::number(m_stateCounter++);
}

QPointF AutomatonGUI::PointTranslation(QPointF firstPoint, QPointF secondPoint)
{
	double slope = CalculateSlope(firstPoint, secondPoint);

	return firstPoint.x() < secondPoint.x()
		? QPointF{ kCircleRadius * cos(atan(slope)), -kCircleRadius * sin(atan(slope)) }
	    : QPointF{ -kCircleRadius * cos(atan(slope)), kCircleRadius * sin(atan(slope)) };
}

double AutomatonGUI::CalculateSlope(QPointF firstPoint, QPointF secondPoint)
{
    return (double)-(firstPoint.y() - secondPoint.y()) / (firstPoint.x() - secondPoint.x());
}

void AutomatonGUI::InitTransitionDialog()
{
	m_transitionDialog = new QDialog();
	QGridLayout* layout = new QGridLayout();

	QLineEdit* lineEdit = new QLineEdit(m_transitionDialog);
	QPushButton* okButton = new QPushButton("Confirm", m_transitionDialog);

	layout->addWidget(lineEdit);
	layout->addWidget(okButton);
	m_transitionDialog->setLayout(layout);

	connect(okButton, &QPushButton::clicked, [=]() {
		m_selectedTransition = lineEdit->displayText();
		lineEdit->setText("");
		m_transitionDialog->close();
		});
}