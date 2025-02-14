#ifndef FIRORSEC_H
#define FIRORSEC_H

#include <QWidget>
#include "aiplay.h"
#include "aifirst.h"

namespace Ui {
class FirOrSec;
}

class FirOrSec : public QWidget
{
    Q_OBJECT

public:
    explicit FirOrSec(QWidget *parent = nullptr);
    ~FirOrSec();

public slots:
    void on_first_clicked() {
        f.show();
    }
    void on_second_clicked() {
        s.show();
    }


private:
    Ui::FirOrSec *ui;
    GomokuBoard_1 f;
    aifirst s;

};

#endif // FIRORSEC_H
