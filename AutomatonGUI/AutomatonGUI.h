#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AutomatonGUI.h"

class AutomatonGUI : public QMainWindow
{
    Q_OBJECT

public:
    AutomatonGUI(QWidget *parent = nullptr);
    ~AutomatonGUI();

private:
    Ui::AutomatonGUIClass ui;
};
