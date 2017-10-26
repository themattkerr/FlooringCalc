#include "floorsection.h"
#include <qmath.h>

FloorSection::FloorSection()
{
    //inializeVariables();
}


void FloorSection::CalcNumRows()
{
    if (m_nBoardWidth > 0 && m_nSectionWidth >0)
        m_nNumberOfRows = m_nSectionWidth/m_nBoardWidth;
    else
        m_nNumberOfRows = 0;
}

bool FloorSection::OkStartLength(int nLengthToCheck, int nRowNum)
{
    int nNumberOfFullBoards = 0;
    if (nLengthToCheck < m_nMinimumLength || nLengthToCheck > m_nBoardLength )
        return false;
    int nVariableOverlap = m_nOverlapMin;
    for (int nRowCheck = nRowNum - 1, nRowsToCheck = m_nNumOfRowsToCheck; nRowCheck > 0 && nRowsToCheck > 0; nRowCheck--, nRowsToCheck--)
    {
        int nDifference = qFabs(m_sRow[nRowCheck].sStart.nLeangth - nLengthToCheck);
        if(nDifference < nVariableOverlap)
            return false;
        int nTemp = CalculateRemainderLength(nLengthToCheck,nNumberOfFullBoards );
        if (nTemp < m_nMinimumLength)
            return false;
        nVariableOverlap = nVariableOverlap / 5;
    }
    return true;
}
int FloorSection::CalculateRemainderLength(int nBoardLength, int &nNumOfFullBoards)
{
    nNumOfFullBoards =  (m_nSectionLength- (2*m_nExpansionGap) - nBoardLength) / m_nBoardLength;
    int nRemainderLength = m_nSectionLength - nBoardLength - (nNumOfFullBoards*m_nBoardLength) - (2*m_nExpansionGap);
    return nRemainderLength;
}

QString FloorSection::GenerateCutList()
{
    bool bAllRowsDone = false;
    QString strReport;
    int nCutIndex = 1;
    for (int nCutNum = 1;nCutNum <= m_nCurrentCutNumber; nCutNum++)
    {
        strReport.append("Cut ").append(QString::number( (nCutNum)).append("\n"));
        for(int nTRow = 1; nTRow <= m_nNumberOfRows; nTRow++ )
        {
            if(m_sRow[nTRow].sStart.nCutNumber == nCutNum )
            {
                int nFeet, nInches, Sixteeths;
                strReport.append("Row: ").append(QString::number(nTRow)).append("\t");
                convertSixteenthsToFeetInchesSixteenths(m_sRow[nTRow].sStart.nLeangth, nFeet, nInches, Sixteeths );
                strReport.append("Start: ").append(QString::number(nFeet)).append(" ft  ").append(QString::number(nInches)).append(" in  ").append(QString::number(Sixteeths)).append(" Sixteenths")   ;
                strReport.append("\n");
            }
        }
        for(int nTRow = 1; nTRow <= m_nNumberOfRows; nTRow++ )
        {
            if(m_sRow[nTRow].sEnd.nCutNumber == nCutNum )
            {
                int nFeet, nInches, Sixteeths;
                strReport.append("Row: ").append(QString::number(nTRow)).append("\t");
                convertSixteenthsToFeetInchesSixteenths(m_sRow[nTRow].sEnd.nLeangth, nFeet, nInches, Sixteeths );
                strReport.append("End: ").append(QString::number(nFeet)).append(" ft  ").append(QString::number(nInches)).append(" in  ").append(QString::number(Sixteeths)).append(" Sixteenths")   ;
                strReport.append("\n");
            }
        }
        strReport.append("\n");
    }
    strReport.append("Extra Starts: ").append("\n");
    for (int iii = 1; iii <= m_nStartBufferIndex; iii++)
    {

         int nFeet, nInches, Sixteeths;
         convertSixteenthsToFeetInchesSixteenths(m_sStarsBuffer[iii].nCutNumber, nFeet, nInches, Sixteeths );
         strReport.append("--->  ").append(QString::number(nFeet)).append(" ft  ").append(QString::number(nInches)).append(" in  ").append(QString::number(Sixteeths)).append(" Sixteenths")   ;
         strReport.append("\n");

    }
    strReport.append("Extra Ends: ").append("\n");
    for (int iii = 1; iii <= m_nEndBufferIndex; iii++)
    {

         int nFeet, nInches, Sixteeths;
         convertSixteenthsToFeetInchesSixteenths(m_sEndsBuffer[iii].nCutNumber, nFeet, nInches, Sixteeths );
         strReport.append("--->  ").append(QString::number(nFeet)).append(" ft  ").append(QString::number(nInches)).append(" in  ").append(QString::number(Sixteeths)).append(" Sixteenths")   ;
         strReport.append("\n");

    }
    strReport.append("\n").append("Rows").append("\n\n");
    for(int iii = 1; iii <= m_nNumberOfRows; iii++)
    {
        int nFeet, nInches, Sixteeths;
        strReport.append("Row: ").append(QString::number(iii)).append("\n");
        convertSixteenthsToFeetInchesSixteenths(m_sRow[iii].sStart.nLeangth, nFeet, nInches, Sixteeths );
        strReport.append("Start:  ").append(QString::number(nFeet)).append(" ft  ").append(QString::number(nInches)).append(" in  ").append(QString::number(Sixteeths)).append(" Sixteenths")   ;
        strReport.append("\n");
        strReport.append("Number of Complete Boards: ").append(QString::number(m_sRow[iii].nNumberOfCompleteBoards)).append("\n");
        convertSixteenthsToFeetInchesSixteenths(m_sRow[iii].sEnd.nLeangth, nFeet, nInches, Sixteeths );
        strReport.append("End:  ").append(QString::number(nFeet)).append(" ft  ").append(QString::number(nInches)).append(" in  ").append(QString::number(Sixteeths)).append(" Sixteenths")   ;
        strReport.append("\n\n");
    }

        return strReport;
}

