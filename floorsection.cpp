#include "floorsection.h"
#include <qmath.h>

FloorSection::FloorSection()
{

}


void FloorSection::CalcNumRows()
{
    if (m_nBoardWidth > 0 && m_nSectionWidth >0)
    {
        int nTempNumberOfNewRows = m_nSectionWidth/m_nBoardWidth;
        m_nNumberOfRows = m_nNumberOfPreviousRows + nTempNumberOfNewRows;
    }
    else
        m_nNumberOfRows = 0;
}
void FloorSection::RemoveRow(int nRowToRemove)
{
    if (nRowToRemove < 1 || nRowToRemove > m_nNumberOfRows)
        return;
    for(int iii = nRowToRemove; iii <= m_nNumberOfRows; iii++)
        m_sRow[iii]=m_sRow[iii+1];
    m_nNumberOfRows--;
}
void FloorSection::RemovePreviousRows(int nNumberOfPreviousRows)
{
    int nFirstRow = 1;
    for(int iii = 1; iii <= nNumberOfPreviousRows; iii++)
        RemoveRow(nFirstRow);
    m_nNumberOfPreviousRows = 0;
}

board FloorSection::TakeBoardFromBuffer(int nBufferLocation, int nBoardType)
{
    board sTempBoard;
    if(nBoardType == START)
    {
        sTempBoard = m_sStarsBuffer[nBufferLocation];
        RemoveBoardFromStartBuffer(nBufferLocation);
    }
    if(nBoardType == END)
    {
        sTempBoard = m_sEndsBuffer[nBufferLocation];
        RemoveBoardFromEndBuffer(nBufferLocation);
    }
    return sTempBoard;
}

void FloorSection::RemoveBoardFromStartBuffer(int nBoardToRemove)
{
    if (nBoardToRemove == 254)
    {
        m_sStarsBuffer[nBoardToRemove].nCutNumber = 0;
        m_sStarsBuffer[nBoardToRemove].nLeangth = 0;
        m_sStarsBuffer[nBoardToRemove].bRequires2ndCut = false;
    }
    for(int jjj = nBoardToRemove; jjj < m_nStartBufferIndex; jjj++)
    {
        m_sStarsBuffer[jjj] = m_sStarsBuffer[jjj+1];
    }
    m_nStartBufferIndex--;
}

void FloorSection::RemoveBoardFromEndBuffer(int nBoardToRemove)
{
    if (nBoardToRemove == 254)
    {
        m_sEndsBuffer[nBoardToRemove].nCutNumber = 0;
        m_sEndsBuffer[nBoardToRemove].nLeangth = 0;
        m_sEndsBuffer[nBoardToRemove].bRequires2ndCut = false;
    }
    for(int jjj = nBoardToRemove; jjj < m_nEndBufferIndex; jjj++)
    {
        m_sEndsBuffer[jjj] = m_sEndsBuffer[jjj+1];
    }
    m_nEndBufferIndex--;
}

board FloorSection::FindBoardCloseToLength(int nLengthToCheck, int nBoardType, bool &ok)
{
    board sTempBoard;
    sTempBoard.bRequires2ndCut = false;
    sTempBoard.nCutNumber = -1;
    sTempBoard.nLeangth = -1;

    if (nBoardType == START)
    {
        for (int iii = 1; iii <= m_nStartBufferIndex;iii ++)
        {
            int nDifference = m_sStarsBuffer[iii].nLeangth - nLengthToCheck;
            if(m_nMaxWastePerBoard >= nDifference && 0 <= nDifference )
            {
                sTempBoard = TakeBoardFromBuffer(iii, nBoardType);
                sTempBoard.bRequires2ndCut = true;
                sTempBoard.nLeangth = nLengthToCheck;
                ok=true;
                return sTempBoard;
            }
        }
    }

    if(nBoardType == END)
    {
        for(int III = 1; III <= m_nEndBufferIndex;III++)
        {
            int nDifference = m_sEndsBuffer[III].nLeangth - nLengthToCheck;
            if(m_nMaxWastePerBoard >= nDifference && 0 <= nDifference )
            {
                sTempBoard = TakeBoardFromBuffer(III, nBoardType);
                sTempBoard.bRequires2ndCut = true;
                sTempBoard.nLeangth = nLengthToCheck;
                ok=true;
                return sTempBoard;
            }
        }
    }
    ok=false;
    return sTempBoard;
}



