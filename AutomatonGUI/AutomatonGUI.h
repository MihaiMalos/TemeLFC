#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AutomatonGUI.h"
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

private slots:
    void on_checkWord_clicked();
    void on_save_clicked();
    void on_load_clicked();


private:
    QString GetCurrentStateNotation();
	QPointF PointTranslation(QPointF firstPoint, QPointF secondPoint);
	double CalculateSlope(QPointF firstPoint, QPointF secondPoint);
    void InitTransitionDialog();
    void InitCheckWordDialog();

private:
    const int kCircleRadius = 20;
    int m_stateCounter = 0;
    QString m_selectedState, m_selectedTransition, m_selectedWord;
    QDialog* m_transitionDialog, *m_checkWordDialog;
    std::shared_ptr<Automaton> m_automaton;
    Ui::AutomatonGUIClass ui;
};
