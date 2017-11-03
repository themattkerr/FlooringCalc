#ifndef VARIBLEDECLARATIONS_H
#define VARIBLEDECLARATIONS_H

enum boardType {
    START   = 0,
    END     = 1
};

struct board {
    int nLeangth;
    int nCutNumber;
    bool bRequires2ndCut;
};

struct s5Boards{
    int nNumberOfBoards;
    board sBrd[6];
};

struct row {
    board sStart;
    board sEnd;
    int nNumberOfCompleteBoards;
};
struct statistics {

    unsigned int nStairStepFail = 0;
    unsigned int nProportionFail = 0;
    unsigned int nMinLengthFail = 0;
    unsigned int nMaxWasteFail = 0;

};

#endif // VARIBLEDECLARATIONS_H
