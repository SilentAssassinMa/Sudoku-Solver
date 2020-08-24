#include "SudokuSolver.h"

using namespace std;

inline int FindElement(int *sudoku, int i, int j)
{
    return sudoku[9 * (i - 1) + (j - 1)];
}

SudokuSolver::SudokuSolver(const char *inputfilename)
{
    ifstream infile;
    infile.open(inputfilename, ios::in);

    for (int i = 0; i < 81; i++)
    {
        infile >> OriginalSudoku[i];
        if (OriginalSudoku[i] < 0 || OriginalSudoku[i] > 9)
        {
            cout << "Error : Input Sudoku Is Wrong !" << endl;
            isSolvable = false;
            return;
        }

        isChangeable[i] = OriginalSudoku[i] == 0 ? true : false;
        Current[i] = OriginalSudoku[i];
    }

    if (!CheckConsistency(OriginalSudoku))
    {
        cout << "Error : Input Sudoku Is Wrong !" << endl;
        isSolvable = false;
        return;
    }

    if (!CheckEmpty(OriginalSudoku))
    {
        cout << "Error : The Input Sudoku Has Already Been Fiiled Out" << endl;
        isSolvable = false;
        return;
    }

    infile.close();

    isSolvable = true;
}

SudokuSolver::~SudokuSolver()
{
}

void SudokuSolver::BackTracking()
{
    if (!isSolvable)
    {
        cout << "Error : The Input Sudoku Is Not Solvable" << endl;
        return;
    }

    int step = 0;
    while (CheckEmpty(Current))
    {
        int x, y;
        int guesstime = 0;
        vector<int> guess_number;
        int original_number;
        FindMaxConstrain(Current, x, y);

        original_number = FindElement(Current, x, y);

        for (int i = 1; i <= 9; i++)
        {
            Current[9 * (x - 1) + (y - 1)] = i;
            if (CheckConsistency(Current))
            {
                guesstime++;
                guess_number.push_back(i);
            }
            Current[9 * (x - 1) + (y - 1)] = 0;
        }

        if (guesstime != 0)
        {
            Current[9 * (x - 1) + (y - 1)] = guess_number.back();
            // Here I use 5 digits to store the change
            RecordChange.push_back(guesstime * 10000 + x * 1000 + y * 100 + original_number * 10 + guess_number.back());
            GuessNumber.push_back(guess_number);
        }
        else
        {
            int ChangeSize = RecordChange.size();
            for (int i = ChangeSize - 1; i >= 0; i--)
            {
                int xc, yc, origin, guess, time;
                // recover the last change
                xc = (RecordChange[i] / 1000) % 10;
                yc = (RecordChange[i] / 100) % 10;
                origin = (RecordChange[i] / 10) % 10;
                guess = RecordChange[i] % 10;
                time = RecordChange[i] / 10000;

                if (time == 1)
                {
                    // if it's the last guess, undo the change
                    Current[9 * (xc - 1) + (yc - 1)] = 0;
                    RecordChange.pop_back(); // delete the change
                    GuessNumber.pop_back();
                }
                else
                {
                    // if we have other possible value, change a guess number
                    GuessNumber.at(i).pop_back();
                    Current[9 * (xc - 1) + (yc - 1)] = GuessNumber.at(i).back();
                    time--;
                    RecordChange.pop_back();
                    RecordChange.push_back(time * 10000 + xc * 1000 + yc * 100 + guess * 10 + GuessNumber.at(i).back());
                    break;
                }
            }
        }

        step++;
    }

    cout << "The Original Sudoku is " << endl;
    PrintSudoku(OriginalSudoku);
    cout << endl;

    cout << "The Filled Sudoku is " << endl;
    PrintSudoku(Current);

    cout << endl;
    cout << "Total Steps : " << step << endl;
}

