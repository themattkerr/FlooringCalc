#ifndef FLOORSECTION_H
#define FLOORSECTION_H

#include <QString>
#include "mattcalculations.h"

struct board {
    int nLeangth;
    int nCutNumber;
};
struct row {
    board sStart;
    board sEnd;
    int nNumberOfCompleteBoards;
};

class FloorSection
{
private:

    int m_nSectionLength;
    int m_nSectionWidth;
    int m_nBoardWidth;
    int m_nBoardLength;
    int m_nNumberOfRows = 0;
    int m_nNumberOfPreviousRows = 0;
    int m_nWaste;
    int m_nRemainderLength;


    int m_nSawBladeWidth = 2;
    int m_nExpansionGap = 8;
    int m_nMinimumLength = (9*16);
    int m_nNumOfRowsToCheck = 5;
    int m_nOverlapMin = (19*16);
    int m_nStairStepRange = (2*16);
    int m_nCurrentCutNumber = 0;


    board m_sStarsBuffer[255];
    int m_nStartBufferIndex = 0;

    board m_sEndsBuffer[255];
    int m_nEndBufferIndex = 0;
    row m_sRow[255];

    void CalcNumRows();
    void CalcMinOverlap();
    void RemoveRow(int nRowToRemove);
    void RemovePreviousRows(int nNumberOfPreviousRows);
    int CalculateRemainderLength(int nBoardLength, int &nNumOfFullBoards);
    bool OkStartLength(int nLengthToCheck, int nRowNum);
    QString GenerateCutList();
    void CalcCutList();


public:
    FloorSection();
    void EnterSectionLength(int nSixteenthsOfAnInch);
    void EnterSectionWidth(int nSixteenthsOfAnInch);
    void EnterBoardLength(int nSixteenthsOfAnInch);
    void EnterBoardWidth(int nSixteenthsOfAnInch);
    void EnterPreviousRows(int nInputStartLength);

    int GetNumberOfRows();
    int GetTotalNumberOfBoardsUsed();

    QString GetCutList();
    void inializeVariables();

};

#endif // FLOORSECTION_H
