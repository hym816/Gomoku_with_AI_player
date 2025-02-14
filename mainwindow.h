#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "aboutus.h"
#include "settings.h"

#include "battlepage.h"

#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QDesktopServices>
#include "aiplay.h"
#include "ad.h"

#include <QMediaPlayer>




#include "aifirst.h"

#include "firorsec.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetGameMode(GameMode mode = LocalNon);
    aboutus a;
    settings s;

    battlepage b;

    string username;
    GameMode gameMode;
    int steptime;
    int gametime;

private:
    Ui::MainWindow *ui;
    QString GameModeTranslate();
    GomokuBoard_1 ai;
    aifirst AIFIRST;
    FirOrSec askUser;
    int volume = 100;
    // 在你的类中或者函数中
    QMediaPlayer* player = new QMediaPlayer;
    GomokuBoard_2 g;

public slots:
    void Btn_StartGame_Click();
    void Btn_ExitGame_Click();
    void Action_Exit();


private slots:
    void on_Action_LocalNon_triggered();
    void on_Action_Local_triggered();
    void on_Action_AI_triggered();
    void on_Action_Info_triggered();
    void on_Action_Settings_triggered();
    void on_musicon_triggered();
    void on_musicoff_triggered();
    void on_Action_Entertain_triggered();
    void ReShow();
    void ReTurn();
};
#endif // MAINWINDOW_H
