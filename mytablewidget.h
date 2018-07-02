#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QTableWidget>
#include <QKeyEvent>

class myTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    myTableWidget(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *k);
signals:
    void keyPress(int key);
};

#endif // MYTABLEWIDGET_H