bool FloorSection::OkStartLength(int nLengthToCheck, int nRowNum)
{
    int nNumberOfFullBoards = 0;
    if (nLengthToCheck < 0)
        return false;
    if (nLengthToCheck < m_nMinimumLength || nLengthToCheck > m_nBoardLength )
    {
        sFailLog.nMinLengthFail++;
        return false;
    }
    int nTemp = CalculateRemainderLength(nLengthToCheck,nNumberOfFullBoards );
    if (nTemp < m_nMinimumLength || nTemp > m_nBoardLength)
    {
        sFailLog.nMinLengthFail++;
        return false;
    }

    //Check for variation
    int nVariableOverlap = m_nOverlapMin;
    for (int nRowCheck = nRowNum - 1, nRowsToCheck = m_nNumOfRowsToCheck; nRowCheck > 0 && nRowsToCheck > 0; nRowCheck--, nRowsToCheck--)
    {
        int nDifference = qFabs(m_sRow[nRowCheck].sStart.nLeangth - nLengthToCheck);
        if(nDifference < nVariableOverlap)
        {
            sFailLog.nProportionFail++;
            return false;
        }
        nVariableOverlap = nVariableOverlap / 4 ;// Reduces the amound of variation allowed the further away you get.
    }

    //Check for stairstepping
    if ((m_nNumOfRowsToCheck >= 3) && (nRowNum >= 3) ) // Checks Immediate neighbors
    {
        int nTempDifference1 = qFabs(m_sRow[(nRowNum-1)].sStart.nLeangth - nLengthToCheck);
        int nTempDifference2 = qFabs(m_sRow[(nRowNum-2)].sStart.nLeangth - m_sRow[(nRowNum-1)].sStart.nLeangth);
        int nImmediateNeighborDifference = qFabs(nTempDifference1 - nTempDifference2);

        if(nImmediateNeighborDifference < m_nStairStepRange)
        {
            sFailLog.nStairStepFail++;
            return false;
        }
    }
    if ((m_nNumOfRowsToCheck >= 5) && (nRowNum >= 5) ) // Checks Every other Row
    {
        int nTempDifference1 = qFabs(m_sRow[(nRowNum-2)].sStart.nLeangth - nLengthToCheck);
        int nTempDifference2 = qFabs(m_sRow[(nRowNum-4)].sStart.nLeangth - m_sRow[(nRowNum-2)].sStart.nLeangth);
        int nEveryOtherDifference = qFabs(nTempDifference1 - nTempDifference2);

        if(nEveryOtherDifference < m_nStairStepRange)
        {
            sFailLog.nStairStepFail++;
            return false;
        }
    }
    return true;
}
int FloorSection::CalculateRemainderLength(int nBoardLength, int &nNumOfFullBoards)
{
    nNumOfFullBoards =  (m_nSectionLength- (2*m_nExpansionGap) - nBoardLength) / m_nBoardLength;
    int nRemainderLength = m_nSectionLength - nBoardLength - (nNumOfFullBoards*m_nBoardLength) - (2*m_nExpansionGap);
    return nRemainderLength;
}
int FloorSection::FindWorkingLength(int nRowNum)
{
    int nNumberOfSixteenthsPerInch = 16;
    int OkLengths[255];
    int OkLengthsIndex = 0;
    for (int iii = 0;iii < 255; iii++)
        OkLengths[iii] = 0;

    for (int nTestLength = 0; nTestLength < m_nBoardLength; nTestLength = nTestLength + nNumberOfSixteenthsPerInch)
    {
        if(OkStartLength(nTestLength, nRowNum))
        {
            OkLengths[OkLengthsIndex] = nTestLength;
            OkLengthsIndex++;
        }
    }
    return OkLengths[randomInt(0,OkLengthsIndex)];
}

