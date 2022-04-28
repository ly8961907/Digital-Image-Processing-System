#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    setWindowTitle("数字图像处理");
    connect(ui->action, SIGNAL(triggered()), this, SLOT(open()));//打开图片
    connect(ui->action_2, SIGNAL(triggered()), this, SLOT(save()));//保存图片
    connect(ui->action1, SIGNAL(triggered()), this, SLOT(linearTrans()));//线性变换A=1.2,B=50
    connect(ui->action2, SIGNAL(triggered()), this, SLOT(linearTrans_2()));//线性变换
    connect(ui->action1_2, SIGNAL(triggered()), this, SLOT(nonlinearTrans()));//非线性变换
    connect(ui->action2_2, SIGNAL(triggered()), this, SLOT(nonlinearTrans_2()));//非线性变换
    connect(ui->action_8, SIGNAL(triggered()), this, SLOT(equal_orig()));//图像均衡化初始版本
    connect(ui->action_9, SIGNAL(triggered()), this, SLOT(equal_up()));//图像均衡化升级本
    connect(ui->action_3, SIGNAL(triggered()), this, SLOT(bitPlan()));//生成位平面图
    connect(ui->actionBMP, SIGNAL(triggered()), this, SLOT(bmp()));//bmp
    connect(ui->action_4, SIGNAL(triggered()), this, SLOT(FFT()));//傅里叶变换
    connect(ui->action_5, SIGNAL(triggered()), this, SLOT(DCT()));//DCT
    connect(ui->action_6, SIGNAL(triggered()), this, SLOT(WT()));//小波
    connect(ui->action_11, SIGNAL(triggered()), this, SLOT(aveSmooth()));//均值平滑
    connect(ui->action_12, SIGNAL(triggered()), this, SLOT(midSmooth()));//中值平滑
    connect(ui->action_7, SIGNAL(triggered()), this, SLOT(gausSmooth()));//高斯平滑
    connect(ui->action1_3, SIGNAL(triggered()), this, SLOT(LaplaceSharpe()));//拉普拉斯锐化
    connect(ui->action2_3, SIGNAL(triggered()), this, SLOT(sobelSharpe()));//sobel锐化
    connect(ui->action3, SIGNAL(triggered()), this, SLOT(PrewittSharpe()));//prewitt锐化
    connect(ui->action256, SIGNAL(triggered()), this, SLOT(bit24ToColor256()));//256色
    connect(ui->actionsobel, SIGNAL(triggered()), this, SLOT(SobelEdge()));//sobel边缘分割
    connect(ui->action_10, SIGNAL(triggered()), this, SLOT(LaplaceEdge()));//拉普拉斯边缘分割
    connect(ui->actionPrewitt, SIGNAL(triggered()), this, SLOT(PrewittEdge()));//Prewitt边缘分割
    connect(ui->action_13, SIGNAL(triggered()), this, SLOT(HoughTransform()));//霍夫边缘检测
    connect(ui->action_14, SIGNAL(triggered()), this, SLOT(RLE()));//游程编码
    connect(ui->actionHuffman, SIGNAL(triggered()), this, SLOT(Huffman()));//哈夫曼编码
    connect(ui->action_15, SIGNAL(triggered()), this, SLOT(edgeTracing()));//边缘追踪
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setHistogram(long long tot,long long tot2,int xiangsu,QVector<double> datay)
{
    QVector<double> datax;
    for(double i = 0; i < 256; i++)
        datax.append(i);
    double yMax = 0;
    for(int j = 0; j < 256 ; j++)
    {
        if(yMax < datay.at(j))
            yMax = datay.at(j);
    }
    ui->widget->xAxis->setRange(0,255);  //x轴范围
    ui->widget->yAxis->setRange(0,int(yMax));  //y轴范围
    QCPBars *bars = new QCPBars(ui->widget->xAxis, ui->widget->yAxis);
    bars->setData(datax, datay);
    bars->setPen(QColor(0, 0, 0));
    bars->setWidth(0.05);
    ui->widget->setVisible(true);
    ui->widget->replot();

    aveGray = tot/xiangsu;
    QString ave = QString::number(aveGray);
    int bzc = sqrt(tot2/xiangsu - aveGray*aveGray);
    QString Qbzc = QString::number(bzc);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem(ave));//算平均灰度
    ui->tableWidget->setItem(2,0,new QTableWidgetItem(Qbzc));//算标准差
}

void MainWindow::open()
{

    QString fileName = QFileDialog::getOpenFileName(this,tr("选择图像"),"","Image Files(*.jpg *.png *.jpeg *.bmp)");
    string str = fileName.toStdString();
    imgPath = str;
    mat =  cv::imread(str,0);

    if(fileName.isEmpty())
    {
        return;
    }
    else
    {
        if(!img.load(fileName))//加载图像
        {
            QMessageBox::information(this,tr("警告"),tr("打开图像失败！"));
            return;
        }

        int width = img.width();
        int height = img.height();

        QImage  *grayImg = new QImage(width, height, QImage::Format_Grayscale8);

        if(!img.allGray())//change to grayImage
        {
            //img = img.convertToFormat(QImage::Format_Grayscale8);
            QRgb rgbVal = 0;
            int grayVal = 0;
            for (int x = 0; x < width; ++x)
            {
                for (int y = 0; y < height; ++y)
                {
                    rgbVal = img.pixel(x, y);

                    grayVal = qGray(rgbVal);    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算

                    grayImg->setPixel(x, y, QColor(grayVal, grayVal, grayVal).rgb());
                }
            }
        }
        else
        {
            *grayImg = img.copy();
        }
        grayImage = grayImg->copy();
        //QImage

        if(img.load(fileName))
        {
            QVector<double> datay;
            ui->widget->clearGraphs();
            datay.resize(256);
            histogram.resize(256);
            for(int i = 0; i < 256; i = i + 1)
                histogram[i] = 0;
            //  对直方图进行数据处理

            //  计算像素数
            QString w = QString::number(width);
            QString h = QString::number(height);
            QString pixel = w+"*"+h;
            ui->tableWidget->setItem(3,0,new QTableWidgetItem(pixel));

            long long tot = 0;
            long long tot2 =0;
            //计算各灰度像素数
            for(int i = 0; i < width; i++)
            {
               for(int j = 0; j < height; j++)
               {
                   int nIndex = int(grayImg->bits()[i*height + j]);
                   datay[nIndex] = datay.at(nIndex) + 1;
                   histogram[nIndex] = histogram.at(nIndex) + 1;
                   tot += nIndex;
                   tot2 += nIndex * nIndex;
               }
            }
            setHistogram( tot, tot2, width * height, datay);
            ui->label->setPixmap(QPixmap::fromImage(*grayImg));
            nowImg = grayImage.copy();
        }
    }
}

void MainWindow::save()
{
    QString filename1 = QFileDialog::getSaveFileName(this,tr("另存为图片"),"",tr("Images (*.jpg *.png *.jpeg *.bmp)"));
    QScreen *screen = QGuiApplication::primaryScreen();
    screen->grabWindow(ui->label->winId()).save(filename1);
}


void MainWindow::on_pushButton_4_clicked()//生成二值图
{
    QString str = ui->lineEdit->text();
    int thrVal =str.toInt();
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    QImage binImg = grayImage.copy();
    QString thr = QString::number(thrVal);
    ui->tableWidget->setItem(1,0,new QTableWidgetItem(thr));//阈值
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {
            grayVal = qGray(img.pixel(i, j));    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算

            if(grayVal < thrVal)
            {
                grayVal = 0;
                binImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
            }
            else
            {
                grayVal = 255;
                binImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
            }
       }
    }
    ui->label->setPixmap(QPixmap::fromImage(binImg.scaled(binImg.size())));
    nowImg = binImg.copy();
}

