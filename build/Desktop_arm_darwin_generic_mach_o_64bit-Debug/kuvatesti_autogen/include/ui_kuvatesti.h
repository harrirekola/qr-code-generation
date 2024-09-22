/********************************************************************************
** Form generated from reading UI file 'kuvatesti.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KUVATESTI_H
#define UI_KUVATESTI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_kuvatesti
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *kuvatesti)
    {
        if (kuvatesti->objectName().isEmpty())
            kuvatesti->setObjectName("kuvatesti");
        kuvatesti->resize(800, 600);
        centralwidget = new QWidget(kuvatesti);
        centralwidget->setObjectName("centralwidget");
        kuvatesti->setCentralWidget(centralwidget);
        menubar = new QMenuBar(kuvatesti);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        kuvatesti->setMenuBar(menubar);
        statusbar = new QStatusBar(kuvatesti);
        statusbar->setObjectName("statusbar");
        kuvatesti->setStatusBar(statusbar);

        retranslateUi(kuvatesti);

        QMetaObject::connectSlotsByName(kuvatesti);
    } // setupUi

    void retranslateUi(QMainWindow *kuvatesti)
    {
        kuvatesti->setWindowTitle(QCoreApplication::translate("kuvatesti", "kuvatesti", nullptr));
    } // retranslateUi

};

namespace Ui {
    class kuvatesti: public Ui_kuvatesti {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KUVATESTI_H
