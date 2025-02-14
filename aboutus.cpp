#include "aboutus.h"
#include "ui_aboutus.h"

aboutus::aboutus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutus)
{
    ui->setupUi(this);
}

aboutus::~aboutus()
{
    delete ui;
}
