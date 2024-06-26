#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QThread>
#include <QPlainTextEdit>

#include "AutomatonGUI.h"
#include "DFA.h"
#include "NFA.h"
#include "LambdaNFA.h"
#include "PDA.h"


AutomatonGUI::AutomatonGUI(QWidget *parent)
    : QMainWindow(parent)
    , m_automaton{std::make_shared<Automaton>()}
{
    ui.setupUi(this);
    InitTransitionDialog();
    InitCheckWordDialog();
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
                }
                m_selectedState = "";
                m_selectedTransition = "";
            }
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

    font.setPixelSize(20);
    painter.setFont(font);
    painter.drawText(QPointF{ 225, 40 }, m_animationOutput);
    font.setPixelSize(13);
    painter.setFont(font);

    auto automatonStates = m_automaton->GetStatesPositions();
    for (const auto& [state, position] : automatonStates)
    {
        if (m_selectedState == state) pen.setColor(Qt::red);
        else if (m_automaton->IsStartState(state)) pen.setColor(Qt::darkGreen);
        else pen.setColor(Qt::black);
        

        for (auto animationState : m_animationStates)
        {
            if (state == animationState)
            {
                pen.setColor(Qt::blue);
                break;
            }
        }
        painter.setPen(pen);

        QRectF smallCircle(position.x() - kCircleRadius, position.y() - kCircleRadius, 2 * kCircleRadius, 2 * kCircleRadius);
        painter.drawEllipse(smallCircle);

        if (m_automaton->IsFinalState(state)) 
        {
            QRectF bigCircle;
            bigCircle.setSize(smallCircle.size() * 1.25f);
            bigCircle.moveCenter(smallCircle.center());
            painter.drawEllipse(bigCircle);
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
		const auto& [firstStateName, firstStatePos] = *firstState;
		const auto& [secondStateName, secondStatePos] = *secondState;
        if (firstState != secondState)
        {

            const auto& centerToBorderOffset = PointTranslation(firstStatePos, secondStatePos);
            const auto startPoint = firstStatePos + centerToBorderOffset;
            const auto endPoint = secondStatePos - centerToBorderOffset;

            QPointF controlPoint(startPoint.x() + (endPoint.x() - startPoint.x()) / 2.0f, startPoint.y() - (endPoint.y() - startPoint.y()) / 2.0f);
			QPointF textPoint;

			if (std::abs(startPoint.x() - endPoint.x()) <= 30)
			{
				if (startPoint.x() < endPoint.x())
					textPoint = { controlPoint.x() / 2 ,controlPoint.y() };
				else
					textPoint = { controlPoint.x() * 2,controlPoint.y() };
			}

			if (std::abs(startPoint.y() - endPoint.y()) < 150)
				textPoint = controlPoint;
			else
				if (startPoint.y() > endPoint.y())
					textPoint = { controlPoint.x(), std::abs(startPoint.y() - endPoint.y()) };
				else
					textPoint = { controlPoint.x(), std::abs(startPoint.y() - endPoint.y()) * 2 };

            QPainterPath path;
            path.moveTo(startPoint);
            path.quadTo(controlPoint, endPoint);
            painter.drawPath(path);
            QString transitionText = "";
            if (transition.size() == 3)
            {
                transitionText = transition[1];
            }
            else if (transition.size() == 5)
            {
                transitionText = transition[1] + ", " + transition[2] + " / " + transition[3];
            }
            painter.drawText(textPoint, transitionText);

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
            const auto& upPoint = firstStatePos + QPointF(0, -kCircleRadius);
            const auto& leftPoint = firstStatePos + QPointF(-kCircleRadius, 0);
            
            QPainterPath path;
            path.moveTo(upPoint);
            QPointF center = (upPoint + leftPoint) / 2.0;
			qreal rx = QLineF(center, upPoint).length();
			qreal ry = rx; // Increase the aspect ratio
			qreal startAngle = QLineF(center, upPoint).angle();
			qreal endAngle = QLineF(center, leftPoint).angle();
            path.arcTo(center.x() - rx, center.y() - ry, 2 * rx, 2 * ry, startAngle, endAngle - startAngle);
            painter.drawPath(path);

			static const int arrowOffset = 30;
			double angle =  120;
			QLineF angleline;
			angleline.setP1(leftPoint);
			angleline.setLength(kCircleRadius/2.0f);

			pen.setColor(Qt::darkCyan);
			painter.setPen(pen);
			angleline.setAngle(angle + arrowOffset);
			painter.drawLine(angleline);
			angleline.setAngle(angle - arrowOffset);
			painter.drawLine(angleline);
			pen.setColor(Qt::black);
			painter.setPen(pen);

			QString transitionText = "";
			if (transition.size() == 3)
			{
				transitionText = transition[1];
			}
			else if (transition.size() == 5)
			{
				transitionText = transition[1] + "; " + transition[2] + " / " + transition[3];
			}

            const float textDistanceOffset = 1.5f;
            QPointF textPoint = firstStatePos - textDistanceOffset * QPoint(kCircleRadius, kCircleRadius);
			painter.drawText(textPoint, transitionText);
        }

    }
}

