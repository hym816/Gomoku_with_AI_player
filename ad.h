#ifndef AD_H
#define AD_H

#include <QWidget>

namespace Ui {
class ad;
}

class ad : public QWidget
{
    Q_OBJECT

public:
    explicit ad(QWidget *parent = nullptr);
    ~ad();

private:
    Ui::ad *ui;
};

#endif // AD_H
