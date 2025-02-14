#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaPlayer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetGameMode();
    connect(ui->Btn_ExitGame,SIGNAL(clicked()),this,SLOT(Btn_ExitGame_Click()));
    connect(ui->Btn_StartGame,SIGNAL(clicked()),this,SLOT(Btn_StartGame_Click()));
    ui->StatusBar->showMessage(GameModeTranslate(),0);


    player->setMedia(QUrl::fromLocalFile("C:\\Users\\haoyumin\\Desktop\\HappyGoBang-GUI\\Plants vs. Zombies-Faster.mp3"));
    player->setVolume(volume); // 设置音量，取值范围是0-100
    player->play(); // 开始播放

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::GameModeTranslate() {
    if (gameMode == LocalNon) {
        return "当前游戏模式：无禁手";
    }
    else if (gameMode == Local) {
        return "当前游戏模式：有禁手";
    }
    else if (gameMode == Entertain) {
        return "当前游戏模式：娱乐模式";
    }
    else {
        return "当前游戏模式：人机无禁手";
    }
}

void MainWindow::Btn_ExitGame_Click() {
    Action_Exit();
}

void MainWindow::Btn_StartGame_Click() {


    if (gameMode == AI) {
        askUser.show();
        return;
    } else if (gameMode == Entertain) {
        g.show();
    }
    else {
        this->hide();
        connect(&b,SIGNAL(sendsignal()),this,SLOT(ReShow()));
        b.initialization();
        b.show();
    }
    qDebug() << "game start";
}

void MainWindow::SetGameMode(GameMode mode) {
    gameMode = mode;
    ui->StatusBar->showMessage(GameModeTranslate(),0);
    qDebug() << mode;
}

void MainWindow::Action_Exit() {
    int ret = QMessageBox::question(NULL,"警告","要退出游戏吗？");
    if (ret == QMessageBox::Yes) {
        close();
        qDebug() << "game close";
    }
    else {
        qDebug() << "close cancel";
    }
}

void MainWindow::on_Action_LocalNon_triggered() {
    SetGameMode(LocalNon);
    b.gameMode = gameMode;
}

void MainWindow::on_Action_Entertain_triggered() {
    SetGameMode(Entertain);
    b.gameMode = gameMode;
}

void MainWindow::on_Action_Local_triggered() {
    SetGameMode(Local);
    b.gameMode = gameMode;
}

void MainWindow::on_Action_AI_triggered() {
    SetGameMode(AI);
    b.gameMode = gameMode;
}

void MainWindow::on_musicon_triggered() {
    volume = 100;
    player->setVolume(volume);
}

void MainWindow::on_musicoff_triggered() {
    volume = 0;
    player->setVolume(volume);
}



void MainWindow::on_Action_Info_triggered() {
    a.exec();
}

void MainWindow::on_Action_Settings_triggered() {
    s.exec();
    b.steptime = s.StepTime;
    b.gametime = s.GameTime;
    g.winCnt = s.winCount;
}

void MainWindow::ReShow() {
    this->show();
    SetGameMode(b.gameMode);
}

void MainWindow::ReTurn() {
}
