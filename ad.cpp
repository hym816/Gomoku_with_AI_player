#include "ad.h"
#include "ui_ad.h"

ad::ad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ad)
{
    ui->setupUi(this);
}

ad::~ad()
{
    delete ui;
}