void MainWindow::bitPlan()
{
    int width = img.width();
    int height = img.height();
    QRgb rgbVal = 0;
    int grayVal = 0;
    for(int i = 0; i < 8; i++)
    {
        QImage *a = new QImage;
        *a = grayImage.copy();
        planImg.push_back(a);
    }
    int num[8] = {1,2,4,8,16,32,64,128};//8个阈值
    for(int x = 0;x < 8;x++)
    {
        for(int i = 0; i < width; i++)
        {
           for(int j = 0; j < height; j++)
           {
                rgbVal = img.pixel(i, j);

                grayVal = qGray(rgbVal);    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算
                if(grayVal < num[x])
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

    dialog = new bitPlane();
    dialog->a = planImg[0]->copy();
    for(int i=0 ; i < 8 ; i++)
    {
        * dialog->Img[i] = planImg[i]->copy();
    }
    dialog->work();
    dialog->show();
}

void MainWindow::on_pushButton_clicked()//生成量化图
{
    //量化等级越高，图片质量越好
    //量化等级越多，所得图像层次越丰富，灰度分辨率高，图 像质量好，但数据量大；
    //量化等级越少，图像层次欠丰富，灰度分辨率低，图像质量变差，但数据量小。
    //图像的量化就是将取样后图像的每个样点的取值范围分成若干区间，并仅用一个数值代表每个区间中的所有取值。
    //如4 就是把 0-255分成4分 0-64 64 -128 128 -192 192 - 254每一份就是 0 64 128 192
    QString str = ui->lineEdit_2->text();
    int quaVal =str.toInt();
    QRgb rgbVal = 0;
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    QImage quaImg = grayImage.copy();
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {

            rgbVal = img.pixel(i, j);

            grayVal = qGray(rgbVal);    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算

            int num = int((grayVal * quaVal / 256) * 256 / (quaVal - 1));
            if(num < 0)
                grayVal = 0;
            if(num > 255)
                grayVal = 255;
            quaImg.setPixel(i, j, QColor(num, num, num).rgb());
       }
    }
    ui->label->setPixmap(QPixmap::fromImage(quaImg.scaled(quaImg.size())));
    nowImg = quaImg.copy();

}

void MainWindow::on_pushButton_2_clicked()//生成采样图
{
    //采样间隔越大，所得图像像素数越少，空间分辨率低，质量差，严重时出现马赛克效应；采样间隔越小，所得图像像素数越多，空间分辨率高，图像质量好，但数据量大。
    //例如输入3就是有个3*3矩阵然后这个矩阵的所有取值就是最右上角的值
    QVector<double> datay;
    long long tot = 0;
    long long tot2 =0;
    ui->widget->clearGraphs();
    datay.resize(256);

    QString str = ui->lineEdit_3->text();
    int sampVal =str.toInt();
    QRgb rgbVal = 0;
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    QImage sampImg = grayImage.copy();
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {
            rgbVal = img.pixel(i - i % sampVal,j - j % sampVal);

            grayVal = qGray(rgbVal);    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算

            sampImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());

            datay[grayVal] = datay.at(grayVal) + 1;

            tot += grayVal;

            tot2 += grayVal * grayVal;
       }
    }
        setHistogram( tot,tot2, width * height,  datay);
        ui->label->setPixmap(QPixmap::fromImage(sampImg.scaled(sampImg.size())));
        nowImg = sampImg.copy();

}

char bmp_get_char(int grayVal)
{
    char txtMap[8] = {'@','$','#','%','!','~','^','`'};
    //char txtMap[2] = {' ','$'};
    if(grayVal == 255)
    {
        return ' ';
    }
    int num = grayVal % 8;
    return txtMap[num];

}
void MainWindow::bmp()
{
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    QString filename = QFileDialog::getSaveFileName(this,tr("新建txt"),"",tr("文本 (*.txt)"));
    //字符数组大小
    QFile file(filename);
    QString str = "";
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::information(this, "Error Message", "Please Select a Text File!");
        return;
    }
      for(int i = 0; i < height; i++)
      {
         for(int j = 0; j < width; j++)
         {
                grayVal = qGray(img.pixel(j, i));    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算
                str += bmp_get_char(grayVal);
         }
         str += "\n";
      }
      file.open(QIODevice::WriteOnly);
      file.write(str.toUtf8());
      file.close();
      QMessageBox::information(this, "Success", "Success");
}

void MainWindow::linearTrans()//A=1.2,B=50
{
    QRgb rgbVal = 0;
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    QImage linerImg = grayImage.copy();
    QVector<double> datax;
    QVector<double> datay;
    long long tot = 0;
    long long tot2 =0;
    ui->widget->clearGraphs();
    datay.resize(256);
    for(double i = 0; i < 256; i = i + 1)
        datax.append(i);
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {

            rgbVal = img.pixel(i ,j);

            grayVal = qGray(rgbVal);    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算

            grayVal = int(1.2*grayVal + 50);

            if(grayVal > 255)
                grayVal = 255;
            linerImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());

            datay[grayVal] = datay.at(grayVal) + 1;

            tot += grayVal;
            tot2 += grayVal * grayVal;
       }
    }
    setHistogram( tot,tot2, width * height, datay);
    ui->label->setPixmap(QPixmap::fromImage(linerImg.scaled(linerImg.size())));
    nowImg = linerImg.copy();
}

void MainWindow::linearTrans_2()//A=0.8,B=-30
{
    QRgb rgbVal = 0;
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    QImage linerImg = grayImage.copy();
    QVector<double> datax;
    QVector<double> datay;
    long long tot = 0;
    long long tot2 =0;
    ui->widget->clearGraphs();
    datay.resize(256);
    for(double i = 0; i < 256; i = i + 1)
        datax.append(i);
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {

            rgbVal = img.pixel(i ,j);

            grayVal = qGray(rgbVal);    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算

            grayVal = int(0.8*grayVal -30 );

            if(grayVal < 0)
                grayVal = 0;
            linerImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());

            datay[grayVal] = datay.at(grayVal) + 1;

            tot += grayVal;
            tot2 += grayVal * grayVal;
       }
    }
    setHistogram( tot,tot2, width * height,  datay);
    ui->label->setPixmap(QPixmap::fromImage(linerImg.scaled(linerImg.size())));
    nowImg = linerImg.copy();
}


void MainWindow::nonlinearTrans()//D = 150*COS(PI*D/255/2)
{
    QRgb rgbVal = 0;
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    QImage linerImg = grayImage.copy();
    QVector<double> datay;
    long long tot = 0;
    long long tot2 =0;
    ui->widget->clearGraphs();
    datay.resize(256);
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {
            rgbVal = img.pixel(i ,j);

            grayVal = qGray(rgbVal);    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算

            grayVal = int(150 * cos(PI * grayVal/255/2));

            if(grayVal < 0)
                grayVal = 0;

            if(grayVal > 255)
                grayVal = 255;

            linerImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());

            datay[grayVal] = datay.at(grayVal) + 1;

            tot += grayVal;

            tot2 += grayVal * grayVal;
       }
    }
    setHistogram( tot,tot2, width * height,  datay);
    ui->label->setPixmap(QPixmap::fromImage(linerImg.scaled(linerImg.size())));
    nowImg = linerImg.copy();
}

void MainWindow::nonlinearTrans_2()//D = 128*SIN(PI*D/255)
{
    QRgb rgbVal = 0;
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    QImage linerImg = grayImage.copy();
    QVector<double> datay;
    long long tot = 0;
    long long tot2 =0;
    ui->widget->clearGraphs();
    datay.resize(256);
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {

            rgbVal = img.pixel(i ,j);

            grayVal = qGray(rgbVal);    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算

            grayVal = int(128 * sin(PI * grayVal/255));

            if(grayVal < 0)
                grayVal = 0;
            if(grayVal > 255)
                grayVal = 255;
            linerImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());

            datay[grayVal] = datay.at(grayVal) + 1;

            tot += grayVal;
            tot2 += grayVal * grayVal;

       }
    }
    setHistogram( tot,tot2, width * height,  datay);
    ui->label->setPixmap(QPixmap::fromImage(linerImg.scaled(linerImg.size())));
    nowImg = linerImg.copy();
}

void MainWindow::equal_orig()//图像均衡化初始版本
{
    QRgb rgbVal = 0;
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    int pixel = width * height;
    int LUT[256];
    ui->widget->clearGraphs();
    LUT[0] = int(1.0*histogram[0] / pixel*255);
    int sum = int(histogram[0]);
    for (int i = 1; i <= 255; ++i)
    {
        sum += histogram[i];

        LUT[i] = int(1.0*sum / pixel * 255);
    }

    QImage linerImg = grayImage.copy();
    QVector<double> datay;
    long long tot = 0;
    long long tot2 =0;
    ui->widget->clearGraphs();
    datay.resize(256);
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {

            rgbVal = img.pixel(i ,j);

            grayVal = qGray(rgbVal);    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算

            grayVal = LUT[grayVal];

            if(grayVal < 0)
                grayVal = 0;
            if(grayVal > 255)
                grayVal = 255;
            linerImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());

            datay[grayVal] = datay.at(grayVal) + 1;

            tot += grayVal;
            tot2 += grayVal * grayVal;

       }
    }
    setHistogram( tot,tot2, width * height,  datay);
    ui->label->setPixmap(QPixmap::fromImage(linerImg.scaled(linerImg.size())));
    nowImg = linerImg.copy();
}

