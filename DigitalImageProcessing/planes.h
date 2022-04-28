#ifndef PLANES_H
#define PLANES_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class planes;
}

class planes : public QWidget
{
    Q_OBJECT

public:
    explicit planes(QWidget *parent = nullptr);
    ~planes();

private slots:
    void receiveData(QString data);

private:
    Ui::planes *ui;
    void creatPlaneImage();
};

#endif // PLANES_H
