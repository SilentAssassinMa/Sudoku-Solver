Sudoku-Solver

This is a sudoku solver with two methods, the backtracking and simulated-annealing method.

Usage

You can simply download the files, and use GNU make, then use the command :

    ./solve.exe test.dat

You will see a result like

    The Original Sudoku is 
    +-----+-----+-----+    
    |0 0 5|3 0 0|0 0 0|    
    |8 0 0|0 0 0|0 2 0|    
    |0 7 0|0 1 0|5 0 0|    
    +-----+-----+-----+    
    |4 0 0|0 0 5|3 0 0|    
    |0 1 0|0 7 0|0 0 6|    
    |0 0 3|2 0 0|0 8 0|    
    +-----+-----+-----+    
    |0 6 0|5 0 0|0 0 9|    
    |0 0 4|0 0 0|0 3 0|    
    |0 0 0|0 0 9|7 0 0|    
    +-----+-----+-----+    
                           
    The Filled Sudoku is   
    +-----+-----+-----+    
    |1 4 5|3 2 7|6 9 8|    
    |8 3 9|6 5 4|1 2 7|    
    |6 7 2|9 1 8|5 4 3|    
    +-----+-----+-----+    
    |4 9 6|1 8 5|3 7 2|    
    |2 1 8|4 7 3|9 5 6|    
    |7 5 3|2 9 6|4 8 1|    
    +-----+-----+-----+    
    |3 6 7|5 4 2|8 1 9|    
    |9 8 4|7 6 1|2 3 5|    
    |5 2 1|8 3 9|7 6 4|    
    +-----+-----+-----+    
                           
    Total Steps : 12828    

In the SudokuSolver class, there are two member function, BackTracking and SimulatedAnnealing, providing two different algorithms of solving a sudoku.




