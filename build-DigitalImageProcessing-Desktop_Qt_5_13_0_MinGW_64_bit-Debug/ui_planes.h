/********************************************************************************
** Form generated from reading UI file 'planes.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLANES_H
#define UI_PLANES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_planes
{
public:

    void setupUi(QWidget *planes)
    {
        if (planes->objectName().isEmpty())
            planes->setObjectName(QString::fromUtf8("planes"));
        planes->resize(999, 509);

        retranslateUi(planes);

        QMetaObject::connectSlotsByName(planes);
    } // setupUi

    void retranslateUi(QWidget *planes)
    {
        planes->setWindowTitle(QCoreApplication::translate("planes", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class planes: public Ui_planes {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLANES_H
