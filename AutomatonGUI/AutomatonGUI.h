#pragma once

#include <QtWidgets/QMainWindow>
#include "Automaton.h"

class AutomatonGUI : public QMainWindow
{
    Q_OBJECT

public:
    AutomatonGUI(QWidget *parent = nullptr);
    ~AutomatonGUI();

protected:
	virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void paintEvent(QPaintEvent* event);

private:
    QString GetCurrentStateNotation();
	QPointF PointTranslation(QPointF firstPoint, QPointF secondPoint);
	double CalculateSlope(QPointF firstPoint, QPointF secondPoint);
    void InitTransitionDialog();

private:
    const int kCircleRadius = 20;
    int m_stateCounter = 0;
    QString m_selectedState;
    QString m_selectedTransition;
    QDialog* m_transitionDialog;
    std::shared_ptr<Automaton> m_automaton;
};
