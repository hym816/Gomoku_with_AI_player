#ifndef ABOUTUS_H
#define ABOUTUS_H

#include <QDialog>

namespace Ui {
class aboutus;
}

class aboutus : public QDialog
{
    Q_OBJECT

public:
    explicit aboutus(QWidget *parent = nullptr);
    ~aboutus();

private:
    Ui::aboutus *ui;
};

#endif // ABOUTUS_H
