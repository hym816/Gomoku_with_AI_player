#include "battlepage.h"
#include "ui_battlepage.h"
#include "ad.h"

battlepage::battlepage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::battlepage)
{
    ui->setupUi(this);
    steptime = 60;
    gametime = 60;


    timer = new QTimer(this);
    timer->setInterval(1000);

    timer->start();

    ui->lcdNumber_MyGameTime->setPalette(Qt::black);
    ui->lcdNumber_MyGameTime->display(gametime*60);
    ui->lcdNumber_MyStepTime->setPalette(Qt::black);
    ui->lcdNumber_MyStepTime->display(steptime);
    ui->lcdNumber_EnemyGameTime->setPalette(Qt::black);
    ui->lcdNumber_EnemyGameTime->display(gametime*60);
    ui->lcdNumber_EnemyStepTime->setPalette(Qt::black);
    ui->lcdNumber_EnemyStepTime->display(steptime);

    connect(timer,SIGNAL(timeout()),this,SLOT(ticker()));

    checkerBoard.SetSize(14);
    isBlackTurn = true;
    gameMode = LocalNon;
    checkerBoard.Route.SetPlayers(BlackTurn,WhiteTurn);




}

battlepage::~battlepage()
{
    delete ui;
}

int battlepage::player() {
    if (isBlackTurn) {
        return BlackTurn;
    }
    else
        return WhiteTurn;
}

void battlepage::initialization() {


    timer->start();

    ui->lcdNumber_MyGameTime->setPalette(Qt::black);
    ui->lcdNumber_MyGameTime->display(gametime*60);
    ui->lcdNumber_MyStepTime->setPalette(Qt::black);
    ui->lcdNumber_MyStepTime->display(steptime);
    ui->lcdNumber_EnemyGameTime->setPalette(Qt::black);
    ui->lcdNumber_EnemyGameTime->display(gametime*60);
    ui->lcdNumber_EnemyStepTime->setPalette(Qt::black);
    ui->lcdNumber_EnemyStepTime->display(steptime);




    // qDebug() << QString::fromStdString(username);
    if (gameMode == Local || gameMode == LocalNon) {
        if (username == "") {
            username = "黑棋棋手";
        }
        ui->Lbl_MyName->setText(QString::fromStdString(username));
        ui->Lbl_EnemyName->setText("白棋棋手");
    }

    for (int i = 0; i <= 15; i++) {
        for (int j = 0; j <= 15; j++) {
            checkerBoard.Board[i][j].SetChess(i,j,-1,-1);
        }
    }
    while (checkerBoard.Route.RecentStep().PLAYERID() != -1) {
        checkerBoard.Route.DelRecentStep();
    }
    isBlackTurn = true;

    update();
}


void battlepage::paintEvent(QPaintEvent *event) {
    const int SIZE=13;
    const int WIDTH=40;
    const int x=30,y=30;
    paint = new QPainter;
    paint->begin(this);

    paint->setPen(QPen(Qt::black,1,Qt::SolidLine));//钢笔工具：颜色，线号，实线

  //画SIZE+1条横线
    for(int i = 0; i < SIZE+1; i++) {
      paint->drawLine(x,y+WIDTH*i,x+WIDTH*(SIZE),y+WIDTH*i);//画线函数：x1,y1,x2,y2:画从(x1,y1)到(x2,y2)的线
    }

  //画SIZE+1条竖线
    for(int i = 0; i < SIZE+1; i++) {
      paint->drawLine(x+WIDTH*i,y,x+WIDTH*i,y+WIDTH*(SIZE));
    }

    for (int i = 1; i <= SIZE+1; i++) {
        for (int j = 1; j <= SIZE+1; j++) {
            if (checkerBoard.Board[i][j].PLAYERID() == BlackTurn) {
                paint->setBrush(QBrush(Qt::black,Qt::SolidPattern));
                paint->drawEllipse((40*i-25),(40*j-25),30,30);
            }
            else if (checkerBoard.Board[i][j].PLAYERID() == WhiteTurn) {
                paint->setBrush(QBrush(Qt::white,Qt::SolidPattern));
                paint->drawEllipse((40*i-25),(40*j-25),30,30);
            }
        }
    }

    // 绘制红点，标记上一步落子的位置
    if (checkerBoard.Route.StepCount() > 0) {
qDebug() << "REDPOINT" <<   x + WIDTH * lastX <<   y + WIDTH * lastY;
        paint->setPen(QPen(Qt::red, 10, Qt::SolidLine));  // 设置红色画笔
        paint->drawPoint(x + WIDTH * lastX - 40, y + WIDTH * lastY - 40);  // 绘制红点
    }

    paint->end();
}

void battlepage::mouseMoveEvent(QMouseEvent *event) {
    int mousex = event->x(); int mousey = event->y();
    // qDebug() << mousex << mousey;
    if (mousex >= 10 && mousey >= 10 && mousex <= 570 && mousey <= 570) {
        int checkerboardx = (mousex-10)/40 + 1;
        int checkerboardy = (mousey-10)/40 + 1;
        lastX = checkerboardx;
        lastY = checkerboardy;
         qDebug() <<"UPDATED";
    }

}