void MainWindow::equal_up()//图像均衡化优化版本
{
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    int pixel = width * height;
    int LUT[256];
    ui->widget->clearGraphs();
    LUT[0] = int(histogram[0]);
    LUT[255] = int(histogram[255]);
    int sum = int(histogram[0]);
    for (int i = 1; i <= 254; ++i)
    {
        sum += histogram[i];

        LUT[i] = int((sum *1.0  * 255/(pixel-histogram[i]))+0.5);
    }

    QImage linerImg = grayImage.copy();
    QVector<double> datay;
    long long tot = 0;
    long long tot2 =0;
    ui->widget->clearGraphs();
    datay.resize(256);
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {
            grayVal = qGray(img.pixel(i ,j));    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计算

            grayVal = LUT[grayVal];

            linerImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());

            datay[grayVal] = datay.at(grayVal) + 1;

            tot += grayVal;

            tot2 += grayVal * grayVal;
       }
    }
    setHistogram( tot,tot2, width * height, datay);
    ui->label->setPixmap(QPixmap::fromImage(linerImg));
    nowImg = linerImg.copy();
}

void MainWindow::on_pushButton_7_clicked()//顺时针
{
    QString str = ui->lineEdit_10->text();
    double angle =str.toDouble();
    angle = -1*angle*3.1415926/180;
    int width = img.width();
    int height = img.height();
    int newWidth = int(width * abs(cos(angle)) + height * abs(sin(angle))),
        newHeight = int(height * abs(cos(angle)) + width * abs(sin(angle)));
    QImage rotateImg(newWidth, newHeight, QImage::Format_Grayscale8);
    for(int i = 0; i < newWidth; i++)
    {
       for(int j = 0; j < newHeight; j++)
       {
            int i0 = int(i * cos(angle) - j * sin(angle) - 0.5 * newWidth * cos(angle) + 0.5 * newHeight * sin(angle) + 0.5 * width + 0.5);//得到原图中的位置
            int j0 = int(i * sin(angle) + j * cos(angle) - 0.5 * newWidth * sin(angle) - 0.5 * newHeight * cos(angle) + 0.5 * height + 0.5);

            if(i0 < 0 || i0 >= width || j0 < 0 || j0  >= height)
            {
                rotateImg.setPixel(i, j, QColor(255, 255, 255).rgb());
            }
            else
            {
                QRgb rgbVal = nowImg.pixel(i0 ,j0);
                rotateImg.setPixel(i, j, rgbVal);
            }
       }
    }
    ui->label->setPixmap(QPixmap::fromImage(rotateImg));
}

void MainWindow::on_pushButton_3_clicked()//逆时针
{
    QString str = ui->lineEdit_9->text();
    double angle =str.toDouble();
    angle = angle*3.1415926/180;
    int width = img.width();
    int height = img.height();
    int newWidth = int(width * abs(cos(angle)) + height * abs(sin(angle))),
        newHeight = int(height * abs(cos(angle)) + width * abs(sin(angle)));
    QImage rotateImg(newWidth, newHeight, QImage::Format_Grayscale8);
    for(int i = 0; i < newWidth; i++)
    {
       for(int j = 0; j < newHeight; j++)
       {
            int i0 = int(i * cos(angle) - j * sin(angle) - 0.5 * newWidth * cos(angle) + 0.5 * newHeight * sin(angle) + 0.5 * width + 0.5);//得到原图中的位置
            int j0 = int(i * sin(angle) + j * cos(angle) - 0.5 * newWidth * sin(angle) - 0.5 * newHeight * cos(angle) + 0.5 * height + 0.5);

            if(i0 < 0 || i0 >= width || j0 < 0 || j0  >= height)
            {
                rotateImg.setPixel(i, j, QColor(255, 255, 255).rgb());
            }
            else
            {
                QRgb rgbVal = nowImg.pixel(i0 ,j0);
                rotateImg.setPixel(i, j, rgbVal);
            }
       }
    }
    ui->label->setPixmap(QPixmap::fromImage(rotateImg));
}


void MainWindow::on_pushButton_5_clicked() //最临近差值缩放
{
    QVector<double> datay;
    long long tot = 0,tot2 = 0;
    ui->widget->clearGraphs();
    datay.resize(256);
    QString str = ui->lineEdit_4->text();
    double scal =str.toDouble();
    QRgb rgbVal = 0;
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    int newWidth = int(width * scal);
    int newHeight = int(height * scal);
    double sw = (height * 1.0) / newHeight ,sh = (width * 1.0) / newWidth;//缩放比例
    QImage *scalImg = new QImage(newWidth , newHeight , QImage::Format_Grayscale8);
    for(int i = 0; i < newWidth; i++)
    {
       for(int j = 0; j < newHeight; j++)
       {
            int p_x = int(i * sw + 0.5 * (sw-1));//找到在原图中的坐标位置
            int p_y = int(j * sh + 0.5 * (sh-1));
            rgbVal = img.pixel(p_x ,p_y);
            grayVal = qGray(rgbVal);    // 这里调用Qt的函数，使用(R * 11 + G * 16 + B * 5)/32的方法计
            if(grayVal < 0)
                grayVal = 0;
            if(grayVal > 255)
                grayVal = 255;
            scalImg->setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
            datay[grayVal] = datay.at(grayVal) + 1;
            tot += grayVal;
            tot2 += grayVal * grayVal;
       }
    }
        QString w = QString::number(newWidth);
        QString h = QString::number(newHeight);
        QString pixel = w+"*"+h;
        ui->tableWidget->setItem(3,0,new QTableWidgetItem(pixel));
        setHistogram( tot, tot2, newWidth * newHeight, datay);
        ui->label->setPixmap(QPixmap::fromImage(scalImg->scaled(scalImg->size())));
        nowImg = scalImg->copy();
}

void MainWindow::on_pushButton_8_clicked()//双线性
{
    /*双线性插值通过寻找距离这个对应坐标最近的四个像素点，来计算该点的值（灰度值或者RGB值）。
    目标图的象素坐标为（1，1），那么反推得到的对应于源图的坐标是（0.75 , 0.75）, 这其实只是一个概念上
    的虚拟象素,实际在源图中并不存在这样一个象素,那么目标图的象素（1，1）的取值不能够由这个虚拟象素来决定，而只能由
    源图的这四个象素共同决定：（0，0）（0，1）（1，0）（1，1），而由于（0.75,0.75）离（1，1）要更近一些
    ，那么（1,1）所起的决定作用更大一些，这从公式1中的系数uv=0.75×0.75就可以体现出来，而（0.75,0.75）离（0，0）
     最远，所以（0，0）所起的决定作用就要小一些，公式中系数为(1-u)(1-v)=0.25×0.25也体现出了这一特点；*/
    QVector<double> datay;
    long long tot = 0,tot2 = 0;
    ui->widget->clearGraphs();
    datay.resize(256);
    QString str = ui->lineEdit_5->text();
    double scal =str.toDouble();
    QRgb  rgbVal_1, rgbVal_2, rgbVal_3, rgbVal_4;
    int grayVal_1,grayVal_2,grayVal_3,grayVal_4;
    int width = img.width();
    int height = img.height();
    int newWidth = int(width * scal);
    int newHeight = int(height * scal);
    double sw = (height * 1.0) / newHeight ,sh = (width * 1.0) / newWidth;//边长比
    QImage *scalImg = new QImage(newWidth , newHeight , QImage::Format_Grayscale8);
    for(int i = 0; i < newWidth; i++)
    {
       for(int j = 0; j < newHeight; j++)
       {
            double p_x = i * sw + 0.5 * (sw-1);//找到在原图中的坐标位置
            double p_y = j * sh + 0.5 * (sh-1);
            int in_x = int(p_x);
            int in_y = int(p_y);
            double u = p_x - int(p_x);//小数部分
            double v = p_y - int(p_y);
            rgbVal_1 = img.pixel(in_x ,in_y);
            rgbVal_2 = img.pixel(in_x+1 ,in_y);
            rgbVal_3 = img.pixel(in_x ,in_y+1);
            rgbVal_4 = img.pixel(in_x+1 ,in_y+1);

            grayVal_1 = qGray(rgbVal_1);
            grayVal_2 = qGray(rgbVal_2);
            grayVal_3 = qGray(rgbVal_3);
            grayVal_4 = qGray(rgbVal_4);
            //f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1)
            int grayVal = int((1-u)*(1-v)*grayVal_1 + (1-u)*v*grayVal_2 + u*(1-v)*grayVal_3 + u*v*grayVal_4);
            if(grayVal < 0)
                grayVal = 0;
            if(grayVal > 255)
                grayVal = 255;
            scalImg->setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
            datay[grayVal] = datay.at(grayVal) + 1;
            tot += grayVal;
            tot2 += grayVal * grayVal;
       }
    }
        QString w = QString::number(newWidth);
        QString h = QString::number(newHeight);
        QString pixel = w+"*"+h;
        ui->tableWidget->setItem(3,0,new QTableWidgetItem(pixel));
        setHistogram( tot,tot2, newWidth * newHeight, datay);
        ui->label->setPixmap(QPixmap::fromImage(scalImg->scaled(scalImg->size())));
        nowImg = scalImg->copy();
}

