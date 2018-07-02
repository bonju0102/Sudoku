#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    nameLabel = new QLabel;
    inputName();

    myTimer = new QTimer(this);
    connect(myTimer, SIGNAL(timeout()), this, SLOT(showTime()));

    // add time label & icon
    timeLabel = new QLabel(this);
    QLabel *iconLabel = new QLabel;
    QPixmap pixmap(QPixmap(":/images/resources/stopwatch.png").scaledToHeight(statusBar()->height()/2));
    iconLabel->setPixmap(pixmap);
    ui->statusBar->addPermanentWidget(iconLabel);
    ui->statusBar->addPermanentWidget(timeLabel);

    // set playlist & player
    playlist.addMedia(QUrl("qrc:/bgm/resources/The Spectre.mp3")); //加入播放清單
    playlist.setPlaybackMode(QMediaPlaylist::Loop); //循環播放

    player.setPlaylist(&playlist);
    player.setVolume(70);
    player.play();

    // muteBox
    ui->muteBox->setStyleSheet("color: rgb(238,233,209);");
    connect(ui->muteBox, SIGNAL(toggled(bool)), this, SLOT(mute()));

    // hintBox & hintLabel
    ui->hintBox->setStyleSheet("color: rgb(238,233,209);");
    ui->hintLabel->setStyleSheet("color: rgb(238,233,209);");
    ui->hintLabel->hide();
    connect(ui->hintBox, SIGNAL(toggled(bool)), this, SLOT(toggleHint()));

    connect(ui->tableWidget, SIGNAL(keyPress(int)), this, SLOT(keyPress(int)));
    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(highLight(int,int)));
    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(hint(int,int)));
    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(block()));

    connect(ui->levelWidget, SIGNAL(currentRowChanged(int)), this, SLOT(setLevel(int)));

    connect(ui->timeButton, SIGNAL(clicked(bool)), this, SLOT(toggleTimer()));
    connect(ui->clearButton, SIGNAL(clicked(bool)), this, SLOT(clearBoard()));
    connect(ui->newButton, SIGNAL(clicked(bool)), this, SLOT(newBoard()));
    connect(ui->solveButton, SIGNAL(clicked(bool)), this, SLOT(solveBoard()));


//   int map[81] = {8,6,5,3,2,9,4,1,7,2,4,3,1,7,5,8,6,9,1,9,7,6,8,4,5,2,3,3,1,9,2,5,8,6,7,4,4,2,6,7,9,1,3,5,8,5,7,8,4,3,6,1,9,2,7,5,4,9,1,3,2,8,6,6,8,2,5,4,7,9,3,1,9,3,1,8,6,2,7,4,5};
    int map[81] = {0};
    Sudoku ques(map);
    Sudoku ans;

    setLevel(1);
//    if (create(ques, ans) == true) {
//        qDebug() << "Solvable!\n";
//        for(int i=0; i<81; i++) {
//            qDebug() << ans.getElement(i) << " ";
//            if(i%9==8)
//                qDebug() << "\n";
//        }
//    } else
//        qDebug() << " Unsolvable!!\n";

     // 產生QTableWidget
     QTableWidget *myTableWidget = ui->tableWidget;
     myTableWidget->setRowCount(9);     //設定QTableWidget列數
     myTableWidget->setColumnCount(9);  //設定QTableWidget行數

     myTableWidget->horizontalHeader()->hide();
     myTableWidget->verticalHeader()->hide();
     myTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //平均分割QTableWidget水平寬度
     myTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);   //平均分割QTableWidget垂直寬度
     myTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);           //同時只能選取一個item

     // 產生QListWidget
     QListWidget *myListWidget = ui->levelWidget;
     myListWidget->insertItem(0, new QListWidgetItem("Easy"));
     myListWidget->insertItem(1, new QListWidgetItem("Normal (Default)"));
     myListWidget->insertItem(2, new QListWidgetItem("Hard"));


     newBoard();

}
void MainWindow::setTable(Sudoku su){
    for (int i = 0; i < 81; ++i) {
       QTableWidgetItem *item = new QTableWidgetItem(QString::number(su.getElement(i)));
       if (item->text() == "0")
           item->setText("");
       item->setTextAlignment(Qt::AlignCenter);    //置中
       item->setFlags(item->flags() ^ Qt::ItemIsEditable); //設定不可編輯
       // 設定方格顏色
       int c=i%9/3;
       int r=i/9/3;
       int m=c+r*3;
       if(m%2==0)
           item->setBackgroundColor(QColor(114,167,163));
       else
           item->setBackgroundColor(QColor(238,233,209));
       ui->tableWidget->setItem(i/9, i%9, item);
    }
}