void FloorSection::EnterSectionLength(int nSixteenthsOfAnInch)
{
        m_nSectionLength = nSixteenthsOfAnInch;
}

void FloorSection::EnterSectionWidth(int nSixteenthsOfAnInch)
{
        m_nSectionWidth = nSixteenthsOfAnInch;
}

void FloorSection::EnterBoardLength(int nSixteenthsOfAnInch)
{
        m_nBoardLength = nSixteenthsOfAnInch;
}

void FloorSection::EnterBoardWidth(int nSixteenthsOfAnInch)
{
        m_nBoardWidth = nSixteenthsOfAnInch;
}

int FloorSection::GetNumberOfRows()
{
    return m_nNumberOfRows;
}

int FloorSection::GetTotalNumberOfBoardsUsed()
{

}

QString FloorSection::GetCutList()
{
    CalcNumRows();
    CalcCutList();
    return GenerateCutList();
}

void FloorSection::CalcCutList()
{
    // if check start idex is greater than zero
    //  check all boards to see if they will work.
    //      if so enter it into current row start and calculate end
    // else
    //  check if End index > 0
    //      see if any starts for end would work.
    //         if enter into end and calculate start
    // else
    //  -> gerneate radom length loop unitil one fits


//    m_nStartBufferIndex = 0;
//    m_nEndBufferIndex = 0;
    bool bSolutionFound = false;

    for (int nRowIndex = 1; nRowIndex <= m_nNumberOfRows; nRowIndex++)
    {
        bSolutionFound = false;
        if(m_nStartBufferIndex > 0 && bSolutionFound == false)
        {
            for (int iii = 1; iii <= m_nStartBufferIndex; iii++)
            {
                if(OkStartLength(m_sStarsBuffer[iii].nLeangth,nRowIndex))
                {
                    m_sRow[nRowIndex].sStart = m_sStarsBuffer[iii];
                    for(int jjj = iii; jjj < m_nStartBufferIndex; jjj++) //removes selected startbuffer board
                    {
                        m_sStarsBuffer[jjj] = m_sStarsBuffer[jjj+1];

                    }
                    m_nStartBufferIndex--;
                    m_sRow[nRowIndex].sEnd.nLeangth = CalculateRemainderLength(m_sRow[nRowIndex].sStart.nLeangth,m_sRow[nRowIndex].nNumberOfCompleteBoards );
                    m_nCurrentCutNumber++;
                    m_sRow[nRowIndex].sEnd.nCutNumber = m_nCurrentCutNumber;

                    m_nStartBufferIndex++; //enter remainder into buffer
                    m_sStarsBuffer[m_nStartBufferIndex].nCutNumber = m_nCurrentCutNumber;
                    //int nSawBladeWidth = 2;
                    m_sStarsBuffer[m_nStartBufferIndex].nLeangth = (m_nBoardLength - (m_sRow[nRowIndex].sStart.nLeangth) - m_nSawBladeWidth );
                    bSolutionFound = true;
                    break;
                }
              }//end Start buffer search
        }// end if start index has values
        if(m_nEndBufferIndex > 0 && bSolutionFound == false)
        {
            for (int iii = 1; iii <= m_nEndBufferIndex; iii++)
            {
                int nTempNumOfBoards=0;
                int nTempStartLength = CalculateRemainderLength(m_sEndsBuffer[iii].nLeangth,nTempNumOfBoards );
                if(OkStartLength(nTempStartLength,nRowIndex))
                {
                    m_sRow[nRowIndex].sEnd = m_sEndsBuffer[iii];
                    for(int jjj = iii; jjj < m_nEndBufferIndex; jjj++) //removes selected End buffer board
                    {
                        m_sEndsBuffer[jjj] = m_sEndsBuffer[jjj+1];
                    }
                    m_nEndBufferIndex--;

                    m_sRow[nRowIndex].sStart.nLeangth = CalculateRemainderLength(m_sRow[nRowIndex].sEnd.nLeangth,m_sRow[nRowIndex].nNumberOfCompleteBoards );
                    m_nCurrentCutNumber++;
                    m_sRow[nRowIndex].sStart.nCutNumber = m_nCurrentCutNumber;

                    m_nEndBufferIndex++; //enter remainder into buffer
                    m_sEndsBuffer[m_nEndBufferIndex].nCutNumber = m_nCurrentCutNumber;
                    //int nSawBladeWidth = 2;
                    m_sEndsBuffer[m_nEndBufferIndex].nLeangth = (m_nBoardLength - (m_sRow[nRowIndex].sStart.nLeangth) - m_nSawBladeWidth );
                    bSolutionFound = true;
                    break;
                }
            }
        }//end search end buffer
        if (!(bSolutionFound)) //generate random start cut
        {
            int nBeginging = m_nMinimumLength / 16;
            int nEnding = m_nBoardLength / 16;
            int nTempStartlength=0;
            while((!(bSolutionFound)))
            {
                nTempStartlength = (randomInt(nBeginging,nEnding))*16; // <------------- need to multiply by 16
                bSolutionFound = OkStartLength(nTempStartlength,nRowIndex );
            }
            m_nCurrentCutNumber++;
            m_sRow[nRowIndex].sStart.nLeangth = nTempStartlength;
            m_sRow[nRowIndex].sStart.nCutNumber = m_nCurrentCutNumber;

            m_nEndBufferIndex++; // enter remainder into end buffer
            m_sEndsBuffer[m_nEndBufferIndex].nCutNumber = m_nCurrentCutNumber;
            m_sEndsBuffer[m_nEndBufferIndex].nLeangth = (m_nBoardLength - m_sRow[nRowIndex].sStart.nLeangth - m_nSawBladeWidth);

            m_sRow[nRowIndex].sEnd.nLeangth = CalculateRemainderLength(m_sRow[nRowIndex].sStart.nLeangth,m_sRow[nRowIndex].nNumberOfCompleteBoards );
            m_nCurrentCutNumber++;
            m_sRow[nRowIndex].sEnd.nCutNumber = m_nCurrentCutNumber;

            m_nStartBufferIndex++; //enter remainder into  Start buffer
            m_sStarsBuffer[m_nStartBufferIndex].nCutNumber = m_nCurrentCutNumber;

            m_sStarsBuffer[m_nStartBufferIndex].nLeangth = (m_nBoardLength - (m_sRow[nRowIndex].sEnd.nLeangth) - m_nSawBladeWidth );
            //bSolutionFound = true;
        }
    }

}
void FloorSection::inializeVariables()
{
     m_nCurrentCutNumber = 0;
     m_nStartBufferIndex = 0;
     m_nEndBufferIndex = 0;

//    board sTemp;
//    sTemp.nCutNumber = 0;
//    sTemp.nLeangth = 0;
//    sTemp.nLeangth = 0;

//    for(int iii = 0; iii < 255; iii++)
//    {
//         m_sStarsBuffer[iii] = sTemp;
//         m_sEndsBuffer[iii] = sTemp;
//         m_sRow[iii].nNumberOfCompleteBoards = 0;
//         m_sRow[iii].sEnd = sTemp;
//         m_sRow[iii].sStart = sTemp;
//    }

}
