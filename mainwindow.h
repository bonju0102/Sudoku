#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPushButton>
#include "sudoku.h"
#include <QVector>
#include <algorithm>
//#include <QGlobal.h>
#include <QTime>
#include <QTimer>
#include <QLabel>
#include <QMessageBox>
#include <QDialog>
#include <QInputDialog>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool create(Sudoku question, Sudoku & answer);
    bool solve(Sudoku question, Sudoku & answer);
    void setTable(Sudoku su);
    void getTable(Sudoku &su);
    int getLevel();
    QLabel *timeLabel;
    QLabel *nameLabel;

private slots:
    void mute();
    void keyPress(int key);
    void clearBoard();
    void newBoard();
    void solveBoard();
    void setLevel(int level);
    void toggleTimer();
    void showTime();
    void highLight(int row, int col);
    void toggleHint();
    void hint(int row, int col);
    void inputName();
    void restart();
    void block();

private:

    Ui::MainWindow *ui;
    QMediaPlayer player;
    QMediaPlaylist playlist;
    int level;
    QTimer *myTimer;
    QTime time2;
    QString name;

};

#endif // MAINWINDOW_H