void SudokuSolver::SimulatedAnnealing(double temp, int steps)
{
    if (!isSolvable)
    {
        cout << "Error : The Input Sudoku Is Not Solvable" << endl;
        return;
    }

    /* 
    Here we use 243 - (sum of unique elements) as the energy of the sudoku,
    and we initialize it so that each subsquare containes all integers once only
    */
    cout << "The Original Sudoku is " << endl;
    PrintSudoku(OriginalSudoku);
    cout << endl;

    // Initialization
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            if ((i % 3) == 1 && (j % 3) == 1)
            {
                int subsquare[10] = {0};
                // Find the missing integers
                for (int m = 0; m < 3; m++)
                {
                    for (int n = 0; n < 3; n++)
                    {
                        if (FindElement(OriginalSudoku, i + m, j + n) != 0)
                        {
                            subsquare[FindElement(OriginalSudoku, i + m, j + n)] = 1;
                        }
                    }
                }

                // Fill the rest
                for (int m = 0; m < 3; m++)
                {
                    for (int n = 0; n < 3; n++)
                    {

                        if (isChangeable[9 * (i + m - 1) + j + n - 1])
                        {
                            for (int l = 1; l <= 9; l++)
                            {
                                if (subsquare[l] == 0)
                                {
                                    Current[9 * (i + m - 1) + j + n - 1] = l;
                                    subsquare[l] = 1;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    srand((unsigned)time(NULL));

    double Temperature = temp < 0 ? 0.25 : temp;
    int MaxSteps = steps <= 0 ? 1e6 : steps;

    int Ebefore = 243 - UniqueElements(Current);

    // start loop
    int step;
    for (step = 0; step < MaxSteps; step++)
    {
        // pick a random block and two random changeable elements

        int xblock = 3 * (rand() % 3) + 1;
        int yblock = 3 * (rand() % 3) + 1;
        int x1, x2, y1, y2;

        do
        {
            x1 = rand() % 3 + xblock;
            y1 = rand() % 3 + yblock;
            x2 = rand() % 3 + xblock;
            y2 = rand() % 3 + yblock;
        } while (!isChangeable[9 * (x1 - 1) + (y1 - 1)] || !isChangeable[9 * (x2 - 1) + (y2 - 1)] ||
                 (x1 == x2 && y1 == y2));

        // exchange the chosen elements
        int temp;
        temp = Current[9 * (x1 - 1) + (y1 - 1)];
        Current[9 * (x1 - 1) + (y1 - 1)] = Current[9 * (x2 - 1) + (y2 - 1)];
        Current[9 * (x2 - 1) + (y2 - 1)] = temp;

        int Eafter = 243 - UniqueElements(Current);

        if (Eafter < Ebefore)
        {
            // cout << step << " " << Eafter << endl;
        }

        if (Eafter == 0)
            break;

        if (exp((Ebefore - Eafter) / Temperature) > (rand() % 10000 / (double)10001))
        {
            Ebefore = Eafter;
        }
        else
        {
            temp = Current[9 * (x1 - 1) + (y1 - 1)];
            Current[9 * (x1 - 1) + (y1 - 1)] = Current[9 * (x2 - 1) + (y2 - 1)];
            Current[9 * (x2 - 1) + (y2 - 1)] = temp;
        }
    }

    if (CheckConsistency(Current))
    {
        cout << "The Filled Sudoku is " << endl;
        PrintSudoku(Current);
    }
    else
    {
        cout << "The Sudoku is not solved, try more steps or different temperature" << endl;
    }

    cout << endl;
    cout << "Total Steps : " << step << endl;
}

bool SudokuSolver::CheckConsistency(int *sudoku)
{
    // loop over every column and row and subsquare
    for (int i = 1; i <= 9; i++)
    {
        int rowsum[10] = {0};
        int columnsum[10] = {0};
        for (int j = 1; j <= 9; j++)
        {
            if (FindElement(sudoku, i, j) != 0)
            {
                columnsum[FindElement(sudoku, i, j)]++;
                if (columnsum[FindElement(sudoku, i, j)] > 1)
                    return false;
            }
            if (FindElement(sudoku, j, i) != 0)
            {
                rowsum[FindElement(sudoku, j, i)]++;
                if (rowsum[FindElement(sudoku, j, i)] > 1)
                    return false;
            }

            if ((i % 3) == 1 && (j % 3) == 1)
            {
                int subsquaresum[10] = {0};
                for (int m = 0; m < 3; m++)
                {
                    for (int n = 0; n < 3; n++)
                    {
                        if (FindElement(sudoku, i + m, j + n) != 0)
                        {
                            subsquaresum[FindElement(sudoku, i + m, j + n)]++;
                            if (subsquaresum[FindElement(sudoku, i + m, j + n)] > 1)
                                return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool SudokuSolver::CheckEmpty(int *sudoku)
{
    for (int i = 0; i < 81; i++)
    {
        if (sudoku[i] == 0)
        {
            return true;
        }
    }

    return false;
}

void SudokuSolver::FindMaxConstrain(int *sudoku, int &x, int &y)
{
    // This returns the empty element with maximum constrain
    int rowconstrain[10] = {0};
    int columnconstrain[10] = {0};
    int subsquareconstrain[3][3] = {0};
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            if (FindElement(sudoku, i, j) != 0)
            {
                rowconstrain[i]++;
                columnconstrain[j]++;
            }

            if ((i % 3) == 1 && (j % 3) == 1)
            {
                for (int m = 0; m < 3; m++)
                {
                    for (int n = 0; n < 3; n++)
                    {
                        if (FindElement(sudoku, i + m, j + n) != 0)
                            subsquareconstrain[i / 3][j / 3]++;
                    }
                }
            }
        }
    }
    int maxconstrain = -1;

    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            if (FindElement(sudoku, i, j) == 0)
            {
                int sumconstrain = rowconstrain[i] + columnconstrain[j] + subsquareconstrain[(i - 1) / 3][(j - 1) / 3];
                if (sumconstrain > maxconstrain)
                {
                    maxconstrain = sumconstrain;
                    x = i;
                    y = j;
                }
            }
        }
    }
}

int SudokuSolver::UniqueElements(int *sudoku)
{
    // This function returns the sum of unique elements in each row, column and subsquare
    int num = 0;
    for (int i = 1; i <= 9; i++)
    {
        int rowsum[10] = {0};
        int columnsum[10] = {0};
        for (int j = 1; j <= 9; j++)
        {
            if (FindElement(sudoku, i, j) != 0)
            {
                columnsum[FindElement(sudoku, i, j)]++;
            }
            if (FindElement(sudoku, j, i) != 0)
            {
                rowsum[FindElement(sudoku, j, i)]++;
            }

            if ((i % 3) == 1 && (j % 3) == 1)
            {
                int subsquaresum[10] = {0};
                for (int m = 0; m < 3; m++)
                {
                    for (int n = 0; n < 3; n++)
                    {
                        if (FindElement(sudoku, i + m, j + n) != 0)
                        {
                            subsquaresum[FindElement(sudoku, i + m, j + n)]++;
                        }
                    }
                }

                for (int m = 1; m <= 9; m++)
                {
                    if (subsquaresum[m] == 1)
                    {
                        num++;
                    }
                }
            }
        }

        for (int n = 1; n <= 9; n++)
        {
            if (rowsum[n] == 1)
            {
                num++;
            }
            if (columnsum[n] == 1)
            {
                num++;
            }
        }
    }

    return num;
}

void SudokuSolver::PrintSudoku(int *sudoku)
{
    for (int i = 1; i <= 9; i++)
    {
        if (i % 3 == 1)
        {
            cout << "+-----+-----+-----+" << endl;
        }
        cout << "|";
        for (int j = 1; j <= 9; j++)
        {
            if (j % 3 == 0)
            {
                cout << FindElement(sudoku, i, j) << "|";
            }
            else
            {
                cout << FindElement(sudoku, i, j) << " ";
            }
        }
        cout << endl;
    }
    cout << "+-----+-----+-----+" << endl;
}
