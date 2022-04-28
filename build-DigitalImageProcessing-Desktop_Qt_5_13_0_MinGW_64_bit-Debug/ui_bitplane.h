/********************************************************************************
** Form generated from reading UI file 'bitplane.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BITPLANE_H
#define UI_BITPLANE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_bitPlane
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;

    void setupUi(QWidget *bitPlane)
    {
        if (bitPlane->objectName().isEmpty())
            bitPlane->setObjectName(QString::fromUtf8("bitPlane"));
        bitPlane->resize(1133, 680);
        gridLayout = new QGridLayout(bitPlane);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(bitPlane);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(200, 200));
        label->setMaximumSize(QSize(150, 150));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(bitPlane);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(200, 200));
        label_2->setMaximumSize(QSize(200, 200));

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        label_3 = new QLabel(bitPlane);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(200, 200));
        label_3->setMaximumSize(QSize(200, 200));

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        label_4 = new QLabel(bitPlane);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(200, 200));
        label_4->setMaximumSize(QSize(200, 200));

        gridLayout->addWidget(label_4, 0, 3, 1, 1);

        label_5 = new QLabel(bitPlane);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(200, 200));
        label_5->setMaximumSize(QSize(200, 200));

        gridLayout->addWidget(label_5, 1, 0, 1, 1);

        label_6 = new QLabel(bitPlane);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(200, 200));
        label_6->setMaximumSize(QSize(200, 200));

        gridLayout->addWidget(label_6, 1, 1, 1, 1);

        label_7 = new QLabel(bitPlane);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(200, 200));
        label_7->setMaximumSize(QSize(200, 200));

        gridLayout->addWidget(label_7, 1, 2, 1, 1);

        label_8 = new QLabel(bitPlane);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(200, 200));
        label_8->setMaximumSize(QSize(200, 200));

        gridLayout->addWidget(label_8, 1, 3, 1, 1);


        retranslateUi(bitPlane);

        QMetaObject::connectSlotsByName(bitPlane);
    } // setupUi

    void retranslateUi(QWidget *bitPlane)
    {
        bitPlane->setWindowTitle(QCoreApplication::translate("bitPlane", "Form", nullptr));
        label->setText(QCoreApplication::translate("bitPlane", "1", nullptr));
        label_2->setText(QCoreApplication::translate("bitPlane", "2", nullptr));
        label_3->setText(QCoreApplication::translate("bitPlane", "3", nullptr));
        label_4->setText(QCoreApplication::translate("bitPlane", "4", nullptr));
        label_5->setText(QCoreApplication::translate("bitPlane", "5", nullptr));
        label_6->setText(QCoreApplication::translate("bitPlane", "6", nullptr));
        label_7->setText(QCoreApplication::translate("bitPlane", "7", nullptr));
        label_8->setText(QCoreApplication::translate("bitPlane", "8", nullptr));
    } // retranslateUi

};

namespace Ui {
    class bitPlane: public Ui_bitPlane {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BITPLANE_H