QString FloorSection::GenerateCutList()
{
    bool bAllRowsDone = false;
    QString strReport;
    QString str2ndCutIndicator = " <--- 2nd Cut";
    int nCutIndex = 1;
    for (int nCutNum = 1;nCutNum <= m_nCurrentCutNumber; nCutNum++)
    {
        strReport.append("Cut ").append(QString::number( (nCutNum)).append("\n"));
        for(int nTRow = (m_nNumberOfPreviousRows+1); nTRow <= m_nNumberOfRows; nTRow++ )
        {
            if(m_sRow[nTRow].sStart.nCutNumber == nCutNum )
            {
                int nFeet, nInches, Sixteeths;
                strReport.append("Row: (").append(intToLetters(nTRow)).append(")  ").append("\t");

                bool ok;
                QString tempFeetInches = createFeetInchesString(m_sRow[nTRow].sStart.nLeangth, m_nFeetInchesFormat, ok);
                strReport.append("Start: ").append(tempFeetInches);

                if(m_sRow[nTRow].sStart.bRequires2ndCut)
                    strReport.append(str2ndCutIndicator);
                strReport.append("\n");
            }
        }
        for(int nTRow = (m_nNumberOfPreviousRows+1); nTRow <= m_nNumberOfRows; nTRow++ )
        {
            if(m_sRow[nTRow].sEnd.nCutNumber == nCutNum )
            {
                int nFeet, nInches, Sixteeths;
                strReport.append("Row: (").append(intToLetters(nTRow)).append(")  ").append("\t");

                bool ok;
                QString tempFeetInches = createFeetInchesString(m_sRow[nTRow].sEnd.nLeangth, m_nFeetInchesFormat ,ok);
                strReport.append("End: ").append(tempFeetInches);
                if(m_sRow[nTRow].sEnd.bRequires2ndCut)
                    strReport.append(str2ndCutIndicator);
                strReport.append("\n");
            }
        }
        strReport.append("\n");
    }

    strReport.append("Extra Starts: ").append("\n");
    for (int iii = 1; iii <= m_nStartBufferIndex; iii++)
    {
         bool ok;
         QString tempFeetInches = createFeetInchesString(m_sStarsBuffer[iii].nLeangth, m_nFeetInchesFormat, ok);
         strReport.append("--->  ").append(tempFeetInches);
         strReport.append("   ").append("Cut: ").append(QString::number(m_sStarsBuffer[iii].nCutNumber ));
         strReport.append("\n");
    }
    strReport.append("Extra Ends: ").append("\n");
    for (int iii = 1; iii <= m_nEndBufferIndex; iii++)
    {
         bool ok;
         QString tempFeetInches = createFeetInchesString(m_sEndsBuffer[iii].nLeangth, m_nFeetInchesFormat, ok );
         strReport.append("--->  ").append(tempFeetInches);
         strReport.append("   ").append("Cut: ").append(QString::number(m_sEndsBuffer[iii].nCutNumber ));
         strReport.append("\n");
    }

    strReport.append("\n").append("Rows").append("\n\n");
    for(int iii = (m_nNumberOfPreviousRows+1); iii <= m_nNumberOfRows; iii++)
    {
        bool ok;
        QString tempFeetInches;
        strReport.append("Row: ").append(intToLetters(iii)).append("\n");
        tempFeetInches = createFeetInchesString(m_sRow[iii].sStart.nLeangth, m_nFeetInchesFormat, ok );

        strReport.append("Start:  ").append(tempFeetInches);
        strReport.append("   Cut: ").append(QString::number(m_sRow[iii].sStart.nCutNumber   ));
        strReport.append("\n");

        strReport.append("Number of Complete Boards: ").append(QString::number(m_sRow[iii].nNumberOfCompleteBoards)).append("\n");

        tempFeetInches = createFeetInchesString(m_sRow[iii].sEnd.nLeangth, m_nFeetInchesFormat, ok );
        strReport.append("End:  ").append(tempFeetInches);
        strReport.append("   Cut: ").append(QString::number(m_sRow[iii].sEnd.nCutNumber   ));

        int temp = m_sRow[iii].sStart.nLeangth +(m_sRow[iii].nNumberOfCompleteBoards * m_nBoardLength ) + m_sRow[iii].sEnd.nLeangth;
        tempFeetInches = createFeetInchesString(temp, m_nFeetInchesFormat, ok );
        strReport.append("\n").append("Total Row Length:  ").append(tempFeetInches);


        strReport.append("\n\n");
    }

    strReport.append("Fail Statistics:").append("\n");
    strReport.append("Min Length Fails: ").append( addCommasToDouble( sFailLog.nMinLengthFail,0)).append("\n");
    strReport.append("Max Waste Fails: ").append(addCommasToDouble( sFailLog.nMaxWasteFail,0 )).append("\n");
    strReport.append("Proportional Fails: ").append(addCommasToDouble( sFailLog.nProportionFail,0 )).append("\n");
    strReport.append("Stairstep Fails: ").append(addCommasToDouble( sFailLog.nStairStepFail, 0 )).append("\n");


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

void FloorSection::EnterPreviousRows(int nInputStartLength)
{
    m_nNumberOfPreviousRows++;
    board sTempBoard;
    sTempBoard.nLeangth = nInputStartLength;
    sTempBoard.nCutNumber = 0;

}
void FloorSection::EnterLast5Starts(s5Boards sLast5Starts)
{
    m_nNumberOfPreviousRows = sLast5Starts.nNumberOfBoards;
    for (int iii = 1; iii <= sLast5Starts.nNumberOfBoards; iii++)
    {
        m_sRow[iii].sStart = sLast5Starts.sBrd[iii];
    }
}
void FloorSection::EnterMaxWaste(int nMaxWasteInches)
{
    m_nMaxWastePerBoard = nMaxWasteInches * 16;
}
int FloorSection::GetMaxWasteInches()
{
    return m_nMaxWastePerBoard / 16;
}

int FloorSection::GetNumberOfRows()
{
    return (m_nNumberOfRows - m_nNumberOfPreviousRows);
}

int FloorSection::GetTotalNumberOfBoardsUsed()
{

}

QString FloorSection::GetCutList()
{
    CalcMinOverlap();
    CalcNumRows();
    CalcCutList();
    RemovePreviousRows(m_nNumberOfPreviousRows);
    return GenerateCutList();
}
void FloorSection::CalcMinOverlap()
{
    m_nOverlapMin = m_nBoardLength*.25;
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
    bool bUseStartsFirst = false;
    bool bAllStartsChecked = false;
    bool bAllEndsChecked = false;
    if (m_nStartBufferIndex > m_nEndBufferIndex)
        bUseStartsFirst = true;

    for (int nRowIndex = (m_nNumberOfPreviousRows+1); nRowIndex <= m_nNumberOfRows; nRowIndex++)
    {
        bSolutionFound = false;

        bUseStartsFirst = false;
        bAllStartsChecked = false;
        bAllEndsChecked = false;
        if (m_nStartBufferIndex > m_nEndBufferIndex)
            bUseStartsFirst = true;


        while (!bAllStartsChecked && !bAllEndsChecked ){

        if(m_nStartBufferIndex > 0 && bSolutionFound == false && bUseStartsFirst == true)
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


                    int nTempEndLength = CalculateRemainderLength(m_sRow[nRowIndex].sStart.nLeangth,m_sRow[nRowIndex].nNumberOfCompleteBoards );
                    bool ok = false;
                    board sTempEndBoard = FindBoardCloseToLength(nTempEndLength,END, ok);
                    if(ok)
                    {
                        m_sRow[nRowIndex].sEnd = sTempEndBoard;
                    }


                    if(!(ok))//create new cut
                    {
                        m_sRow[nRowIndex].sEnd.nLeangth = nTempEndLength;   //CalculateRemainderLength(m_sRow[nRowIndex].sStart.nLeangth,m_sRow[nRowIndex].nNumberOfCompleteBoards );
                        m_nCurrentCutNumber++;
                        m_sRow[nRowIndex].sEnd.nCutNumber = m_nCurrentCutNumber;

                        int nTempStart = (m_nBoardLength - (m_sRow[nRowIndex].sEnd.nLeangth) - m_nSawBladeWidth );
                        if(nTempStart >= m_nMinimumLength)
                        {
                            m_nStartBufferIndex++; //enter remainder into buffer
                            m_sStarsBuffer[m_nStartBufferIndex].nCutNumber = m_nCurrentCutNumber;
                            m_sStarsBuffer[m_nStartBufferIndex].nLeangth = nTempStart;
                        }
                    }

                    bSolutionFound = true;
                    break;
                }
              }//end Start buffer search
            bUseStartsFirst = false;
            bAllStartsChecked = true;
        }// end if start index has values
        if(m_nStartBufferIndex <= 0 || bSolutionFound)
            bAllStartsChecked = true;

        if(m_nEndBufferIndex > 0 && bSolutionFound == false && bUseStartsFirst == false)
        {
            for (int iii = 1; iii <= m_nEndBufferIndex; iii++)
            {
                int nTempNumOfBoards=0;
                int nTempStartLength = CalculateRemainderLength(m_sEndsBuffer[iii].nLeangth,nTempNumOfBoards );
                if(OkStartLength(nTempStartLength,nRowIndex))
                {
                    m_sRow[nRowIndex].nNumberOfCompleteBoards = nTempNumOfBoards;
                    m_sRow[nRowIndex].sEnd = m_sEndsBuffer[iii];

                    for(int jjj = iii; jjj < m_nEndBufferIndex; jjj++) //removes selected End buffer board
                    {
                        m_sEndsBuffer[jjj] = m_sEndsBuffer[jjj+1];
                    }
                    m_nEndBufferIndex--;

                    bool ok = false;
                    board sTempStartBoard = FindBoardCloseToLength(nTempStartLength,START,ok);
                    if(ok)
                    {
                        m_sRow[nRowIndex].sStart = sTempStartBoard;
                    }

                    else
                    {
                        m_sRow[nRowIndex].sStart.nLeangth = nTempStartLength;//CalculateRemainderLength(m_sRow[nRowIndex].sEnd.nLeangth,m_sRow[nRowIndex].nNumberOfCompleteBoards );
                        m_nCurrentCutNumber++;
                        m_sRow[nRowIndex].sStart.nCutNumber = m_nCurrentCutNumber;

                        int nTempEnd = (m_nBoardLength - (m_sRow[nRowIndex].sStart.nLeangth) - m_nSawBladeWidth );
                        if (nTempEnd >= m_nMinimumLength )
                        {
                            m_nEndBufferIndex++; //enter remainder into buffer
                            m_sEndsBuffer[m_nEndBufferIndex].nCutNumber = m_nCurrentCutNumber;
                            m_sEndsBuffer[m_nEndBufferIndex].nLeangth = nTempEnd;
                        }
                    }
                    bSolutionFound = true;
                    break;
                }
            }
            bAllEndsChecked = true;
        }//end search end buffer
        if(m_nEndBufferIndex <= 0 || bSolutionFound )
            bAllEndsChecked = true;
        } // end While loop.
        if (!(bSolutionFound)) //generate random start cut
        {
//            int nBeginging = m_nMinimumLength / 16;
//            int nEnding = m_nBoardLength / 16;
            int nTempStartlength=0;
            while((!(bSolutionFound)))
            {
                //nTempStartlength = (randomInt(nBeginging,nEnding))*16; // <------------- need to multiply by 16
                nTempStartlength = FindWorkingLength(nRowIndex);
                bSolutionFound = OkStartLength(nTempStartlength,nRowIndex );
                // ====================== This should mean less waste
                int nTempEnd = (m_nBoardLength - nTempStartlength - m_nSawBladeWidth);
                if (nTempEnd < m_nMinimumLength && nTempEnd >= m_nMaxWastePerBoard)
                    bSolutionFound = false;
            }
            m_nCurrentCutNumber++;
            m_sRow[nRowIndex].sStart.nLeangth = nTempStartlength;
            m_sRow[nRowIndex].sStart.nCutNumber = m_nCurrentCutNumber;

            int nTempEnd = (m_nBoardLength - m_sRow[nRowIndex].sStart.nLeangth - m_nSawBladeWidth);
            if (nTempEnd > m_nMinimumLength)
            {
                m_nEndBufferIndex++; // enter remainder into end buffer
                m_sEndsBuffer[m_nEndBufferIndex].nCutNumber = m_nCurrentCutNumber;
                m_sEndsBuffer[m_nEndBufferIndex].nLeangth = nTempEnd;
            }

            bool ok = false;
            int nCalculatedEndLength = CalculateRemainderLength(m_sRow[nRowIndex].sStart.nLeangth,m_sRow[nRowIndex].nNumberOfCompleteBoards );
            board sTempEndBoard = FindBoardCloseToLength(nCalculatedEndLength,END,ok);
            if(ok)
            {
                m_sRow[nRowIndex].sEnd = sTempEndBoard;
            }
            if(!ok) //Generate new cut for end
            {
                m_nCurrentCutNumber++;
                m_sRow[nRowIndex].sEnd.nLeangth = nCalculatedEndLength; //CalculateRemainderLength(m_sRow[nRowIndex].sStart.nLeangth,m_sRow[nRowIndex].nNumberOfCompleteBoards );
                m_sRow[nRowIndex].sEnd.nCutNumber = m_nCurrentCutNumber;

                int nTempStart = (m_nBoardLength - (m_sRow[nRowIndex].sEnd.nLeangth) - m_nSawBladeWidth );
                if (nTempStart > m_nMinimumLength)
                {
                    m_nStartBufferIndex++; //enter remainder into  Start buffer
                    m_sStarsBuffer[m_nStartBufferIndex].nCutNumber = m_nCurrentCutNumber;
                    m_sStarsBuffer[m_nStartBufferIndex].nLeangth = nTempStart;
                }
            }//end generate new end cut

        }
    }

}
void FloorSection::inializeVariables()
{

    sFailLog.nMaxWasteFail = 0;
    sFailLog.nMinLengthFail = 0;
    sFailLog.nProportionFail = 0;
    sFailLog.nStairStepFail = 0;

    m_nCurrentCutNumber = 0;
    m_nStartBufferIndex = 0;
    m_nEndBufferIndex = 0;

    board sTemp;
    sTemp.nCutNumber = 0;
    sTemp.nLeangth = 0;
    sTemp.bRequires2ndCut = false;

    for(int iii = 0; iii < 254; iii++)
    {
         m_sStarsBuffer[iii] = sTemp;
         m_sEndsBuffer[iii] = sTemp;

         m_sRow[iii].nNumberOfCompleteBoards = 0;

         m_sRow[iii].sStart = sTemp;
         m_sRow[iii].sEnd = sTemp;

    }

//    m_nStartBufferIndex = 6;


//    m_sStarsBuffer[1].nCutNumber = 1011 ;
//    m_sStarsBuffer[1].nLeangth = convertFeetInchestSixteenthsToSixteenths(3,9,0 );
//    m_sStarsBuffer[2].nCutNumber = 1027 ;
//    m_sStarsBuffer[2].nLeangth = convertFeetInchestSixteenthsToSixteenths(3,8,8);
//    m_sStarsBuffer[3].nCutNumber = 1030 ;
//    m_sStarsBuffer[3].nLeangth = convertFeetInchestSixteenthsToSixteenths(3,5,8);
//    m_sStarsBuffer[4].nCutNumber = 1034 ;
//    m_sStarsBuffer[4].nLeangth = convertFeetInchestSixteenthsToSixteenths(3,1,8);

//    m_sStarsBuffer[5].nCutNumber = 1003 ;
//    m_sStarsBuffer[5].nLeangth = convertFeetInchestSixteenthsToSixteenths(3,8,6);
//    m_sStarsBuffer[6].nCutNumber = 3014 ;
//    m_sStarsBuffer[6].nLeangth = convertFeetInchestSixteenthsToSixteenths(5,0,0);

//    m_nEndBufferIndex = 7;

//    m_sEndsBuffer[1].nCutNumber = 2006 ;
//    m_sEndsBuffer[1].nLeangth = convertFeetInchestSixteenthsToSixteenths(0,7,11 ) ;

//    m_sEndsBuffer[2].nCutNumber = 1024;
//    m_sEndsBuffer[2].nLeangth = convertFeetInchestSixteenthsToSixteenths(3,8,11 ) ;
//    m_sEndsBuffer[3].nCutNumber = 1036 ;
//    m_sEndsBuffer[3].nLeangth = convertFeetInchestSixteenthsToSixteenths(3,8,14 ) ;
//    m_sEndsBuffer[4].nCutNumber = 1041 ;
//    m_sEndsBuffer[4].nLeangth = convertFeetInchestSixteenthsToSixteenths(3,1,11 ) ;

//    m_sEndsBuffer[5].nCutNumber = 2009 ;
//    m_sEndsBuffer[5].nLeangth = convertFeetInchestSixteenthsToSixteenths(4,0,11) ;
//    m_sEndsBuffer[6].nCutNumber = 2011 ;
//    m_sEndsBuffer[6].nLeangth = convertFeetInchestSixteenthsToSixteenths(5,3,5 ) ;
//    m_sEndsBuffer[7].nCutNumber = 1013 ;
//    m_sEndsBuffer[7].nLeangth = convertFeetInchestSixteenthsToSixteenths(3, 10, 4 ) ;

//    m_nNumberOfPreviousRows = 5;
//    m_sRow[1].sStart.nLeangth = (convertFeetInchestSixteenthsToSixteenths( 5,4 ,0 ));
//    m_sRow[2].sStart.nLeangth = (convertFeetInchestSixteenthsToSixteenths( 1, 7,8 ));
//    m_sRow[3].sStart.nLeangth = (convertFeetInchestSixteenthsToSixteenths( 4, 8,0 ));
//    m_sRow[4].sStart.nLeangth = (convertFeetInchestSixteenthsToSixteenths( 2, 8,12 ));
//    m_sRow[5].sStart.nLeangth = (convertFeetInchestSixteenthsToSixteenths( 5,5 ,8 ));


}

QString FloorSection::GetPreview()
{
    QString strPreview;
    for (int nRow = 1; nRow <= m_nNumberOfRows; nRow++)
    {
        strPreview.append("Row ");
        if(nRow < 27)
            strPreview.append(" ");
        strPreview.append(intToLetters(nRow) ).append(": ");

        strPreview.append("|").append(DrawBoard(m_sRow[nRow].sStart.nLeangth));
        for (int nFullBoard = 1; nFullBoard <= m_sRow[nRow].nNumberOfCompleteBoards; nFullBoard++ )
        {
            strPreview.append(DrawBoard(m_nBoardLength));
        }
        strPreview.append(DrawBoard(m_sRow[nRow].sEnd.nLeangth));
        strPreview.append("|").append("\n");
    }
    return strPreview;

}
QString FloorSection::DrawBoard(int nLengthInicater)
{
    QString strPreview;
    //strPreview.append("|");
    for (int iii = 1;iii < static_cast<int>( roundDoubleToPoints((static_cast<double>(nLengthInicater)/16),0));iii++)
        strPreview.append("_");
    strPreview.append("|");
    return strPreview;
}
