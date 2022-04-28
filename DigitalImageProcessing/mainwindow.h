#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>
#include <QScrollArea>
#include <QLabel>
#include <QDebug>
#include <qmath.h>
#include "bitplane.h"
#include "ui_bitplane.h"
#include<iostream>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\opencv.hpp>
#include "opencv2/imgproc/types_c.h"
#include <string.h>
#include <Eigen>
#include <bitset>
using namespace std;
#define PI acos(-1)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QImage img;
    QImage nowImg; //用于平移的
    cv::Mat mat;  //opencv中用于存储一开始读进来的图片
    QVector<QImage *> planImg;
    QImage grayImage;
    QVector<double> histogram;
    long long aveGray ;  //平均灰度值
    void setHistogram(long long tot,long long tot2,int xiangsu,QVector<double> datay);
    string imgPath;
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void open();

    void save();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void linearTrans();

    void linearTrans_2();

    void nonlinearTrans();

    void nonlinearTrans_2();

    void equal_orig();

    void equal_up();

    void on_pushButton_7_clicked();

    void bmp();

    void bitPlan();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void FFT();

    void DCT();

    void WT();

    void aveSmooth();

    void midSmooth();

    void gausSmooth();

    void LaplaceSharpe();

    void sobelSharpe();

    void PrewittSharpe();

    void bit24ToColor256();

    void SobelEdge();

    void LaplaceEdge();

    void PrewittEdge();

    void HoughTransform();

    void RLE();

    void Huffman();

    void on_pushButton_11_clicked();

    void edgeTracing();

private:
    Ui::MainWindow *ui;
    QString runPath;
    QString imageName;
    QString imagePath;
    bitPlane *dialog;
};

#endif // MAINWINDOW_H
