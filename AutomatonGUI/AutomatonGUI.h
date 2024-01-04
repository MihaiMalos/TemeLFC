#pragma once

#include <QtWidgets/QMainWindow>
#include "FiniteAutomaton.h"

class AutomatonGUI : public QMainWindow
{
    Q_OBJECT

public:
    AutomatonGUI(QWidget *parent = nullptr);
    ~AutomatonGUI();

protected:
	virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void paintEvent(QPaintEvent* event);

private:
    QString GetCurrentStateNotation();

private:
    const int kNodeRadius = 20;
    int m_stateCounter = 0;

    QString m_selectedState;
    std::unique_ptr<FiniteAutomaton> m_automaton;
};