void MainWindow::getTable(Sudoku &su)
{
    QString str;
    for (int i = 0; i < 81; ++i) {
        str = ui->tableWidget->item(i/9,i%9)->text();
        su.setElement(i,str.toInt());
    }
}

void MainWindow::setLevel(int level)
{
    switch (level) {
        case 0: this->level = 25;
                break;
        case 1: this->level = 40;
                break;
        case 2: this->level = 55;
                break;
    }
}

void MainWindow::toggleTimer()
{
    if (myTimer->isActive()){
        myTimer->stop();
        ui->timeButton->setText("Continue");
        for (int i = 0; i < 81; ++i) {
            ui->tableWidget->item(i/9,i%9)->setBackgroundColor(QColor(0,0,0));
        }
    } else {
        myTimer->start(1000);
        ui->timeButton->setText("Pause");
        for (int i = 0; i < 81; ++i) {

        }
    }
}

void MainWindow::showTime()
{
    time2 = time2.addSecs(1);
    QString str = time2.toString("hh:mm:ss");
    timeLabel->setText(str);

    Sudoku su;
    getTable(su);
    if (su.isCorrect()) {
        myTimer->stop();
        QMessageBox msgBox;
        msgBox.setWindowTitle("Congratulations!");
        msgBox.setText("You use only " + QString::number(QTime(0,0,0).secsTo(time2)) + " seconds to finish it!!\n" + "\n" + "Congratulations! " + name);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int result = msgBox.exec();
        switch (result){
            case QMessageBox::Ok:
                restart();
                break;
            default:
                break;
        }
        time2 = QTime(0,0,0);
        timeLabel->setText(time2.toString("hh:mm:ss"));
    }
}

void MainWindow::highLight(int row, int col)
{
    Sudoku su;
    getTable(su);
    int k = su.getElement(row,col);
    for (int i = 0; i < 81; ++i) {
        if (su.getElement(i) == k && k != 0) {
            ui->tableWidget->item(i/9,i%9)->setBackgroundColor(QColor(125,174,255));
        } else {
            // 設定方格顏色
            int c=i%9/3;
            int r=i/9/3;
            int m=c+r*3;
            if( m%2 == 0)
                ui->tableWidget->item(i/9,i%9)->setBackgroundColor(QColor(114,167,163));
            else
                ui->tableWidget->item(i/9,i%9)->setBackgroundColor(QColor(238,233,209));
        }
    }
}

void MainWindow::toggleHint()
{
    if (ui->hintBox->isChecked()) {
        ui->hintLabel->show();
    } else {
        ui->hintLabel->hide();
    }
}

void MainWindow::hint(int row, int col)
{
    Sudoku su;
    getTable(su);
    int k = row*9+col;
    int num = su.getElement(k);
    if (num == 0) {
        QVector<int> vec = su.getAvailableNum(k);
        QString str = "Hint: ";
        for (QVector<int>::iterator it = vec.begin(); it != vec.end(); it++)
            str += QString::number(*it) + " ";
        ui->hintLabel->setText(str);
    }
}

void MainWindow::inputName()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Hello friend!!"),tr("Please Enter Your Name："),QLineEdit::Normal,nameLabel->text(),&ok);
    if (ok && !text.isEmpty()) {
        name = text;
    }
}

void MainWindow::restart()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("It's fun!");
    msgBox.setText("Would you like to try again?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int result = msgBox.exec();
    switch (result) {
    case QMessageBox::Yes:
        newBoard();
        break;
    case QMessageBox::No:
        player.setVolume(30);
        break;
    default:
        break;
    }
}

void MainWindow::block()
{
    if (!myTimer->isActive()) {
        for (int i = 0; i < 81; ++i) {
            ui->tableWidget->item(i/9,i%9)->setBackgroundColor(QColor(0,0,0));
        }
    }
}

