#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->clear();
    Section1.inializeVariables();

    Section1.EnterBoardLength( convertFeetInchestSixteenthsToSixteenths    (ui->BoardLengthFeetSpinBox->value(),
                                                                            ui->BoardLengthInchesSpinBox->value(),
                                                                            ui->BoardLengthSixteenthsSpinBox->value()) );

    Section1.EnterBoardWidth( convertFeetInchestSixteenthsToSixteenths     (ui->BoardWidthFeetSpinBox->value(),
                                                                            ui->BoardWidthInchesSpinBox->value(),
                                                                            ui->BoardWidthSixteenthsSpinBox->value()) );

    Section1.EnterSectionLength( convertFeetInchestSixteenthsToSixteenths  (ui->SectLenFeetSpinBox->value(),
                                                                            ui->SectLenInchesSpinBox->value(),
                                                                            ui->SectLenSixteenthsSpinBox->value()) );

    Section1.EnterSectionWidth( convertFeetInchestSixteenthsToSixteenths   (ui->SectionWidthFeetSpinBox->value(),
                                                                            ui->SectionWidthInchesSpinBox->value(),
                                                                            ui->SectionWidthSixteenthstSpinBox->value()) );

    ui->textBrowser->setText( Section1.GetCutList());

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->PreviewWindow->clear();
    ui->PreviewWindow->setText(Section1.GetPreview());
}
