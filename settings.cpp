#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    for (int i = 15; i <= 60; i += 15) {
        ui->comboBox_StepTime->addItem(QString::number(i)+tr("秒"));
    }
    for (int i = 10; i <= 60; i += 10) {
        ui->comboBox_GameTime->addItem(QString::number(i)+tr("分钟"));
    }
    player->setMedia(QUrl::fromLocalFile(":/pic/pics/Plants vs. Zombies-Faster.mp3"));
    player->setVolume(100); // 设置音量，取值范围是0-100
    player->play(); // 开始播放
}

settings::~settings()
{
    delete ui;
}

void settings::on_buttonBox_accepted() {
    StepTime = ui->comboBox_StepTime->currentText().left(2).toInt();
    GameTime = ui->comboBox_GameTime->currentText().left(2).toInt();
    winCount = ui->CNT->value();

    voiceNum = 100;
    qDebug() << voiceNum << StepTime << GameTime << winCount;
}