void AutomatonGUI::on_checkWord_clicked()
{
    m_selectedState = "";
    m_checkWordDialog->exec();
    CheckWordOutput output = CheckWord(m_selectedWord);

    int index = 0;
    for (const auto& states : output.first)
    {
        m_animationStates = states;
        m_animationOutput = m_selectedWord.left(index++);
        repaint();
        QThread::sleep(kAnimationStepTime);
    }
    m_animationStates = {};
    m_animationOutput = output.second ? "Word is accepted" : "Word is NOT accepted";
    repaint();

}

void AutomatonGUI::on_save_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Text Files (*.txt)");
    if (!filePath.isNull()) m_automaton->SaveAutomaton(filePath);
}

void AutomatonGUI::on_load_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Load File", "", "Text Files (*.txt)");
    if (!filePath.isNull()) m_automaton->LoadAutomaton(filePath);

}

void AutomatonGUI::on_clear_clicked()
{
    m_automaton->Clear();
    m_stateCounter = 0;
    m_selectedState = m_selectedTransition = m_selectedWord = m_animationOutput = "";
    update();
}

void AutomatonGUI::on_loadWords_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Load File", "", "Text Files (*.txt)");
	if (filePath.isNull()) return;
	QPlainTextEdit* textBox = new QPlainTextEdit();
	textBox->setReadOnly(true);

	QFile file(filePath);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString line = in.readLine();
		auto output = CheckWord(line);
		QColor color = output.second ? Qt::darkGreen : Qt::red;
		line = QString("<font color='%1'>%2</font>").arg(color.name()).arg(line);
		textBox->appendHtml(line);
	}
	file.close();
	textBox->show();
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

void AutomatonGUI::InitCheckWordDialog()
{
	m_checkWordDialog = new QDialog();
	QGridLayout* layout = new QGridLayout();

	QLineEdit* lineEdit = new QLineEdit(m_checkWordDialog);
	QPushButton* okButton = new QPushButton("Check word", m_checkWordDialog);

	layout->addWidget(lineEdit);
	layout->addWidget(okButton);
	m_checkWordDialog->setLayout(layout);

	connect(okButton, &QPushButton::clicked, [=]() {
		m_selectedWord = lineEdit->displayText();
		lineEdit->setText("");
		m_checkWordDialog->close();
		});
}

CheckWordOutput AutomatonGUI::CheckWord(const QString& word)
{
	CheckWordOutput output;
	switch (m_automaton->GetAutomatonType())
	{
	case AutomatonType::DFA:
	{
		std::shared_ptr<DFA> dfa = std::make_shared<DFA>(*m_automaton.get());
		for (auto transition : dfa->GetTransitions())
		{
			dfa->AddTransition(transition[0], transition[1].at(0).toLatin1(), transition[2]);
		}
		output = dfa->CheckWord(word);
		break;
	}
	case AutomatonType::NFA:
	{
		std::shared_ptr<NFA> nfa = std::make_shared<NFA>(*m_automaton.get());
		for (auto transition : nfa->GetTransitions())
		{
			nfa->AddTransition(transition[0], transition[1].at(0).toLatin1(), transition[2]);
		}
		output = nfa->CheckWord(word);
		break;
	}
	case AutomatonType::LambdaNFA:
	{
		std::shared_ptr<LambdaNFA> lambdaNFA = std::make_shared<LambdaNFA>(*m_automaton.get());
		for (auto transition : lambdaNFA->GetTransitions())
		{
			lambdaNFA->AddTransition(transition[0], transition[1].at(0).toLatin1(), transition[2]);
		}
		output = lambdaNFA->CheckWord(word);
		break;
	}
	case AutomatonType::PDA:
	{
		std::shared_ptr<PDA> pda = std::make_shared<PDA>(*m_automaton.get());
		for (auto transition : pda->GetTransitions())
		{
			pda->AddTransition(transition[0], transition[1].at(0).toLatin1(), transition[2].at(0).toLatin1(), transition[3], transition[4]);
		}
		output = pda->CheckWord(word);
		break;
	}
	case AutomatonType::None:
	{
		output = { {}, false };
		qDebug() << "Not a valid automaton type";
		break;
	}
	}
	return output;
}
