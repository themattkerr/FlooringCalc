#ifndef MATTCALCULATIONS
#define MATTCALCULATIONS

#define VERSION_OF_MATTCALCULATIONS "1.2.3" //10/23/2017

#include <qstring.h>
#include <QDate>

enum curency{
    US_DOLLARS = 0,
};
enum dayOfWeek{
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THURSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6,
    SUNDAY = 7

};

bool doubleIsEqual (double x, double y, unsigned int nNumberOfDecimalPlaces);

double  usDollarsStringToDouble(QString qstrDollars);
QString doubleToCurrency (double dInput, unsigned int nCurrency);
QString millisecondsToHoursMinsSec (int nMilliseconds);

bool  isBusinessDay(QDate dtDayToTest, QString &strReason);
bool  isNationalHoliday(QDate dtDayToTest, QString &strHolidayName);
void  dayOfWeekStats(QDate dtDay, int &nDayOfWeek, int &nHowManyOfDayOfWeekInMonth, int &occuranceOfDayOfWeekInMonth);
QDate dateBusinessDaysAway(QDate dtInputDate, int nNumOfBusinessDays, QString &strListOfDaysOff);
int   numOfBusinessDaysBetween(QDate dtStartDate, QDate dtEndDate, QString &strListOfDaysOff);

bool convertSixteenthsToFeetInchesSixteenths(int inputSixteeths, int &nFeet, int &nInches, int &nSixteenths);
bool reduceSixteethsFractions(int inputSixteenths, int &nHalf, int &nQuarters, int &nEighths, int &nSixteens );
int convertFeetInchestSixteenthsToSixteenths(int nFeet, int nInches, int nSixteenths);
int randomInt (int nBegining, int nEnd);

//-----------------------------------------------------------------------------

double roundDoubleToPoints(double dInput, unsigned int nNumberOfDecimalPlaces );
QString splitString(QString CSVLine);

//QString addCommasToDouble(double dInput, unsigned int nNumberOfDecimalPlaces);
//QString addCommasToString (QString strInput);
//QString addDecimalPoint (QString strInput, unsigned int nNumberOfDecimalPlaces);
//QString reverseQString(QString strInput);
//void swapChar(QChar &cA, QChar &cB);





#endif // MATTCALCULATIONS