void battlepage::mouseReleaseEvent(QMouseEvent *event) {
    if (gameMode == Local || gameMode == LocalNon) {
        int mousex = event->x(); int mousey = event->y();
        qDebug() << mousex << mousey;
        if (mousex >= 10 && mousey >= 10 && mousex <= 570 && mousey <= 570) {
            int checkerboardx = (mousex-10)/40 + 1;
            int checkerboardy = (mousey-10)/40 + 1;
            lastX = checkerboardx;
            lastY = checkerboardy;
            qDebug() << checkerboardx << checkerboardy;
            if (checkerBoard.Board[checkerboardx][checkerboardy].PLAYERID() == -1) {

                update();






                checkerBoard.PlaceNode(checkerboardx,checkerboardy,player());
                qDebug() << checkerboardx << checkerboardy << player();
                isBlackTurn = !isBlackTurn;
                int winnerid = gobangAlogrithm.GoBangJudger(checkerBoard.Route,checkerBoard.Board,14,gameMode);
                if (winnerid != -1) {
                    string winner;
                    timer->stop();
                    if (winnerid == BlackTurn) {
                        program.Win("黑棋");
                        this->hide();
                        // initialization();
                        emit sendsignal();
                    }
                    else {
                        program.Win("白棋");
                        this->hide();
                        // initialization();
                        emit sendsignal();
                    }
                }
            }
            // qDebug() << checkerboardx << checkerboardy;
        }
    }
}

void battlepage::on_Btn_Return_clicked() {
    timer->stop();
    int ret = QMessageBox::question(NULL,"提示","您确定结束对局并返回主菜单吗?");
    if (ret == QMessageBox::Yes) {
        this->hide();
        // initialization();
        emit sendsignal();
        // program.Win("CYY");
    }
}






void Program::Win(string playername) {
    QMessageBox::information(NULL,"提示",QString::fromStdString(playername+"获得胜利!"));
}

void battlepage::on_Btn_ConfessChess_clicked()
{
    timer->stop();
    QString qstr = "";
    if (isBlackTurn == true) {
        qstr = "黑棋认输，白棋胜利";
    }
    else {
        qstr = "白棋认输，黑棋胜利";
    }
    QMessageBox::information(NULL,"胜利",qstr);
    this->hide();
    emit sendsignal();

}

void battlepage::on_Btn_RepentStep_clicked()
{
    timer->stop();
    if (checkerBoard.Route.StepCount() == 2) {
        QMessageBox::information(NULL,"警告","当前无棋可悔");
        timer->start();
        return;
    }
    QString qstr = "";
    if (isBlackTurn == true) {
        qstr = "白棋悔棋，是否同意？";
    }
    else {
        qstr = "黑棋悔棋，是否同意？";
    }
    int ret = QMessageBox::question(NULL,"悔棋",qstr);
    if (ret == QMessageBox::Yes) {
        ui->lcdNumber_MyStepTime->display(steptime);
        ui->lcdNumber_EnemyStepTime->display(steptime);
        timer->start();
        checkerBoard.Board[checkerBoard.Route.RecentStep().LOCATION()[0]][checkerBoard.Route.RecentStep().LOCATION()[1]].SetChess(checkerBoard.Route.RecentStep().LOCATION()[0],checkerBoard.Route.RecentStep().LOCATION()[1],-1,-1);
        checkerBoard.Route.DelRecentStep();
        isBlackTurn = !isBlackTurn;
        update();
        return;
    }
    timer->start();
}

void battlepage::on_Btn_SeekPeace_clicked()
{
    timer->stop();
    QString qstr = "";
    if (isBlackTurn == true) {
        qstr = "黑棋求和，是否同意？";
    }
    else {
        qstr = "白棋求和，是否同意？";
    }
    int ret = QMessageBox::question(NULL,"求和",qstr);
    if (ret == QMessageBox::Yes) {

        QMessageBox::information(NULL,"和棋","双方平手");
        this->hide();
        return;
    }
    timer->start();
}

void battlepage::ticker() {
    if(checkerBoard.Route.RecentStep().PLAYERID() != -1) {
        if (isBlackTurn) {
            ui->lcdNumber_EnemyStepTime->display(steptime);
            ui->lcdNumber_MyStepTime->display(ui->lcdNumber_MyStepTime->intValue()-1);
            ui->lcdNumber_MyGameTime->display(ui->lcdNumber_MyGameTime->intValue()-1);
            if (ui->lcdNumber_MyStepTime->intValue()==0 || ui->lcdNumber_MyGameTime->intValue()==0) {
                QMessageBox::information(NULL,"胜利","黑棋超时，白棋胜利");
                this->hide();
            }
        }
        else {
            ui->lcdNumber_MyStepTime->display(steptime);
            ui->lcdNumber_EnemyStepTime->display(ui->lcdNumber_EnemyStepTime->intValue()-1);
            ui->lcdNumber_EnemyGameTime->display(ui->lcdNumber_EnemyGameTime->intValue()-1);
            if (ui->lcdNumber_EnemyStepTime->intValue()==0 || ui->lcdNumber_EnemyGameTime->intValue()==0) {
                QMessageBox::information(NULL,"胜利","白棋超时，黑棋胜利");
                this->hide();
            }
        }
    }
}