int MainWindow::getLevel()
{
    return level;
}


int myrandom (int i) {
    return qrand()%i;
}

bool MainWindow::create(Sudoku question, Sudoku & answer) {

    int firstZero;
    firstZero = question.getFirstZeroIndex();
    QVector<int> vec = question.getAvailableNum(firstZero);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    std::random_shuffle(vec.begin(),vec.end(),myrandom);
//    qDebug() << vec;

    if (firstZero == -1) {   // end condition
        if (question.isCorrect()) {
            answer = question;
            return true;
        } else
            return false;
    } else {
//        for (int num=1; num<=9; ++num) {
//            question.setElement( firstZero, num);
//            if (create(question, answer))
//                return true;
//        }
        for(QVector<int>::iterator it = vec.begin(); it != vec.end(); it++){
            question.setElement(firstZero, *it);
            if (create(question, answer))
                return true;
        }
        return false;
    }
}

bool MainWindow::solve(Sudoku question, Sudoku & answer) {

    int firstZero;
    firstZero = question.getFirstZeroIndex();
    QVector<int> vec = question.getAvailableNum(firstZero);

    if (firstZero == -1) {   // end condition
        if (question.isCorrect()) {
            answer = question;
            return true;
        } else
            return false;

    } else {
        for(QVector<int>::iterator it=vec.begin();it!=vec.end();it++){
            question.setElement( firstZero, *it);
            if (solve(question, answer))
                return true;
        }
        return false;
    }
}

void MainWindow::mute()
{
    if (player.isMuted()) {
        player.setMuted(false);
    }
    else {
        player.setMuted(true);
    }
}

void MainWindow::keyPress(int key)
{
    if (key == Qt::Key_M)
        ui->muteBox->toggle();
    if (key == Qt::Key_H)
        ui->hintBox->toggle();
    if (key == Qt::Key_P)
        toggleTimer();
    if (key == Qt::Key_C)
        ui->tableWidget->selectedItems().at(0)->setText("");
    if ('0' < key && key <= '9'){
//        qDebug() << key-'0';
        if(ui->tableWidget->selectedItems().count() == 1 && (ui->tableWidget->selectedItems().at(0)->text() == "" || ui->tableWidget->selectedItems().at(0)->textColor() == QColor(255,0,0))){
            if(myTimer->isActive()){
                Sudoku su,ans;
                ui->tableWidget->selectedItems().at(0)->setText(QString::number(key-'0'));
                getTable(su);
                QFont font;
                if (!solve(su,ans)) {
                    font.setBold(true);
                    ui->tableWidget->selectedItems().at(0)->setTextColor(QColor(255,0,0));
                    ui->tableWidget->selectedItems().at(0)->setFont(font);
                }
                else {
                    font.setBold(false);
                    ui->tableWidget->selectedItems().at(0)->setTextColor(QColor(0,0,0));
                    ui->tableWidget->selectedItems().at(0)->setFont(font);
                }
            }
        }
    }
}

void MainWindow::clearBoard()
{
    Sudoku su;
    setTable(su);
}

void MainWindow::newBoard()
{
    int map[81] = {0};
    Sudoku su(map), ans;
    create(su, ans);

    QVector<int> vec;
    for (int i = 0; i < 81; ++i)
        vec.push_back(i+1);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    std::random_shuffle(vec.begin(),vec.end(),myrandom);

    for (int i = 0; i < getLevel(); ++i)
        ans.setElement(vec.at(i),0);
    setTable(ans);
    myTimer->start(1000);
    time2 = QTime(0,0,0);
    timeLabel->setText(time2.toString("hh:mm:ss"));
    player.setVolume(70);

}

void MainWindow::solveBoard()
{
    Sudoku su, ans;
    getTable(su);
    if (solve(su,ans)){
        setTable(ans);
    } else {
        QMessageBox::warning(this, "Warning!", "This sudoku is NOT solvable!\nPlease set another one.");
    }


//    myTimer->stop();
//    time2 = QTime(0,0,0);
//    timeLabel->setText(time2.toString("hh:mm:ss"));
}

MainWindow::~MainWindow()
{
    player.stop();
    delete ui;
}
