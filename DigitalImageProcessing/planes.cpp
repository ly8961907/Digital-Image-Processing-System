#include "planes.h"
#include "ui_planes.h"

planes::planes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::planes)
{
    ui->setupUi(this);
}

planes::~planes()
{
    delete ui;
}
void planes::creatPlaneImage()
{
    int width = img.width();
    int hight = img.height();
    QRgb rgbVal = 0;
    int grayVal = 0;
    for(int x = 0;x < 8;x++)
    {
        *planImg[x] = grayImage.copy();
        for(int i = 0; i < width; i++)
        {
           for(int j = 0; j < hight; j++)
           {

                rgbVal = img.pixel(i, j);

                grayVal = qGray(rgbVal);    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算

                int num = 2^x;

                if(grayVal < num)
                {
                    grayVal = 0;
                    planImg[x]->setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
                }
                else
                {
                    grayVal = 255;
                    planImg[x]->setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
                }
           }
        }
    }
}
