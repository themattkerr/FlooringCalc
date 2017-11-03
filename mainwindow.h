#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "floorsection.h"
#include "mattcalculations.h"
#include "varibledeclarations.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_maxWasteSpinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;

    FloorSection Section1;
    s5Boards m_sLast5Starts;

};

#endif // MAINWINDOW_H
