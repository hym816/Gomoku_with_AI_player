#include "firorsec.h"
#include "ui_firorsec.h"

FirOrSec::FirOrSec(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FirOrSec)
{
    ui->setupUi(this);
}

FirOrSec::~FirOrSec()
{
    delete ui;
}