void MainWindow::on_pushButton_10_clicked()//垂直移动
{
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    QString str = ui->lineEdit_6->text();
    int dis =str.toInt();
    QImage *moveImg  = new QImage(width , height , QImage::Format_Grayscale8);
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {
           int j1 = j + dis;
           if(j1 >= height)
               grayVal = 255;
           else if(j1 < 0)
               grayVal = 255;
           else
               grayVal = qGray(img.pixel(i, j1));
            moveImg->setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
       }
    }
    ui->label->setPixmap(QPixmap::fromImage(*moveImg));
}

void MainWindow::on_pushButton_9_clicked()//水平移动
{
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    QString str = ui->lineEdit_7->text();
    int dis =str.toInt();
    QImage *moveImg  = new QImage(width , height , QImage::Format_Grayscale8);
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {
           int i1 = i + dis;
           if(i1 >= width)
               grayVal = 255;
           else if(i1 < 0)
               grayVal = 255;
           else
               grayVal = qGray(img.pixel(i1, j));
            moveImg->setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
       }
    }
    ui->label->setPixmap(QPixmap::fromImage(*moveImg));
}

void fft2Image(cv::InputArray _src, cv::OutputArray _dst)
{
    //得到Mat类型
    cv::Mat src = _src.getMat();
    //判断位深
    CV_Assert(src.type() == CV_32FC1 || src.type() == CV_64FC1);
    CV_Assert(src.channels() == 1 || src.channels() == 2);
    int rows = src.rows;
    int cols = src.cols;
    //为了进行快速的傅里叶变换，我们经行和列的扩充,找到最合适扩充值
    cv::Mat padded;
    int rPadded = cv::getOptimalDFTSize(rows);
    int cPadded = cv::getOptimalDFTSize(cols);
    //进行边缘扩充,扩充值为零
    copyMakeBorder(src, padded, 0, rPadded - rows, 0, cPadded - cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    //给计算出来的结果分配空间
    cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F) }; //将图像转换成浮点类型
    cv::Mat complexI;
    merge(planes, 2, complexI);         //为延扩后的图像增添一个初始化为0的通道

    dft(complexI, complexI);            //进行离散傅立叶变换. 支持图像原地计算 (输入输出为同一图像):
    dft(padded, _dst, cv::DFT_COMPLEX_OUTPUT); //返回结果进行逆变换（双通道：用于存储实部 和 虚部）；

    // 将复数转换为幅度
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    cv::Mat magI = planes[0];

    magI += cv::Scalar::all(1);                    // 切换到对数尺度
    log(magI, magI);

    //剪切和重分布幅度图象限.
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // 重新排列傅立叶图像的象限，使原点位于图像中心
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // 左上角 - 每个象限创建一个ROI
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // 右上
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // 左下方
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // 右下

    cv::Mat tmp;                           // 交换象限（左上角和右上角）
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // 交换象限（右上角与左下角）
    q2.copyTo(q1);
    tmp.copyTo(q2);

    //归一化
    normalize(magI, magI, 0, 1, CV_MINMAX); // 使用浮点值将矩阵转换为可视图像形式（在值0和1之间浮动）。

    namedWindow("DFT结果", cv::WINDOW_AUTOSIZE);
    imshow("DFT结果", magI);
}

void MainWindow::FFT()//傅里叶变换
{
    cv::Mat img = mat.clone();
    //数据类型转换：转换为浮点型

    cv::Mat fImg;
    img.convertTo(fImg, CV_64FC1);
    //傅里叶变换
    cv::Mat dft1;
    fft2Image(fImg, dft1);

    //傅里叶逆变换
    cv::Mat image;
    cv::dft(dft1, image, cv::DFT_INVERSE + cv::DFT_REAL_OUTPUT + cv::DFT_SCALE);

    //裁剪傅里叶逆变换
    image = image(cv::Rect(0, 0, img.cols, img.rows));
    image.convertTo(image, CV_8UC1);

    namedWindow("逆变换图", cv::WINDOW_AUTOSIZE);
    imshow("逆变换图", image);
}
void MainWindow::DCT()//离散余弦变换
{
    cv::Mat dctImg;
    cv::resize(mat, dctImg, cv::Size(512, 512));//必须改不然尺寸不低会报错
    dctImg.convertTo(dctImg, CV_32F, 1.0/255);
    cv::Mat srcDCT;
    dct(dctImg, srcDCT);
    imshow("dct", srcDCT);
}

