CXX = g++
CXXFLAGS = -lm -O3
objects = SudokuSolver.o main.o

buildobj : $(objects) solve.exe

# build object file
SudokuSolver.o : SudokuSolver.cpp SudokuSolver.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

main.o : main.cpp SudokuSolver.cpp SudokuSolver.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

# build executable file 
solve.exe : main.o SudokuSolver.o
	$(CXX) $^ $(CXXFLAGS) -o $@ 

.PHONY : clean
clean :
	rm *.o
