#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->maxWasteSpinBox->setValue(Section1.GetMaxWasteInches());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    ui->textBrowser->clear();
    Section1.inializeVariables();

    m_sLast5Starts.nNumberOfBoards = 5;
    for (int iii = 1; iii <= m_sLast5Starts.nNumberOfBoards; iii++)
    {
        m_sLast5Starts.sBrd[iii].bRequires2ndCut = false;
        m_sLast5Starts.sBrd[iii].nCutNumber = -1;
        m_sLast5Starts.sBrd[iii].nLeangth = -1;
    }
    m_sLast5Starts.sBrd[1].nLeangth = (convertFeetInchestSixteenthsToSixteenths( 1,8,0));
    m_sLast5Starts.sBrd[2].nLeangth = (convertFeetInchestSixteenthsToSixteenths( 4,11,0));
    m_sLast5Starts.sBrd[3].nLeangth = (convertFeetInchestSixteenthsToSixteenths( 2,2,0 ));
    m_sLast5Starts.sBrd[4].nLeangth = (convertFeetInchestSixteenthsToSixteenths( 5,7,6 ));
    m_sLast5Starts.sBrd[5].nLeangth = (convertFeetInchestSixteenthsToSixteenths( 1,2,0 ));


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

    Section1.EnterLast5Starts(m_sLast5Starts);

    ui->textBrowser->setText( Section1.GetCutList());

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->PreviewWindow->clear();
    ui->PreviewWindow->setText(Section1.GetPreview());
}

void MainWindow::on_maxWasteSpinBox_valueChanged(int arg1)
{
    Section1.EnterMaxWaste(arg1);
}
