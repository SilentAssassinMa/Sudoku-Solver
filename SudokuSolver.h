/*
This is a sudoku solver based on two algorithms :
1. Backtracking
2. Simulated Annealing

Auther : Wenhao Ma
Last Change Date : 2020.8.24
*/
#ifndef SUDOKUSOLVER_HEARDER
#define SUDOKUSOLVER_HEARDER

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <time.h>

inline int FindElement(int *sudoku, int i, int j);

class SudokuSolver
{
public:
    SudokuSolver(const char *inputfilename);
    virtual ~SudokuSolver();
    virtual void BackTracking();
    virtual void SimulatedAnnealing(double temp, int steps);

private:
    int OriginalSudoku[81] = {0};
    int Current[81] = {0};
    bool isChangeable[81] = {0};
    bool isSolvable;

    std::vector<int> RecordChange;
    std::vector<std::vector<int>> GuessNumber;

    virtual bool CheckConsistency(int *sudoku);
    virtual bool CheckEmpty(int *sudoku);
    virtual void FindMaxConstrain(int *sudoku, int &x, int &y);
    virtual int UniqueElements(int *sudoku);
    virtual void PrintSudoku(int *sudoku);
};

#endif