#ifndef BITPLANE_H
#define BITPLANE_H

#include <QWidget>

namespace Ui {
class bitPlane;
}

class bitPlane : public QWidget
{
    Q_OBJECT


public:
    explicit bitPlane(QWidget *parent = nullptr);
    ~bitPlane();
    QImage a;
    QVector<QImage *> Img;
    void work();
private slots:

private:
    Ui::bitPlane *ui;
};

#endif // BITPLANE_H
