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

struct row {
    board sStart;
    board sEnd;
    int nNumberOfCompleteBoards;
};

#endif // VARIBLEDECLARATIONS_H
