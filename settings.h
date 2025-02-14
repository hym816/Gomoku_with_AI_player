#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QDebug>
#include <QMediaPlayer>
#include "entertainment.h"



namespace Ui {
class settings;
}

class settings : public QDialog
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = nullptr);
    ~settings();

    int voiceNum;
    int StepTime;
    int GameTime;
    int winCount;
    QMediaPlayer* player = new QMediaPlayer;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::settings *ui;
};

#endif // SETTINGS_H