void MainWindow::WT()//小波变换
{
    cv::Mat Img = mat.clone();
    int height = Img.cols;
    int width = Img.rows;
    int depth = 3, cnt = 1;//分解深度，深度计数
    cv::Mat tmp = cv::Mat::ones(width, height, CV_32FC1);//ones代表的第一个通道为1，其余两个通道为0
    cv::Mat wavelet = cv::Mat::ones(width, height, CV_32FC1);
    cv::Mat imgtmp = Img.clone();
    imgtmp.convertTo(imgtmp, CV_32FC1);
    while(cnt < depth)
    {
        width = Img.rows / cnt;
        height = Img.cols / cnt;
        for (int i = 0;i < width;i++)
        {
            for (int j=0;j < height/2 ;j++)
            {
                tmp.at<float>(i, j) = (imgtmp.at<float>(i, 2 * j) + imgtmp.at<float>(i, 2 * j + 1)) / 2;
                tmp.at<float>(i, j + height / 2) = (imgtmp.at<float>(i, 2 * j) - imgtmp.at<float>(i, 2 * j + 1)) / 2;
            }
        }
        for (int i = 0; i < width / 2; i++)
        {
            for (int j = 0; j < height; j++)
            {
                wavelet.at<float>(i, j) = (tmp.at<float>(2 * i, j) + tmp.at<float>(2 * i + 1, j)) / 2;
                wavelet.at<float>(i + width / 2, j) = (tmp.at<float>(2 * i, j) - tmp.at<float>(2 * i + 1, j)) / 2;
            }
        }
       imgtmp = wavelet;
       cnt++;
    }
    wavelet.convertTo(wavelet, CV_8UC1);
    wavelet += 60;//提升亮度不然太暗
    imshow("wt", wavelet);

}
void MainWindow::aveSmooth()//均值平滑
{
    //3*3邻域里面像素得平均值
    int grayVal = 0,size = 3 / 2; //3*3矩阵
    int width = img.width();
    int height = img.height();
    QImage smoothImg = grayImage.copy();
    QVector<double> datay;
    long long tot = 0,tot2 = 0;
    ui->widget->clearGraphs();
    datay.resize(256);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if ((i - size >= 0) && (j - size) >= 0 && (i + size) < width && (j + size)<height)//边界判断
            {
                int num = 0;
                float sum = 0;
                for (int m = i - size; m < i + size + 1; m++)
                {
                    for (int n = j - size; n < j + size + 1; n++)
                    {
                        grayVal = qGray(img.pixel(m, n));
                        num++;
                        sum += grayVal;
                    }
                }
                int val = int(sum / num);
                smoothImg.setPixel(i, j, QColor( val,val,val).rgb());
                datay[val] = datay.at(val) + 1;
                tot += val;
                tot2 += val * val;
            }
            else
            {
                grayVal = qGray(img.pixel(i, j));
                smoothImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
                datay[grayVal] = datay.at(grayVal) + 1;
                tot += grayVal;
                tot2 += grayVal * grayVal;
            }
        }
    }
    setHistogram( tot, tot2, width * height, datay);
    ui->label->setPixmap(QPixmap::fromImage(smoothImg.scaled(smoothImg.size())));
    nowImg = smoothImg.copy();
}
void MainWindow::midSmooth()//中值平滑
{
    //3*3矩阵里面像素灰度值得中值
    int grayVal = 0,size = 3 / 2; //3*3矩阵
    int width = img.width();
    int height = img.height();
    QImage smoothImg = grayImage.copy();
    QVector<double> datay;
    long long tot = 0,tot2 = 0;
    ui->widget->clearGraphs();
    datay.resize(256);
    int num[10]={0};
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if ((i - size >= 0) && (j - size) >= 0 && (i + size) < width && (j + size)<height)//边界判断
            {
                int k = 0;
                int val;
                for (int m = i - size; m < i + size + 1; m++)
                {
                    for (int n = j - size; n < j + size + 1; n++)
                    {
                        grayVal = qGray(img.pixel(m, n));
                        num[k++] = grayVal;
                    }
                }
                std::sort(num, num+k);
                if (k % 2)
                   val = num[k / 2];
                else
                    val = (num[k / 2] + num[k / 2 - 1]) / 2;
                smoothImg.setPixel(i, j, QColor( val,val,val).rgb());
                datay[val] = datay.at(val) + 1;
                tot += val;
                tot2 += val * val;
            }
            else
            {
                grayVal = qGray(img.pixel(i, j));
                smoothImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
                datay[grayVal] = datay.at(grayVal) + 1;
                tot += grayVal;
                tot2 += grayVal * grayVal;
            }
        }
    }
    setHistogram( tot, tot2, width * height, datay);
    ui->label->setPixmap(QPixmap::fromImage(smoothImg.scaled(smoothImg.size())));
    nowImg = smoothImg.copy();
}
void MainWindow::gausSmooth()//高斯平滑
{
    //按高斯模板，按一定的权重比求出3*3领域的平均值
    int grayVal = 0,size = 3 / 2; //3*3矩阵
    int width = img.width();
    int height = img.height();
    QImage smoothImg = grayImage.copy();
    QVector<double> datay;
    long long tot = 0,tot2 = 0;
    ui->widget->clearGraphs();
    datay.resize(256);
    Eigen::MatrixXf GauMat(3, 3);//计算高斯核
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            GauMat(i, j) = std::exp(-(std::pow((i - size), 2) + std::pow((j - size), 2)) / (2 * std::pow(0.8, 2)));
            //ksum += GauMat(i, j);
        }
    }
    GauMat /= GauMat.sum();
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if ((i - size >= 0) && (j - size) >= 0 && (i + size) < width && (j + size)<height)//边界判断
            {
                int val;
                Eigen::MatrixXf subGau = Eigen::MatrixXf::Zero(3, 3);
                float sumDis = 0;
                for (int m = i - size, gi = 0; m < i + size + 1; m++, gi++)
                {
                    for (int n = j - size, gj = 0; n < j + size + 1; n++, gj++)
                    {
                        subGau(gi, gj) = GauMat(gi, gj);
                        sumDis += qGray(img.pixel(m, n)) * GauMat(gi, gj);
                    }
                }
                val = int(sumDis / (subGau.sum()));
                smoothImg.setPixel(i, j, QColor( val,val,val).rgb());
                datay[val] = datay.at(val) + 1;
                tot += val;
                tot2 += val * val;
            }
            else
            {
                grayVal = qGray(img.pixel(i, j));
                smoothImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
                datay[grayVal] = datay.at(grayVal) + 1;
                tot += grayVal;
                tot2 += grayVal * grayVal;
            }
        }
    }
    setHistogram( tot, tot2, width * height, datay);
    ui->label->setPixmap(QPixmap::fromImage(smoothImg.scaled(smoothImg.size())));
    nowImg = smoothImg.copy();
}
void MainWindow::LaplaceSharpe()//拉普拉斯图像锐化
{
    int width = img.width();
    int height = img.height();
    QImage sharpeImg = grayImage.copy();
    int grayVal;
    int Template3[3][3] = { {0, -1, 0}, {-1, 4, -1}, {0, -1, 0} };//laplace锐化模板,4邻域
    int Template4[3][3] = { {-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1} };//laplace锐化模板,8邻域
    int sizeKernel = 3;
    int sumKernel = 3;
    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            QColor color;
            for (int m = - 1; m <=  1; m++)
            {
                for (int n =  - 1; n <= 1; n++)
                {
                    color = QColor(img.pixel(m+i,n+j));
                    r+=color.red()*Template4[sizeKernel/2+m][sizeKernel/2+n];
                    g+=color.green()*Template4[sizeKernel/2+m][sizeKernel/2+n];
                    b+=color.blue()*Template4[sizeKernel/2+m][sizeKernel/2+n];
                }
            }
            r = r/sumKernel+ color.red();
            g = g/sumKernel+ color.green();
            b = b/sumKernel + color.blue();
            r = qBound(0,r,255) ;
            g = qBound(0,g,255) ;
            b = qBound(0,b,255) ;
            grayVal = qGray(r,g,b);
            sharpeImg.setPixel(i, j, QColor( grayVal,grayVal,grayVal).rgb());
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(sharpeImg.scaled(sharpeImg.size())));
    nowImg = sharpeImg.copy();
}
void MainWindow::sobelSharpe()//sobel图像锐化
{
    int width = img.width();
    int height = img.height();
    QImage sharpeImg = grayImage.copy();
    int grayVal;
    int Template3[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};//Sobel锐化模板,4邻域
    int Template4[3][3] = { {-1 ,0 ,1} ,{-2 ,0 ,2} ,{-1 ,0 ,1}};//Sobel锐化模板,8邻域
    int sizeKernel = 3;
    int sumKernel = 3;
    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            QColor color;
            for (int m = - 1; m <=  1; m++)
            {
                for (int n =  - 1; n <= 1; n++)
                {
                    color = QColor(img.pixel(m+i,n+j));
                    r+=color.red()*Template4[sizeKernel/2+m][sizeKernel/2+n];
                    g+=color.green()*Template4[sizeKernel/2+m][sizeKernel/2+n];
                    b+=color.blue()*Template4[sizeKernel/2+m][sizeKernel/2+n];
                }
            }
            r = r/sumKernel+ color.red();
            g = g/sumKernel+ color.green();
            b = b/sumKernel + color.blue();
            r = qBound(0,r,255) ;
            g = qBound(0,g,255) ;
            b = qBound(0,b,255) ;
            grayVal = qGray(r,g,b);
            sharpeImg.setPixel(i, j, QColor( grayVal,grayVal,grayVal).rgb());
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(sharpeImg.scaled(sharpeImg.size())));
    nowImg = sharpeImg.copy();
}
void MainWindow::PrewittSharpe()//Prewitt图像锐化
{
    int width = img.width();
    int height = img.height();
    QImage sharpeImg = grayImage.copy();
    int grayVal;
    int Template3[3][3] = {{1, 1, 1}, {0, 0, 0}, {-1, -1, -1}};//Sobel锐化模板,4邻域
    int Template4[3][3] = { {-1 ,0 ,1} ,{-1 ,0 ,1} ,{-1 ,0 ,1} };//Sobel锐化模板,8邻域
    int sizeKernel = 3;
    int sumKernel = 3;
    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            QColor color;
            for (int m = - 1; m <=  1; m++)
            {
                for (int n =  - 1; n <= 1; n++)
                {
                    color = QColor(img.pixel(m+i,n+j));
                    r+=color.red()*Template3[sizeKernel/2+m][sizeKernel/2+n];
                    g+=color.green()*Template3[sizeKernel/2+m][sizeKernel/2+n];
                    b+=color.blue()*Template3[sizeKernel/2+m][sizeKernel/2+n];
                }
            }
            r = r/sumKernel+ color.red();
            g = g/sumKernel+ color.green();
            b = b/sumKernel + color.blue();
            r = qBound(0,r,255) ;
            g = qBound(0,g,255) ;
            b = qBound(0,b,255) ;
            grayVal = qGray(r,g,b);
            sharpeImg.setPixel(i, j, QColor( grayVal,grayVal,grayVal).rgb());
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(sharpeImg.scaled(sharpeImg.size())));
    nowImg = sharpeImg.copy();
}
void MainWindow::on_pushButton_6_clicked()//模板卷积只能输入1,1,1,0,0,0,-1,-1,-1这样的格式
{
    //将输入的字符串转化为二维数组
    QString str = ui->lineEdit_8->text();
    int tag = 0;
    for (int i=0; i <= str.length() ;i++)
    {
        if(str[i] == ',')
            tag++;
    }
    int sizeKernel = int(sqrt(tag));
    int sumKernel = int(sqrt(tag));
    int Template[5][5] = {{0}};
    int j = 0,k = 0;
    for (int i=0 ; i < tag ;i++)
    {
        QString tmp = str.section(",", i, i);
        Template[j][k++] = tmp.toInt();
        if( k == sizeKernel)
        {
            j++;k = 0;
        }
    }
    //模板卷积
    int width = img.width();
    int height = img.height();
    QImage sharpeImg = grayImage.copy();
    int grayVal;
    for (int i = sumKernel/2; i < width - sumKernel/2; i++)
    {
        for (int j = sumKernel/2; j < height - sumKernel/2; j++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            QColor color;
            for (int m = - 1; m <=  1; m++)
            {
                for (int n =  - 1; n <= 1; n++)
                {
                    color = QColor(img.pixel(m+i,n+j));
                    r+=color.red()*Template[sizeKernel/2+m][sizeKernel/2+n];
                    g+=color.green()*Template[sizeKernel/2+m][sizeKernel/2+n];
                    b+=color.blue()*Template[sizeKernel/2+m][sizeKernel/2+n];
                }
            }
            r = r/sumKernel+ color.red();
            g = g/sumKernel+ color.green();
            b = b/sumKernel + color.blue();
            r = qBound(0,r,255) ;
            g = qBound(0,g,255) ;
            b = qBound(0,b,255) ;
            grayVal = qGray(r,g,b);
            sharpeImg.setPixel(i, j, QColor( grayVal,grayVal,grayVal).rgb());
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(sharpeImg.scaled(sharpeImg.size())));
    nowImg = sharpeImg.copy();
}

int QColorToInt(const QColor &color)//返回一个0到4095的值
{
    //return   (int)(((unsigned int)color.red()<< 16) | (unsigned short)(((unsigned short)color.green()<< 8) | color.blue()));
    int Blue=(int)(color.blue() & 0xf0);//各取最高4位
    int Green=(int)(color.green() & 0xf0);
    int Red=(int)(color.red()  & 0xf0);
    //拼成一个12位整数
    int ClrIndex=(Blue<<4) + Green +(Red >>4);
    return ClrIndex;
}
QColor IntToQColor(const int &intColor)
{
    int blue = intColor & 255;
    int green = intColor >> 8 & 255;
    int red = intColor >> 16 & 255;
    return QColor(red, green, blue);
}
void MainWindow::bit24ToColor256()//转为256色
{
    int width = img.width();
    int height = img.height();
    QColor color;
    int intColor;
    int pallet[4096] = {0}, colorIndex[4096];//准备一个长度为4096的数组，代表4096种颜色
    int tag = 0;//使用颜色的数量
    for(int i = 0; i < width;i++ )//统计颜色频率。对图中的每一个象素，取R、G、B的最高四位，拼成一个12位的整数，对应的数组元素加1。
    {
        for (int j = 0;j < height; j++)
        {
            color = QColor(img.pixel(i,j));
            intColor = QColorToInt(color);
            pallet[intColor]++;
        }
    }
    //可能有一些颜色一次也没用到，即对应的数组元素为零(假设不为零的数组元素共有PalCounts个)。将这些为零的数组元素清除出去，使得前PalCounts个元素都不为零。
    for (int i=0;i<4096;i++)//去除0的颜色
    {
        if(pallet[i]!=0)
        {
            colorIndex[i]=i;
            tag++;
        }
    }
    //将颜色个数按从大到小的顺序排列(这里我们使用起泡排序)。
    for (int i = 1;i < 4096-1 ;i++)
    {
        // 内层for循环控制相邻的两个元素进行比较
        for(int j = 0;j < 4096-i;j++)//冒泡排序
        {
           if(pallet[j] < pallet[j+1])
           {
               int temp = pallet[j];
               pallet[j]= pallet[j+1];
               pallet[j+1] = temp;
               temp = colorIndex[j];
               colorIndex[j] = colorIndex[j+1];
               colorIndex[j+1] = temp;
            }
        }
    }
    struct color{
        int red;
        int blue;
        int green;
    };
    struct color colors[256];
    for(int i = 0;i<256;i++)//记录前256种颜色的rgb值
    {
        int Red=((colorIndex[i] & 0x00f) << 4);
        int Green=((colorIndex[i] & 0x0f0));
        int Blue=((colorIndex[i] & 0xf00) >> 4);
        colors[i].red = Red;
        colors[i].green = Green;
        colors[i].blue = Blue;
        pallet[i] = i;//pallet作为颜色记数的作用已经完成了，下面的作用是记录12位索引值对应的调色板中的索引值
    }
    //对于剩下的PalCounts-256种颜色并不是简单地丢弃，而是用前256种颜色中的一种来代替，代替的原则是找有最小平方误差的那个。
    //再次对图中的每一个象素，取R、G、B的最高四位，拼成一个12位的整数，如果对应值在前256种颜色中，则直接将该索引值填入位图数据中
    //，如果是在后PalCounts-256种颜色中，则用代替色的索引值填入位图数据中
    if(tag > 256)//其余的颜色依据最小平方误差近似为前256中最接近的一种
    {
        for(int i =256 ;i < tag;i++ )
        {   int index = 0;
            long ColorError1 = 1000000000;//ColorError1记录最小平方误差，一开始赋一个很大的值
            int Red=((colorIndex[i] & 0x00f) << 4);
            int Green=((colorIndex[i] & 0x0f0));
            int Blue=((colorIndex[i] & 0xf00) >> 4);
            for (int j = 0 ; j < 256 ; j++)
            {
                long ColorError2 = (Blue - colors[j].blue)^2 + (Red - colors[j].red)^2 + (Green - colors[j].green)^2;
                if (ColorError2 < ColorError1)
                    { //找到更小的了
                    ColorError1 = ColorError2;
                    index = j; //记录对应的调色板的索引值
                }
            }
            pallet[i] = index;
        }
    }
    QImage Image256 = img.copy();
    for(int i = 0; i < width;i++ )//统计颜色频率
    {
        for (int j = 0;j < height; j++)
        {
            color = QColor(img.pixel(i,j));
            intColor = QColorToInt(color);
            for(int k = 0; k < tag; k++)
            {
                if(intColor == colorIndex[k])
                {
                    Image256.setPixel(i, j, QColor(colors[pallet[k]].red, colors[pallet[k]].green, colors[pallet[k]].blue).rgb());

                }
            }
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(Image256.scaled(Image256.size())));
}
void MainWindow::SobelEdge()//sobel算子边缘检测
{
    QImage edgeImg = grayImage.copy();
    int width = img.width();
    int height = img.height();
    int TemplateX[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};//模板
    int TemplateY[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    //Gx和Gy分别是在横向及纵向的灰度偏导的近似值。（
    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {
            double Gx = TemplateX[0][0] * qGray(img.pixel(i-1,j-1))
                        + TemplateX[1][0] * qGray(img.pixel(i,j-1))
                        + TemplateX[2][0] * qGray(img.pixel(i+1,j-1))
                        + TemplateX[0][1] * qGray(img.pixel(i-1,j))
                        + TemplateX[1][1] * qGray(img.pixel(i,j))
                        + TemplateX[2][1] * qGray(img.pixel(i+1,j))
                        + TemplateX[0][2] * qGray(img.pixel(i-1,j+1))
                        + TemplateX[1][2] * qGray(img.pixel(i,j+1))
                        + TemplateX[2][2] * qGray(img.pixel(i+1,j+1));
            double Gy = TemplateY[0][0] * qGray(img.pixel(i-1,j-1))
                        + TemplateY[0][1] * qGray(img.pixel(i-1,j))
                        + TemplateY[0][2] * qGray(img.pixel(i-1,j+1))
                        + TemplateY[1][0] * qGray(img.pixel(i,j-1))
                        + TemplateY[1][1] * qGray(img.pixel(i,j))
                        + TemplateY[1][2] * qGray(img.pixel(i,j+1))
                        + TemplateY[2][0] * qGray(img.pixel(i+1,j-1))
                        + TemplateY[2][1] * qGray(img.pixel(i+1,j))
                        + TemplateY[2][2] * qGray(img.pixel(i+1,j+1));
            double G = sqrt(Gx*Gx+Gy*Gy);

            if(G > aveGray)
            {
                int grayVal = 255;
                edgeImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
            }
            else
            {
                int grayVal = 0;
                edgeImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
            }
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(edgeImg.scaled(edgeImg.size())));
    nowImg = edgeImg.copy();
}
void MainWindow::LaplaceEdge()//Laplace算子边缘检测
{
    QImage edgeImg = grayImage.copy();
    int width = img.width();
    int height = img.height();
    int TemplateX[3][3] = {{0, 1, 0}, {1, -4, 1}, {0, 1, 0}};
    int TemplateY[3][3] = {{1, 1, 1}, {1, -8, 1}, {1, 1, 1}};
    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {
            double G = TemplateX[0][0] * qGray(img.pixel(i-1,j-1))//模板卷积
                        + TemplateX[1][0] * qGray(img.pixel(i,j-1))
                        + TemplateX[2][0] * qGray(img.pixel(i+1,j-1))
                        + TemplateX[0][1] * qGray(img.pixel(i-1,j))
                        + TemplateX[1][1] * qGray(img.pixel(i,j))
                        + TemplateX[2][1] * qGray(img.pixel(i+1,j))
                        + TemplateX[0][2] * qGray(img.pixel(i-1,j+1))
                        + TemplateX[1][2] * qGray(img.pixel(i,j+1))
                        + TemplateX[2][2] * qGray(img.pixel(i+1,j+1));
            if(G > 255)
            {
                int grayVal = 255;
                edgeImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
            }
            else if(G < 0)
            {
                int grayVal = 0;
                edgeImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
            }
            else
            {
                int grayVal = int(G);
                edgeImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
            }
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(edgeImg.scaled(edgeImg.size())));
    nowImg = edgeImg.copy();
}
void MainWindow::PrewittEdge()//Prewitt算子边缘检测
{
    QImage edgeImg = grayImage.copy();
    int width = img.width();
    int height = img.height();
    int TemplateY[3][3] = {{1, 1, 1} ,{0, 0, 0} ,{-1, -1, -1}};
    int TemplateX[3][3] = {{-1 ,0 ,1} ,{-1 ,0 ,1} ,{-1 ,0 ,1}};
    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {
            double G = TemplateX[0][0] * qGray(img.pixel(i-1,j-1))
                        + TemplateX[1][0] * qGray(img.pixel(i,j-1))
                        + TemplateX[2][0] * qGray(img.pixel(i+1,j-1))
                        + TemplateX[0][1] * qGray(img.pixel(i-1,j))
                        + TemplateX[1][1] * qGray(img.pixel(i,j))
                        + TemplateX[2][1] * qGray(img.pixel(i+1,j))
                        + TemplateX[0][2] * qGray(img.pixel(i-1,j+1))
                        + TemplateX[1][2] * qGray(img.pixel(i,j+1))
                        + TemplateX[2][2] * qGray(img.pixel(i+1,j+1));
            if(G > 255)
            {
                int grayVal = 255;
                edgeImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
            }
            else if(G < 0)
            {
                int grayVal = 0;
                edgeImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
            }
            else
            {
                int grayVal = int(G);
                edgeImg.setPixel(i, j, QColor(grayVal, grayVal, grayVal).rgb());
            }
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(edgeImg.scaled(edgeImg.size())));
    nowImg = edgeImg.copy();
}
void MainWindow::HoughTransform()
{
    int width = img.width();
    int height = img.height();
    double sin_value[180];
    double cos_value[180];
    int thrVal = int(aveGray);
    int size = sqrt(width * width + height * height);
    QImage HougthImg = grayImage.copy();
    int hough_acc[180][size];//存放直线
    memset(hough_acc ,0 ,sizeof(hough_acc));
    for(int i = 0; i < 180; i++)//为避免hough变换程序中多次计算三角函数值，可以采用先用数组存储每个可能的三角函数值，再用查找表的方法查找对应三角函数值，从而提高效率。
    {
        sin_value[i] = sin(i*3.1415926/180);
        cos_value[i] = cos(i*3.1415926/180);
    }
    for (int i = 0; i < width ; i++)
    {
        for (int j = 0; j < height ; j++)
        {
            int grayVal = qGray(img.pixel(i, j));
            if(grayVal < 100)//是黑色的点的话将所有计算器加一
            {
                for(int k = 0; k < 180; k++)
                {
                    int tp = (int)(i * sin_value[k] + j * cos_value[k]);
                    hough_acc[k][tp]++;
                }
            }
        }
    }
    for (int i = 0; i < width ; i++)
    {
        for (int j = 0; j < height ; j++)
        {
            for(int k = 0; k < 180; k++)
            {
                int tp = (int)(i * sin_value[k] + j * cos_value[k]);
                if(hough_acc[k][tp] > 100)//如果直线数量大于100
                {
                    HougthImg.setPixel(i, j, QColor(132, 223, 12).rgb());
                    break;
                }
            }
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(HougthImg));
//    int maxCnt = 0;
//    int maxTheta;
//    int maxP;
//    for (int i = 0; i < 180; i++)
//    {
//        for (int j = 0; j < maxd; j++)
//        {
//            if (hough_acc[i][j] > maxCnt)
//            {
//                maxCnt = hough_acc[i][j];
//                maxTheta = i;
//                maxP = j;
//            }
//        }
//    }

//    cv::Point pt1, pt2;
//    double a = cos(maxTheta * 3.1415926/180), b = sin(maxTheta*3.1415926/180);
//    double x0 = a * maxP, y0 = b * maxP;
//    pt1.x = cvRound(x0 + 1000 * (-b));
//    pt1.y = cvRound(y0 + 1000 * (a));
//    pt2.x = cvRound(x0 - 1000 * (-b));
//    pt2.y = cvRound(y0 - 1000 * (a));
//    cv::Mat Img = mat.clone();
//    cv::line(Img, pt1, pt2, cv::Scalar(55, 100, 195), 1, 0);
//    //cv::line(Img,pt1,pt2,cv::Scalar(0,0,255),5,8);
//    imshow("HoughTransform",Img);
//-------------------------------------------------------------//

//    cv::Mat temp ,dstImg;
//    Canny(mat, temp, 50, 200, 3);
//    cvtColor(temp, dstImg, CV_GRAY2BGR);
//    vector<cv::Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
//    HoughLines(temp, lines, 1, CV_PI / 180, 150, 0, 0);
//    for (size_t i = 0; i < lines.size(); i++)
//    {
//        float rho = lines[i][0], theta = lines[i][1];
//        cv::Point pt1, pt2;
//        double a = cos(theta), b = sin(theta);
//        double x0 = a*rho, y0 = b*rho;
//        pt1.x = cvRound(x0 + 1000 * (-b));
//        pt1.y = cvRound(y0 + 1000 * (a));
//        pt2.x = cvRound(x0 - 1000 * (-b));
//        pt2.y = cvRound(y0 - 1000 * (a));
//        line(dstImg, pt1, pt2, cv::Scalar(55, 100, 195), 1, 0);
//    }
//    imshow("3", dstImg);

}
void MainWindow::RLE()//游程编码
{
    int width = img.width();
    int height = img.height();
    QImage rleImg = img.copy();
    int I1[300][300];//I1记录灰度值开太大QT会卡死
    int I2[300][300];//I2记录重复次数
    int I3[300];//I3记录每一列压缩后有多少位
    QString filename = QFileDialog::getSaveFileName(this,tr("新建txt"),"",tr("文本 (*.txt)"));
    //字符数组大小
    QFile file(filename);
    QString str = "";
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::information(this, "Error Message", "Please Select a Text File!");
        return;
    }
    for(int i=0; i < 300;i++)
    {
        for(int j=0; j< 300 ;j++)
        {
            I1[i][j] = I2[i][j] = 0;
        }
    }
    for(int j=0; j< 300 ;j++)
    {
        I3[j] = 0;
    }
    int k = 0;
    for (int i = 0; i < width ; i++)
    {
        int num = 1;
        for (int j = 0; j < height ; j++)
        {
           if(j == height - 1)//因为换行所以每列最后一个单独考虑
            {
                int grayVal = qGray(img.pixel(i,j));
                I1[i][k] = grayVal;
                I2[i][k] = num;
                k++;
                I3[i]++;
            }
            else
            {   int a = qGray(img.pixel(i,j));
                int b = qGray(img.pixel(i,j+1));
                if(a == b) //相同的话
                    num++;
                else
                {
                    I1[i][k] = qGray(img.pixel(i,j));
                    I2[i][k] = num;
                    k++;
                    num = 1;
                    I3[i]++;
                }
            }
        }
        k = 0;
    }
    //解码
   int m = 0;
    for(int i = 0 ;i < width; i++)
    {
        for (int j = 0;j < I3[i];j++)
        {
            for (int k = 0;k < I2[i][j]; k++)
            {
                rleImg.setPixel(i, m, QColor(I1[i][j], I1[i][j], I1[i][j]).rgb());
                m++;
            }
        }
        m=0;
    }
    int num = 1;
    for(int i = 0 ;i < width; i++)
    {
        int flag = 0;
        if(flag == 0)
            num = 1;
        for (int j = 0; j < height ; j++)
        {
            if(j == height - 1)
             {
                 int a = qGray(img.pixel(i,j));
                 int b = qGray(img.pixel(i+1,0));
                 if(a == b) //相同的话
                 {
                     flag = 1;
                     num++;
                 }
                 else
                 {
                     QString str1 = QString::number(a);
                     QString str2 = QString::number(num);
                     str += str1;
                     str += str2;
                     num = 1;
                     flag = 0;
                 }
             }
             else
             {
                int a = qGray(img.pixel(i,j));
                int b = qGray(img.pixel(i,j+1));
                if(a == b) //相同的话
                    num++;
                else
                {
                    QString str1 = QString::number(a);
                    QString str2 = QString::number(num);
                    str += str1;
                    str += str2;
                    num = 1;
                }
            }
        }
    }
      file.open(QIODevice::WriteOnly);
      file.write(str.toUtf8());
      file.close();
      QMessageBox::information(this, "Success", "Success");
    ui->label->setPixmap(QPixmap::fromImage(rleImg));
}
//----------------------哈夫曼变换
struct HfNode{
    int weight;
    int data;
    int parent;
    int left;
    int right;
};
HfNode* CreateHfTree(const QVector<int> pData)
{
    int n = 0;
    for(int i = 0; i < 256; i++)//统计不为0的数量
           if(pData[i] != 0)
               n++;
    int MAXLEN = n*2 - 1;
    HfNode *pTree = new HfNode[MAXLEN];//2n-1个节点

    for(int i = 0, j = 0; i < 256; i++)
    {
        if(pData[i] != 0)//初始化
        {
            pTree[j].weight = pData[i];
            pTree[j].data = i;
            pTree[j].parent = -1;
            pTree[j].left = -1;
            pTree[j].right = -1;
            j++;
        }
    }
    if(n == 1)
        return pTree;

    int min1=0;//权值最小节点下标
    int min2=0;//权值次小节点下标
    for(; n < MAXLEN; n++)
    {
        //找最小节点
        for(int i = 0; i < n; i++)
            if(pTree[i].parent == -1)
            {
                min1 = i;
                break;
            }
        for(int i = 0; i < n; i++)
            if(pTree[i].weight < pTree[min1].weight && pTree[i].parent == -1)
                min1 = i;

        //找次小节点
        for(int i = 0; i < n; i++)
            if(pTree[i].parent == -1 && i != min1)
                min2 = i;
        for(int i = 0; i < n; i++)
        {
            if(i == min1)
                continue;
            else if(pTree[i].weight < pTree[min2].weight && pTree[i].parent == -1)
                min2 = i;
        }

        //将最小节点与次小节点合并为一个新的节点并指明着三个节点的父子关系
        pTree[n].data = n;
        pTree[n].weight = pTree[min1].weight + pTree[min2].weight;
        pTree[n].left = min1;
        pTree[n].right = min2;
        pTree[n].parent = -1;
        pTree[min1].parent = n;
        pTree[min2].parent = n;
    }
    return pTree;
}
struct HfCode
{
  int *code;
  int data;
  int index;
};
HfCode* CreateHfCode(const HfNode *pTree,const QVector<int> pData)//编码函数 顺序表pTree是哈夫曼树的首地址  pData是统计数据出现次数的数组
{
    int n = 0;
    for(int i = 0; i < 256; i++)//统计不为0的数量
           if(pData[i] != 0)
               n++;
    HfCode *pCode = new HfCode[n];
    for(int i = 0; i < n; i++)
    {
        int parent = pTree[i].parent;
        pCode[i].data = pTree[i].data;
        pCode[i].code = new int[n];
        for(int j = 0; j < n; j++)//编码全部初始化为-1
            pCode[i].code[j] = -1;

        int j = 0;
        int child = i;
        while(parent != -1)
        {
            if(pTree[parent].left == child)
                pCode[i].code[j] = 0;
            else if(pTree[parent].right == child)
                pCode[i].code[j] = 1;
            j++;
            child = parent;
            parent = pTree[parent].parent;
        }
        pCode[i].index = j - 1;//标记编码的开始位置（倒序）
    }
    return pCode;
}
struct StdHfCode
{
  char *code;
  int data;
  int len;
};
StdHfCode* StdCode(const HfCode* pCode, const int n)//标准化将倒序的编码转为正序存储
{
    StdHfCode *pStdCode = new StdHfCode[256];
    for(int i = 0; i < 256; i++)
        pStdCode[i].code = NULL;
    for(int i = 0; i < n; i++)
    {
        pStdCode[pCode[i].data].data = pCode[i].data;//复制data
        pStdCode[pCode[i].data].code = new char[pCode[i].index + 1];//申请code的空间
        for(int j = 0; j < pCode[i].index+1; j++)//复制code
            pStdCode[pCode[i].data].code[j] = (char)pCode[i].code[pCode[i].index - j];
        pStdCode[pCode[i].data].len = pCode[i].index + 1;
    }
    return pStdCode;
}
void Decompress(int *str,QString code,const HfNode *pTree,int n)
{
    int root=2*n-2;
    int length=0,i=0;
    int len = code.length();
    while(i < len)
    {
        if(code[i]=='0'+0)
            root = pTree[root].left;
        else if(code[i]=='0'+1)
            root = pTree[root].right;
        else
            return;
        if(pTree[root].left==-1 && pTree[root].right==-1){
            str[length++] = pTree[root].data;
            root = 2 * n - 2;
        }
        i++;
    }
    if(root==2 * n-2)
        return;
    return;
}
void MainWindow::Huffman()
{
    //压缩
    int grayVal = 0;
    int width = img.width();
    int height = img.height();
    QImage HufImg = grayImage.copy();
    QString filename = QFileDialog::getSaveFileName(this,tr("新建txt"),"",tr("文本 (*.txt)"));
    //字符数组大小
    //写入文件
    QFile file(filename);
    QString str = "";
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::information(this, "Error Message", "Please Select a Text File!");
        return;
    }
    QVector<int> pData;//计算每个像素出现的频率
    //
    pData.resize(256);
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {
            grayVal = qGray(img.pixel(i ,j));
            pData[grayVal] = pData.at(grayVal) + 1;//统计出255个像素出现频率
       }
    }
    HfNode *pTree = CreateHfTree(pData);
    HfCode *pCode = CreateHfCode(pTree, pData);
    int n = 0;
    for(int i = 0; i < 256; i++)//统计不为0的数量
           if(pData[i] != 0)
               n++;
    StdHfCode *pStdCode = StdCode(pCode, n);//编码标准化
    int TreeSize = 2*n-1;
    QString huffmanCode = NULL;
    int k = 0;
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {
            grayVal = qGray(img.pixel(i ,j));
            //统计出255个像素出现频率
            huffmanCode += pStdCode[grayVal].code;
            str += pStdCode[grayVal].code;
       }
    }
    int len = huffmanCode.length();
    int *biao = new int[width*height];
    //Decompress(biao, huffmanCode, pTree, n);
    for(int i = 0; i < width; i++)
    {
       for(int j = 0; j < height; j++)
       {
            grayVal = qGray(img.pixel(i ,j));
            //统计出255个像素出现频率
            //HufImg.setPixel(i, j, QColor(biao[i*width+j], biao[i*width+j], biao[i*width+j]).rgb());
       }
    }
    file.open(QIODevice::WriteOnly);
    file.write(str.toUtf8());
    file.close();
    QMessageBox::information(this, "Success", "Success");
    ui->label->setPixmap(QPixmap::fromImage(HufImg));
    //解压缩
}


