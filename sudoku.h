#ifndef SUDOKU_H
#define SUDOKU_H


#include <iostream>
#include <QVector>



class Sudoku {

public:

    Sudoku();
    Sudoku(const int init_map[]);
    void setMap(const int set_map[]);
    int getElement(int index);
    int getElement(int row, int col);
    bool isCorrect();
    static const int sudokuSize = 81;
    void setElement(int index, int value);
    void setElement(int row, int col, int value);
    int getFirstZeroIndex();
    QVector<int> getAvailableNum(int index);

private:

    bool checkUnity(int arr[]);
    int map[sudokuSize];
};

#endif // SUDOKU_H
