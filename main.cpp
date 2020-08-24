#include "SudokuSolver.h"
#include <iostream>
#include <time.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << " Usage : " << argv[0] << " inputfilename" << std::endl;
        return 0;
    }

    clock_t time_start = clock();
    SudokuSolver solver1(argv[1]);
    SudokuSolver solver2(argv[1]);
    solver1.BackTracking();
    solver2.SimulatedAnnealing(0.1, 1e9);
    clock_t time_end = clock();
    std::cout << "Time use : " << double(time_end - time_start) / CLOCKS_PER_SEC << "s" << std::endl;

    return 1;
}
