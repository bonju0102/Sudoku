#include "sudoku.h"
using namespace std;

#include <QDebug>

Sudoku::Sudoku() {
    for(int i=0; i<sudokuSize; ++i)
        map[i] = 0;
}

Sudoku::Sudoku(const int init_map[]) {
    for(int i=0; i<sudokuSize; ++i)
        map[i] = init_map[i];
}

void Sudoku::setMap(const int set_map[]) {
    for(int i=0; i<sudokuSize; ++i)
        map[i] = set_map[i];
}

int Sudoku::getElement(int index) {
    return map[index];
}

int Sudoku::getElement(int row, int col)
{
    return map[row*9+col];
}

bool Sudoku::checkUnity(int arr[]) {
    int arr_unity[9];           // counters

    for(int i=0; i<9; ++i)
        arr_unity[i] = 0;       // initialize

    for(int i=0; i<9; ++i)
        ++arr_unity[arr[i]-1];  // count

    for(int i=0; i<9; ++i)
        if(arr_unity[i] != 1)   // all element must be 1
            return false;

    return true;
}

bool Sudoku::isCorrect() {
    bool check_result;

    int check_arr[9];

    int location;
    for(int i=0; i<81; i+=9) {  // check rows
        for(int j=0; j<9; ++j)
            check_arr[j] = map[i+j];
        check_result = checkUnity(check_arr);
        if(check_result == false)
            return false;
    }

    for(int i=0; i<9; ++i) {    // check columns
        for(int j=0; j<9; ++j)
            check_arr[j] = map[i+9*j];
        check_result = checkUnity(check_arr);
        if(check_result == false)
            return false;
    }

    for(int i=0; i<9; ++i) {    // check cells
        for(int j=0; j<9; ++j) {
            location = 27*(i/3) + 3*(i%3) +9*(j/3) + (j%3);
            check_arr[j] = map[location];
        }
        check_result = checkUnity(check_arr);
        if(check_result == false)
            return false;
    }
    return true;
}

void Sudoku::setElement(int index, int value) {
    map[index] = value;
}

void Sudoku::setElement(int row, int col, int value)
{
    map[row*9+col] = value;
}

int Sudoku::getFirstZeroIndex() {
    for(int i=0;i<sudokuSize;++i)
        if(map[i] == 0)
            return i;
    return -1;

}

QVector<int> Sudoku::getAvailableNum(int index)
{
    QVector<int> ans;

    int row = index/9;
    int col = index%9;
    int num;
    bool array[9] = {0};


    //檢查行
    for (int i = 0; i < 9; ++i) {
        num = getElement(row,i);
        if(num){
            array[num-1]=true;
        }
    }

    //檢查列
    for (int i = 0; i < 9; ++i) {
        num = getElement(i,col);
        if(num){
            array[num-1]=true;  //num範圍為0~9
        }
    }

    //檢查方格
    row = row/3*3;  //row為0~2
    col = col/3*3;  //col為0~2
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            num = getElement(row+i,col+j);
            if (num) {
                array[num-1] = true;
            }
        }
    }

    //將可(沒)用到的數字存入vector
    for (int i = 0; i < 9; ++i){
        if(!array[i]){
            ans.push_back(i+1);
        }
    }
    return ans;
}

