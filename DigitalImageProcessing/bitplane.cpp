#include "bitplane.h"
#include "ui_bitplane.h"

bitPlane::bitPlane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::bitPlane)
{
    ui->setupUi(this);
    for(int i = 0; i < 8; i++)
    {
        QImage *a = new QImage;
        Img.push_back(a);
    }
}

bitPlane::~bitPlane()
{
    delete ui;
}
void bitPlane::work()
{
    ui->label->setPixmap(QPixmap::fromImage(a.scaled(a.size())));
    ui->label_2->setPixmap(QPixmap::fromImage(Img[1]->scaled(Img[1]->size())));
    ui->label_3->setPixmap(QPixmap::fromImage(Img[2]->scaled(Img[2]->size())));
    ui->label_4->setPixmap(QPixmap::fromImage(Img[3]->scaled(Img[3]->size())));
    ui->label_5->setPixmap(QPixmap::fromImage(Img[4]->scaled(Img[4]->size())));
    ui->label_6->setPixmap(QPixmap::fromImage(Img[5]->scaled(Img[5]->size())));
    ui->label_7->setPixmap(QPixmap::fromImage(Img[6]->scaled(Img[6]->size())));
    ui->label_8->setPixmap(QPixmap::fromImage(Img[7]->scaled(Img[7]->size())));

    ui->label->setScaledContents(true);
    ui->label_2->setScaledContents(true);
    ui->label_3->setScaledContents(true);
    ui->label_4->setScaledContents(true);
    ui->label_5->setScaledContents(true);
    ui->label_6->setScaledContents(true);
    ui->label_7->setScaledContents(true);
    ui->label_8->setScaledContents(true);
}