void MainWindow::on_pushButton_11_clicked()
{
    QString str = ui->lineEdit_11->text();
    double yuzhi =str.toInt();
    int width = img.width();
    int height = img.height();
    double sin_value[180];
    double cos_value[180];
    int thrVal = int(aveGray);
    int size = sqrt(width * width + height * height);
    QImage HougthImg = grayImage.copy();
    int hough_acc[180][size];
    memset(hough_acc ,0 ,sizeof(hough_acc));
    for(int i = 0; i < 180; i++)//为避免hough变换程序中多次计算三角函数值，可以采用先用数组存储每个可能的三角函数值，再用查找表的方法查找对应三角函数值，从而提高效率。
    {
        sin_value[i] = sin(i*3.1415926/180);
        cos_value[i] = cos(i*3.1415926/180);
    }
    for (int i = 0; i < width ; i++)
    {
        for (int j = 0; j < height ; j++)
        {
            int grayVal = qGray(img.pixel(i, j));
            if(grayVal < 100)//是黑色的点的话将所有计算器加一
            {
                for(int k = 0; k < 180; k++)
                {
                    int tp = (int)(i * sin_value[k] + j * cos_value[k]);
                    hough_acc[k][tp]++;
                }
            }
        }
    }
    int num = 0;
    for(int i = 0; i < 180 ;i++)
    {
        for (int j = 0; j < size ;j++)
        {
            if(hough_acc[i][j] > yuzhi)
            {
                num++;
            }
        }
    }
    for (int i = 0; i < width ; i++)
    {
        for (int j = 0; j < height ; j++)
        {
            for(int k = 0; k < 180; k++)
            {
                int tp = (int)(i * sin_value[k] + j * cos_value[k]);
                if(hough_acc[k][tp] > yuzhi)
                {
                    HougthImg.setPixel(i, j, QColor(132, 223, 12).rgb());
                    break;
                }
            }
        }
    }
    QString str1 = QString::number(num);
    ui->label_2->setText(str1);
    ui->label->setPixmap(QPixmap::fromImage(HougthImg));
}
void MainWindow::edgeTracing()
{
    int width = img.width();
    int height = img.height();
    QImage edgeImg = img.copy();
    for(int i = 1; i < width-2; i++)
    {
       for(int j = 1; j < height-2; j++)
       {
           //如果该中心点等于255阈值,则考虑它的八邻域
           int grayVal = qGray(img.pixel(i ,j));
           if(grayVal > 86)
           {
               int num = 0;
               for(int k = -1; k <= 1; k++)
               {
                    for (int l = -1; l <= 1; l++)
                    {
                        // 如果八邻域中有灰度值大于阈值的点，则点的值加一
                        if( (k != 0)&&(l != 0)&&(qGray(img.pixel(i + k ,j + l)) > 86))
                            num += 1;
                    }
               }
               //如果八邻域中只有一个点大于阈值，说明该中心点为端点，则考虑该点的的十六邻域
               if(num == 1)
               {
                   for(int k = -2; k <= 2; k++)
                   {
                        for (int l = -2; l <= 2; l++)
                        {
                            //如果该点的十六邻域中有大于阈值的点，则该点与中心点之间的点置为255
                            if (!(k < 2 && k > -2 && l < 2 && l > -2) && (qGray(img.pixel(i + k ,j + l)) > 86))
                                edgeImg.setPixel(int(i + k / 2), int(j + l / 2), QColor(255, 255, 255).rgb());
                        }
                   }
               }
           }
       }
    }
    ui->label->setPixmap(QPixmap::fromImage(edgeImg));
}
