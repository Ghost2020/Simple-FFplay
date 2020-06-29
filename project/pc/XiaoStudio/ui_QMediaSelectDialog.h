/********************************************************************************
** Form generated from reading UI file 'QMediaSelectDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMEDIASELECTDIALOG_H
#define UI_QMEDIASELECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QMediaSelectDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_File;
    QGroupBox *groupBox_4;
    QListWidget *listWidget;
    QPushButton *pushButton_add;
    QPushButton *pushButton_remove;
    QGroupBox *groupBox_5;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEdit_subtitle;
    QPushButton *pushButton_browseSubtitle;
    QWidget *tab_Network;
    QGroupBox *groupBox;
    QLabel *label;
    QComboBox *comboBox_URL;
    QLabel *label_2;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QWidget *tab_CaptureDevice;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_2;
    QComboBox *comboBox_captureDevice;
    QLabel *label_3;
    QGroupBox *groupBox_3;
    QLabel *label_4;
    QLabel *label_5;
    QComboBox *comboBox_videoDeviceName;
    QComboBox *comboBox_AuddioDeviceName;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_play;
    QPushButton *pushButton_cancel;

    void setupUi(QDialog *QMediaSelectDialog)
    {
        if (QMediaSelectDialog->objectName().isEmpty())
            QMediaSelectDialog->setObjectName(QString::fromUtf8("QMediaSelectDialog"));
        QMediaSelectDialog->resize(640, 480);
        verticalLayout = new QVBoxLayout(QMediaSelectDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(QMediaSelectDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_File = new QWidget();
        tab_File->setObjectName(QString::fromUtf8("tab_File"));
        groupBox_4 = new QGroupBox(tab_File);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(30, 30, 571, 201));
        listWidget = new QListWidget(groupBox_4);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(15, 40, 431, 141));
        pushButton_add = new QPushButton(groupBox_4);
        pushButton_add->setObjectName(QString::fromUtf8("pushButton_add"));
        pushButton_add->setGeometry(QRect(460, 69, 81, 31));
        pushButton_remove = new QPushButton(groupBox_4);
        pushButton_remove->setObjectName(QString::fromUtf8("pushButton_remove"));
        pushButton_remove->setGeometry(QRect(460, 110, 80, 31));
        groupBox_5 = new QGroupBox(tab_File);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(30, 260, 571, 91));
        layoutWidget = new QWidget(groupBox_5);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 40, 531, 25));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        lineEdit_subtitle = new QLineEdit(layoutWidget);
        lineEdit_subtitle->setObjectName(QString::fromUtf8("lineEdit_subtitle"));

        horizontalLayout_3->addWidget(lineEdit_subtitle);

        pushButton_browseSubtitle = new QPushButton(layoutWidget);
        pushButton_browseSubtitle->setObjectName(QString::fromUtf8("pushButton_browseSubtitle"));

        horizontalLayout_3->addWidget(pushButton_browseSubtitle);

        tabWidget->addTab(tab_File, QString());
        tab_Network = new QWidget();
        tab_Network->setObjectName(QString::fromUtf8("tab_Network"));
        groupBox = new QGroupBox(tab_Network);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 10, 581, 271));
        groupBox->setAlignment(Qt::AlignCenter);
        groupBox->setFlat(true);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 40, 171, 16));
        comboBox_URL = new QComboBox(groupBox);
        comboBox_URL->setObjectName(QString::fromUtf8("comboBox_URL"));
        comboBox_URL->setGeometry(QRect(40, 70, 391, 22));
        comboBox_URL->setEditable(true);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 120, 311, 16));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(50, 150, 311, 16));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(50, 180, 311, 16));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(50, 210, 311, 16));
        tabWidget->addTab(tab_Network, QString());
        tab_CaptureDevice = new QWidget();
        tab_CaptureDevice->setObjectName(QString::fromUtf8("tab_CaptureDevice"));
        verticalLayout_2 = new QVBoxLayout(tab_CaptureDevice);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_2 = new QGroupBox(tab_CaptureDevice);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setAlignment(Qt::AlignCenter);
        comboBox_captureDevice = new QComboBox(groupBox_2);
        comboBox_captureDevice->setObjectName(QString::fromUtf8("comboBox_captureDevice"));
        comboBox_captureDevice->setGeometry(QRect(280, 60, 251, 22));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(60, 60, 71, 16));
        groupBox_3 = new QGroupBox(groupBox_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(60, 110, 511, 211));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(50, 50, 131, 16));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(50, 100, 121, 16));
        comboBox_videoDeviceName = new QComboBox(groupBox_3);
        comboBox_videoDeviceName->setObjectName(QString::fromUtf8("comboBox_videoDeviceName"));
        comboBox_videoDeviceName->setGeometry(QRect(220, 50, 261, 22));
        comboBox_AuddioDeviceName = new QComboBox(groupBox_3);
        comboBox_AuddioDeviceName->setObjectName(QString::fromUtf8("comboBox_AuddioDeviceName"));
        comboBox_AuddioDeviceName->setGeometry(QRect(220, 90, 261, 22));

        verticalLayout_2->addWidget(groupBox_2);

        tabWidget->addTab(tab_CaptureDevice, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(388, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_play = new QPushButton(QMediaSelectDialog);
        pushButton_play->setObjectName(QString::fromUtf8("pushButton_play"));

        horizontalLayout->addWidget(pushButton_play);

        pushButton_cancel = new QPushButton(QMediaSelectDialog);
        pushButton_cancel->setObjectName(QString::fromUtf8("pushButton_cancel"));

        horizontalLayout->addWidget(pushButton_cancel);


        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(QMediaSelectDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QMediaSelectDialog);
    } // setupUi

    void retranslateUi(QDialog *QMediaSelectDialog)
    {
        QMediaSelectDialog->setWindowTitle(QCoreApplication::translate("QMediaSelectDialog", "Dialog", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("QMediaSelectDialog", "File Selection", nullptr));
        pushButton_add->setText(QCoreApplication::translate("QMediaSelectDialog", "Add", nullptr));
        pushButton_remove->setText(QCoreApplication::translate("QMediaSelectDialog", "Remove", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("QMediaSelectDialog", "Use a subtile file", nullptr));
        pushButton_browseSubtitle->setText(QCoreApplication::translate("QMediaSelectDialog", "Browse", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_File), QCoreApplication::translate("QMediaSelectDialog", "\346\226\207\344\273\266", nullptr));
        groupBox->setTitle(QCoreApplication::translate("QMediaSelectDialog", "Network Protocol", nullptr));
        label->setText(QCoreApplication::translate("QMediaSelectDialog", "Please enter a network URL:", nullptr));
        label_2->setText(QCoreApplication::translate("QMediaSelectDialog", "\346\271\226\345\215\227\345\215\253\350\247\206 rtmp://58.200.131.2:1935/livetv/hunantv", nullptr));
        label_6->setText(QCoreApplication::translate("QMediaSelectDialog", "\345\271\277\350\245\277\345\215\253\350\247\206 \357\274\232rtmp://58.200.131.2:1935/livetv/gxtv", nullptr));
        label_7->setText(QCoreApplication::translate("QMediaSelectDialog", "\345\271\277\344\270\234\345\215\253\350\247\206\357\274\232rtmp://58.200.131.2:1935/livetv/gdtv", nullptr));
        label_8->setText(QCoreApplication::translate("QMediaSelectDialog", "\344\270\234\346\226\271\345\215\253\350\247\206\357\274\232rtmp://58.200.131.2:1935/livetv/dftv", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_Network), QCoreApplication::translate("QMediaSelectDialog", "\347\275\221\347\273\234", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("QMediaSelectDialog", "Capture Device", nullptr));
        label_3->setText(QCoreApplication::translate("QMediaSelectDialog", "Capture Mode", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("QMediaSelectDialog", "Device Selection", nullptr));
        label_4->setText(QCoreApplication::translate("QMediaSelectDialog", "Viddeo device name", nullptr));
        label_5->setText(QCoreApplication::translate("QMediaSelectDialog", "Audio device name", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_CaptureDevice), QCoreApplication::translate("QMediaSelectDialog", "\346\221\204\345\203\217\345\244\264", nullptr));
        pushButton_play->setText(QCoreApplication::translate("QMediaSelectDialog", "Play", nullptr));
        pushButton_cancel->setText(QCoreApplication::translate("QMediaSelectDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QMediaSelectDialog: public Ui_QMediaSelectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMEDIASELECTDIALOG_H
