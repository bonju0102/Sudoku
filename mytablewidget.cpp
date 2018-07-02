#include "mytablewidget.h"
#include <QDebug>


myTableWidget::myTableWidget(QWidget *parent) : QTableWidget(parent)
{

}

void myTableWidget::keyPressEvent(QKeyEvent *k)
{
    emit keyPress(k->key());
}
