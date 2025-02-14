#ifndef BATTLEPAGE_H
#define BATTLEPAGE_H

#include <QMainWindow>
#include <QDebug>
#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>
#include "Algorithm.h"
#include "Socket.h"



#define BlackTurn 1
#define WhiteTurn 2

class Program {
public:
    void Win(string playername);
};

namespace Ui {
class battlepage;
}

class battlepage : public QMainWindow
{
    Q_OBJECT

public:
    explicit battlepage(QWidget *parent = nullptr);
    ~battlepage();
    void initialization();

    int mouse_x;
    int mouse_y;

    GoBangAlgorithm gobangAlogrithm;
    CheckerBoard checkerBoard;
    Program program;

    string username;

    GameMode gameMode;
    int steptime;
    int gametime;
    bool isBlackTurn;


private:
    Ui::battlepage *ui;

    QPainter *paint;
int lastX = 0,lastY = 0;
    int player();



    QTimer *timer;

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_Btn_Return_clicked();

    void ticker();

    void on_Btn_ConfessChess_clicked();

    void on_Btn_RepentStep_clicked();

    void on_Btn_SeekPeace_clicked();

signals:
    void sendsignal();
};

#endif // BATTLEPAGE_H
