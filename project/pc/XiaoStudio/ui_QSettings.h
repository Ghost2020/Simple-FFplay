/********************************************************************************
** Form generated from reading UI file 'QSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QSETTINGS_H
#define UI_QSETTINGS_H

#include <QMediaPlayer.h>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QSettings
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listWidget;
    QStackedWidget *stackedWidget;
    QWidget *nomalPage;
    QWidget *audioPage;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_volume;
    QSlider *horizontalSlider_volume;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *labelMicrophone;
    QComboBox *comboBox_Microphone;
    QPushButton *pushButton_Microphone;
    QWidget *layoutWidget_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_volume_Microphone;
    QSlider *horizontalSlider_volume_Microphone;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_speaker;
    QComboBox *comboBox_speakerDevice;
    QPushButton *pushButton;
    QWidget *videoPage;
    QLabel *label_cameraDevice;
    QComboBox *comboBox_cameraDevice;
    QLabel *label_videoPicture;
    QMediaPlayer *openGLWidget;
    QWidget *virtualBGpage;
    QWidget *networkPage;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_networkShow;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_2;
    QWidget *aboutPage;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_12;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_about;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_author;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_email;
    QLabel *label_emailAddress;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_github;
    QPushButton *pushButton_github;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_OK;
    QPushButton *pushButton_Cancel;
    QPushButton *pushButton_Applay;

    void setupUi(QDialog *QSettings)
    {
        if (QSettings->objectName().isEmpty())
            QSettings->setObjectName(QString::fromUtf8("QSettings"));
        QSettings->resize(642, 592);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QSettings->sizePolicy().hasHeightForWidth());
        QSettings->setSizePolicy(sizePolicy);
        QSettings->setStyleSheet(QString::fromUtf8("/* OBSTheme, main QApplication palette and QML values */\n"
"XiaoTheme {\n"
"    window: #181819;\n"
"    windowText: rgb(225,224,225);\n"
"    base: rgb(18,18,21);\n"
"    alternateBase: rgb(0,0,0);\n"
"    text: rgb(225,224,225);\n"
"    button: #162458;\n"
"    buttonText: rgb(225,224,225);\n"
"    brightText: #484848;\n"
"\n"
"    light: #162458;\n"
"    mid: #181819;\n"
"    dark: rgb(18,18,21);\n"
"    shadow: rgb(0,0,0);\n"
"\n"
"    highlight: #252458;\n"
"    highlightText: #FFFFFF;\n"
"\n"
"    link: #605ee6;\n"
"    linkVisited: #605ee6;\n"
"}\n"
"\n"
"XiaoTheme::disabled {\n"
"    text: #484848;\n"
"    buttonText: #484848;\n"
"    brightText: #484848;\n"
"}\n"
"\n"
"XiaoTheme::inactive {\n"
"    highlight: rgb(48,47,48);\n"
"    highlightText: rgb(255, 255, 255);\n"
"}\n"
"\n"
"/* General style, we override only what is needed. */\n"
"QWidget {\n"
"	background-color: #181819;\n"
"	alternate-background-color: rgb(18,18,21);\n"
"	color: rgb(225,224,225);\n"
"	selection-background-color: #252458;\n"
""
                        "	selection-color: white;\n"
"	outline: none;\n"
"	font-family: \"Open Sans\", \"Tahoma\", \"Arial\", sans-serif;\n"
"	font-size: 12px;\n"
"}\n"
"\n"
"#menubar {\n"
"	padding: 2px 8px 0px;\n"
"}\n"
"\n"
"QLabel:link {\n"
"	color: #2a3a75;\n"
"}\n"
"\n"
"QMenu {\n"
"	border: 1px solid #333336;\n"
"	padding: 4px;\n"
"}\n"
"\n"
"* [frameShape=\"1\"], * [frameShape=\"2\"], * [frameShape=\"3\"], * [frameShape=\"4\"], * [frameShape=\"5\"], * [frameShape=\"6\"] {\n"
"	/*border: 1px solid rgb(231,30,31); */\n"
"	border: none;\n"
"}\n"
"\n"
"* [frameShape=\"2\"],\n"
"* [frameShape=\"4\"] {\n"
"	border: 1px solid #333336;\n"
"}\n"
"\n"
"\n"
"QSizeGrip {\n"
"	background-color: transparent;\n"
"	image: url(./Acri/sizegrip.png);\n"
"}\n"
"\n"
"/* Misc */\n"
"\n"
"QWidget::disabled {\n"
"	color: #484848;\n"
"}\n"
"\n"
"* [themeID=\"error\"] {\n"
"	color: #d91740;\n"
"}\n"
"\n"
"* [themeID=\"warning\"] {\n"
"	color: #d9af17;\n"
"}\n"
"\n"
"/* Dropdown menus, Scenes box, Sources box */\n"
"QAbstractItemView {\n"
"	background-c"
                        "olor: #181819;\n"
"	border: 1px solid #333336;\n"
"}\n"
"\n"
"/* Group Box */\n"
"\n"
"QGroupBox {\n"
"	border: 2px solid #2f2f2f;\n"
"	margin-bottom: 8px;\n"
"	padding-top: 32px;\n"
"	font-weight: bold;\n"
"	font-size: 14px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"	left: 4px;\n"
"	right: 0;\n"
"	top: 8px;\n"
"	font-weight: bold;\n"
"	padding-left: 8px;\n"
"	padding-right: 8px;\n"
"	padding-bottom: 8px;\n"
"	font-size: 16px;\n"
"}\n"
"\n"
"/* --- */\n"
"\n"
"\n"
"/* Tooltips */\n"
"QToolTip {\n"
"	background-color: #212121;\n"
"	color: rgb(205,205,205);\n"
"	border: 1px solid #343335;\n"
"	border-radius: 4px;\n"
"}\n"
"\n"
"/* Top Menu Bar Items */\n"
"QMenuBar::item {\n"
"	background-color: rgb(24,24,25);\n"
"	padding: 6px;\n"
"}\n"
"\n"
"QMenuBar::item:selected {\n"
"	background: #2a3a75;\n"
"}\n"
"\n"
"/* Listbox item */\n"
"QListWidget::item,\n"
"SourceTree::item {\n"
"	padding: 4px 2px;\n"
"	margin-bottom: 2px;\n"
"	margin-top: 0px;\n"
"	border: 1px solid transparent;\n"
"}\n"
"\n"
"QListWidget QLineEdit {\n"
""
                        "	 padding-top: 0px;\n"
"	 padding-bottom: 0px;\n"
"	 padding-right: 0;\n"
"	 padding-left: 2px;\n"
"	 border: none;\n"
"	 border-radius: none;\n"
"}\n"
"\n"
"/* Dock stuff */\n"
"QDockWidget {\n"
"	background: #181819;\n"
"	border: none;\n"
"	font-size: 14px;\n"
"	font-weight: bold;\n"
"	border-bottom: 2px solid #2f2f2f;\n"
"}\n"
"\n"
"QDockWidget::title {\n"
"	border-bottom: 2px solid #2f2f2f;\n"
"	margin-left: 5px;\n"
"	margin-right: 5px;\n"
"	padding-top: 0px;\n"
"	padding-bottom: 6px;\n"
"	text-align: left;\n"
"	background-image: url(./Acri/top_hook.png);\n"
"	background-origin: padding;\n"
"	background-clip: padding;\n"
"	background-position: bottom left;\n"
"	background-repeat: none;\n"
"}\n"
"\n"
"QDockWidget::close-button,\n"
"QDockWidget::float-button {\n"
"	icon-size: 20px;\n"
"	subcontrol-position: top right;\n"
"	subcontrol-origin: padding;\n"
"	right: 0px;\n"
"}\n"
"\n"
"QDockWidget::float-button {\n"
"	right: 20px;\n"
"}\n"
"\n"
"\n"
"QListWidget#scenes,\n"
"SourceListWidget {\n"
"	border: none;\n"
""
                        "	border-bottom: 2px solid #2f2f2f;\n"
"}\n"
"\n"
"SourceTree {\n"
"	border: none;\n"
"	border-bottom: 1px solid #2f2f2f;\n"
"}\n"
"\n"
"SourceTree QLabel {\n"
"	padding: 2px 0px;\n"
"	margin: -2px 4px -2px;\n"
"}\n"
"\n"
"SourceTree QLineEdit {\n"
"	background-color: #0c101e;\n"
"	padding: 2px;\n"
"	margin: -2px 6px -2px 3px;\n"
"	font-size: 12px;\n"
"}\n"
"\n"
"#scenesFrame,\n"
"#sourcesFrame {\n"
"	margin-left: -7px;\n"
"	margin-right: -7px;\n"
"	margin-top: -8px;\n"
"	margin-bottom: -12px;\n"
"}\n"
"\n"
"#scenesToolbar,\n"
"#sourcesToolbar{\n"
"	background-image: url(./Acri/bot_hook2.png);\n"
"	background-origin: margin;\n"
"	background-clip: margin;\n"
"	background-position: top left;\n"
"	background-repeat: none;\n"
"}\n"
"\n"
"#sourcesToolbar {\n"
"	background-image: url(./Acri/bot_hook.png);\n"
"}\n"
"\n"
"/* Listbox item selected, unfocused */\n"
"QListWidget::item:hover,\n"
"SourceTree::item:hover {\n"
"	background-color: #212121;\n"
"	border: 1px solid #333336;\n"
"}\n"
"\n"
"/* Listbox item selected"
                        " */\n"
"QListWidget::item:selected,\n"
"SourceTree::item:selected {\n"
"	background-color: #131a30;\n"
"	border: 1px solid #252a45;\n"
"}\n"
"\n"
"/* ScrollBars */\n"
"\n"
"QScrollBar::corner {\n"
"	background-color: transparent;\n"
"	border: none;\n"
"}\n"
"\n"
"QScrollBar:vertical {\n"
"	background-color: transparent;\n"
"	width: 20px;\n"
"	margin-top: -3px;\n"
"	margin-bottom: -3px;\n"
"	padding: 4px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"	background-color: #2f2f2f;\n"
"	min-height: 20px;\n"
"	margin: 0px 3px;\n"
"	border-radius: 0px;\n"
"	border: none;\n"
"}\n"
"\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"	border: none;\n"
"	background: none;\n"
"	height: 0;\n"
"}\n"
"\n"
"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"	border: none;\n"
"	background: none;\n"
"	color: none;\n"
"}\n"
"\n"
"QScrollBar:horizontal {\n"
"	background-color: transparent;\n"
"	height: 20px;\n"
"	margin-left: -"
                        "3px;\n"
"	margin-right: -3px;\n"
"	padding: 4px;\n"
"}\n"
"\n"
"QScrollBar::handle:horizontal {\n"
"	background-color: #2f2f2f;\n"
"	min-width: 20px;\n"
"	margin: 3px 0px;\n"
"	border-radius: 0px;\n"
"	border: none;\n"
"}\n"
"\n"
"QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {\n"
"	border: none;\n"
"	background: none;\n"
"	width: 0;\n"
"}\n"
"\n"
"QScrollBar::left-arrow:horizontal, QScrollBar::right-arrow:horizontal, QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {\n"
"	border: none;\n"
"	background: none;\n"
"	color: none;\n"
"}\n"
"\n"
"/* Scenes and Sources toolbar */\n"
"\n"
"QToolBar {\n"
"	background-color: rgb(24,24,25);\n"
"	margin-top: 4px;\n"
"}\n"
"\n"
"QToolButton:hover {\n"
"	background-color: #2a3a75;\n"
"	border: 1px solid #233166;\n"
"	border-radius: none;\n"
"}\n"
"\n"
"QToolButton:pressed {\n"
"	background-color: #161f41;\n"
"	border-radius: none;\n"
"}\n"
"\n"
"* [themeID=\"addIconSmall\"] {\n"
"	qproperty-icon: url(./Dark/plus.svg);\n"
"}\n"
"\n"
"* [th"
                        "emeID=\"removeIconSmall\"] {\n"
"	qproperty-icon: url(./Dark/minus.svg);\n"
"}\n"
"\n"
"* [themeID=\"propertiesIconSmall\"] {\n"
"	qproperty-icon: url(./Dark/settings/general.svg);\n"
"}\n"
"\n"
"* [themeID=\"configIconSmall\"] {\n"
"	qproperty-icon: url(./Dark/settings/general.svg);\n"
"}\n"
"\n"
"* [themeID=\"upArrowIconSmall\"] {\n"
"	qproperty-icon: url(./Dark/up.svg);\n"
"}\n"
"\n"
"* [themeID=\"downArrowIconSmall\"] {\n"
"	qproperty-icon: url(./Dark/down.svg);\n"
"}\n"
"\n"
"* [themeID=\"pauseIconSmall\"] {\n"
"	qproperty-icon: url(./Dark/media-pause.svg);\n"
"}\n"
"\n"
"/* Tab Widget */\n"
"\n"
"QTabWidget::pane { /* The tab widget frame */\n"
"	border-top: 1px solid #2f2f2f;\n"
"}\n"
"\n"
"QTabWidget::tab-bar {\n"
"	alignment: left;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"	background-color: #212121;\n"
"	border: 1px solid #333336;\n"
"	min-width: 8ex;\n"
"	padding-top: 4px;\n"
"	padding-bottom: 4px;\n"
"	padding-left: 10px;\n"
"	padding-right: 10px;\n"
"	margin-right: 2px;\n"
"	margin-top: 1px;\n"
"	margin-b"
                        "ottom: 1px;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"	background-color: #131a30;\n"
"	border-color: #252a45;\n"
"}\n"
"\n"
"QTabBar::tab:hover {\n"
"	background-color: #233166;\n"
"	border-color: #364683;\n"
"}\n"
"\n"
"QTabBar::tab:pressed {\n"
"	background-color: #161f41;\n"
"}\n"
"\n"
"/* ComboBox */\n"
"\n"
"QComboBox {\n"
"	background-color: rgb(40,40,42);\n"
"	border-style: solid;\n"
"	border: 1px;\n"
"	border-color: rgb(24,24,30);\n"
"	padding: 4px;\n"
"	padding-left: 10px;\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"	border:none;\n"
"	border-left: 1px solid rgba(31,30,31,155);\n"
"	width: 20px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"	qproperty-alignment: AlignTop;\n"
"	image: url(./Dark/updown.svg);\n"
"	width: 100%;\n"
"}\n"
"\n"
"QComboBox:on {\n"
"	background-color: #2a3a75;\n"
"}\n"
"\n"
"QComboBox:editable {\n"
"	border-top-left-radius: 0px;\n"
"	border-bottom-left-radius: 0px;\n"
"}\n"
"\n"
"QComboBox::drop-down:editable {\n"
"	border-top-right-radius: 3px;\n"
"	border-bottom-right-radius: 3p"
                        "x;\n"
"}\n"
"\n"
"QComboBox::down-arrow:editable {\n"
"	qproperty-alignment: AlignTop;\n"
"	image: url(./Dark/down.svg);\n"
"	width: 8%;\n"
"}\n"
"\n"
"/* Textedits etc */\n"
"\n"
"QLineEdit, QTextEdit, QPlainTextEdit {\n"
"	background-color: rgb(8,8,11);\n"
"	border: none;\n"
"	padding: 4px;\n"
"	padding-left: 2px;\n"
"}\n"
"\n"
"/* Spinbox and doubleSpinbox */\n"
"\n"
"QSpinBox, QDoubleSpinBox {\n"
"	background-color: rgb(8,8,11);\n"
"	border: none;\n"
"	padding: 4px;\n"
"	padding-left: 2px;\n"
"	padding-right: 15px;\n"
"	margin-right: 10px;\n"
"}\n"
"\n"
"QSpinBox::up-button, QDoubleSpinBox::up-button {\n"
"	subcontrol-origin: margin;\n"
"	subcontrol-position: top right; /* position at the top right corner */\n"
"\n"
"	background-color: rgb(24,24,30);\n"
"	border: 1px solid rgb(8,8,11);\n"
"	border-radius: 3px;\n"
"	border-width: 0;\n"
"	border-bottom-left-radius: 0;\n"
"	border-bottom-right-radius: 0;\n"
"	border-bottom-width: 0;\n"
"}\n"
"\n"
"QSpinBox::down-button, QDoubleSpinBox::down-button {\n"
"	subc"
                        "ontrol-origin: margin;\n"
"	subcontrol-position: bottom right; /* position at the top right corner */\n"
"	background-color: rgb(24,24,30);\n"
"	border: 1px solid rgb(8,8,11);\n"
"	border-radius: 3px;\n"
"	border-width: 0;\n"
"	border-top-left-radius: 0;\n"
"	border-top-right-radius: 0;\n"
"	border-top-width: 0;\n"
"}\n"
"\n"
"QSpinBox::up-button:hover, QSpinBox::down-button:hover, QDoubleSpinBox::up-button:hover, QDoubleSpinBox::down-button:hover {\n"
"	background-color: rgb(15,33,51);\n"
"}\n"
"\n"
"QSpinBox::up-button:pressed, QSpinBox::down-button:pressed, QDoubleSpinBox::up-button:pressed, QDoubleSpinBox::down-button:pressed {\n"
"	background-color: rgb(24,24,25);\n"
"}\n"
"\n"
"QSpinBox::up-button:disabled, QSpinBox::up-button:off, QSpinBox::down-button:disabled, QSpinBox::down-button:off {\n"
"	background-color: rgb(24,24,25);\n"
"}\n"
"\n"
"QDoubleSpinBox::up-button:disabled, QDoubleSpinBox::up-button:off, QDoubleSpinBox::down-button:disabled, QDoubleSpinBox::down-button:off {\n"
"	background-color: rg"
                        "b(24,24,25);\n"
"}\n"
"\n"
"QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {\n"
"	image: url(./Dark/up.svg);\n"
"	width: 100%;\n"
"}\n"
"\n"
"QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {\n"
"	image: url(./Dark/down.svg);\n"
"	width: 100%;\n"
"}\n"
"\n"
"\n"
"/* Buttons */\n"
"\n"
"QPushButton {\n"
"	color: rgb(225,224,225);\n"
"	background-color: #162458;\n"
"	border: 1px solid #233166;\n"
"	padding: 6px 20px;\n"
"	margin: 1px;\n"
"}\n"
"\n"
"QPushButton::flat {\n"
"	background-color: rgb(24,24,25);\n"
"	border: none;\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"	background-color: #581624;\n"
"	border-color: #84162d;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background-color: #2a3a75;\n"
"	border: 1px solid #364683;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"	background-color: #161f41;\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"	border: 1px solid #232426;\n"
"	background-color: #1a1a1b;\n"
"}\n"
"\n"
"QPushButton::flat:hover,\n"
"QPushButton::flat:disabled {\n"
"	border: none;\n"
"}\n"
"\n"
"/* Progress Bar */\n"
""
                        "\n"
"QProgressBar {\n"
"	background: #101010;\n"
"	border: 2px solid #363636;\n"
"	border-radius: 0px;\n"
"	text-align: center;\n"
"}\n"
"\n"
"QProgressBar::chunk {\n"
"	background-color: #2a3a75;\n"
"}\n"
"\n"
"/* Sliders */\n"
"\n"
"QSlider::groove:horizontal {\n"
"	background-color: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"		stop: 0 rgb(31,30,31),\n"
"		stop: 0.75 rgb(50, 49, 50));\n"
"	height: 4px;\n"
"	border: none;\n"
"	border-radius: 2px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"	background-color: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"		stop: 0 rgb(240,239,240),\n"
"		stop: 0.25 rgb(200,199,200),\n"
"		stop: 1 rgb(162,161,162));\n"
"	border: 1px solid rgb(24,24,25);\n"
"	border-radius: 3px;\n"
"	height: 10px;\n"
"	width: 18px;\n"
"	margin: -3px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */\n"
"}\n"
"\n"
"QSlider::handle:horizontal:pressed {\n"
"	background-color: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"		stop: 0 rgb(240"
                        ",239,240),\n"
"		stop: 0.25 rgb(200,199,200),\n"
"		stop: 1 rgb(162,161,162));\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"	background-color: #2a3a75;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal:disabled {\n"
"	background-color: QLinearGradient(x1: 0, y1: 1, x2: 0, y2: 0,\n"
"		stop: 0 rgb(26,25,26),\n"
"		stop: 0.75 rgb(10, 10, 10));\n"
"	border-radius: 2px;\n"
"}\n"
"\n"
"QSlider::groove:vertical {\n"
"	background-color: QLinearGradient(x1: 0, y1: 0, x2: 1, y2: 0,\n"
"		stop: 0 rgb(31,30,31),\n"
"		stop: 0.75 rgb(50, 49, 50));\n"
"	width: 4px;\n"
"	border: none;\n"
"	border-radius: 2px;\n"
"}\n"
"\n"
"QSlider::handle:vertical {\n"
"	background-color: QLinearGradient(x1: 0, y1: 0, x2: 1, y2: 0,\n"
"		stop: 0 rgb(240,239,240),\n"
"		stop: 0.25 rgb(200,199,200),\n"
"		stop: 1 rgb(162,161,162));\n"
"	border: 1px solid rgb(24,24,25);\n"
"	border-radius: 3px;\n"
"	width: 10px;\n"
"	height: 18px;\n"
"	margin: 0 -3px; /* handle is placed by default on the contents rect of the groove. Expand outside the groove "
                        "*/\n"
"}\n"
"\n"
"QSlider::handle:vertical:pressed {\n"
"	background-color: QLinearGradient(x1: 0, y1: 0, x2: 1, y2: 0,\n"
"		stop: 0 rgb(240,239,240),\n"
"		stop: 0.25 rgb(200,199,200),\n"
"		stop: 1 rgb(162,161,162));\n"
"}\n"
"\n"
"QSlider::add-page:vertical {\n"
"	background-color: #2a3a75;\n"
"}\n"
"\n"
"QSlider::add-page:vertical:disabled {\n"
"	background-color: QLinearGradient(x1: 0, y1: 0, x2: 1, y2: 0,\n"
"		stop: 0 rgb(26,25,26),\n"
"		stop: 0.75 rgb(10, 10, 10));\n"
"	border-radius: 2px;\n"
"}\n"
"\n"
"QSlider::handle:hover {\n"
"	background-color: rgb(200,199,200);\n"
"}\n"
"\n"
"QSlider::handle:disabled {\n"
"	background-color: rgb(15,15,16);\n"
"}\n"
"\n"
"/* Volume Control */\n"
"\n"
"VolumeMeter {\n"
"\n"
"	qproperty-backgroundNominalColor: #42740c;\n"
"	qproperty-backgroundWarningColor: #988F0F;\n"
"	qproperty-backgroundErrorColor: #802004;\n"
"	qproperty-foregroundNominalColor: #84D82B;\n"
"	qproperty-foregroundWarningColor: #E4D717;\n"
"	qproperty-foregroundErrorColor: #D74116;\n"
"	qproper"
                        "ty-magnitudeColor: rgb(49, 54, 59); /* Blue-gray */\n"
"	qproperty-majorTickColor: rgb(239, 240, 241); /* White */\n"
"	qproperty-minorTickColor: rgb(118, 121, 124); /* Light Gray */\n"
"	qproperty-peakDecayRate: 23.4; /* Override of the standard PPM Type I rate. */\n"
"}\n"
"\n"
"\n"
"/* Status Bar */\n"
"\n"
"\n"
"QStatusBar::item {\n"
"	border: none;\n"
"}\n"
"\n"
"/* Checkboxes */\n"
"QCheckBox {\n"
"	padding: 4px;\n"
"}\n"
"\n"
"QCheckBox::indicator,\n"
"QGroupBox::indicator {\n"
"	width: 20px;\n"
"	height: 20px;\n"
"}\n"
"\n"
"QGroupBox::indicator {\n"
"	margin-left: 2px;\n"
"}\n"
"\n"
"QCheckBox::indicator:unchecked {\n"
"	image: url(./Acri/checkbox_unchecked.png);\n"
"}\n"
"\n"
"QCheckBox::indicator:unchecked:hover,\n"
"QGroupBox::indicator:unchecked:hover {\n"
"	border: none;\n"
"	image: url(./Acri/checkbox_unchecked_focus.png);\n"
"}\n"
"\n"
"QCheckBox::indicator:checked {\n"
"	image: url(./Acri/checkbox_checked.png);\n"
"}\n"
"\n"
"QCheckBox::indicator:checked:hover,\n"
"QGroupBox::indicator:checked"
                        ":hover {\n"
"	border: none;\n"
"	image: url(./Acri/checkbox_checked_focus.png);\n"
"}\n"
"\n"
"QCheckBox::indicator:checked:disabled,\n"
"QGroupBox::indicator:checked:disabled {\n"
"	image: url(./Acri/checkbox_checked_disabled.png);\n"
"}\n"
"\n"
"QCheckBox::indicator:unchecked:disabled,\n"
"QGroupBox::indicator:unchecked:disabled {\n"
"	image: url(./Acri/checkbox_unchecked_disabled.png);\n"
"}\n"
"\n"
"/* Radio Buttons */\n"
"\n"
"QRadioButton::indicator {\n"
"	width: 19px;\n"
"	height: 19px;\n"
"}\n"
"\n"
"QRadioButton::indicator:unchecked {\n"
"	image: url(./Acri/radio_unchecked.png);\n"
"}\n"
"\n"
"\n"
"QRadioButton::indicator:unchecked:hover,\n"
"QRadioButton::indicator:unchecked:focus,\n"
"QRadioButton::indicator:unchecked:pressed {\n"
"	border: none;\n"
"	outline: none;\n"
"	image: url(./Acri/radio_unchecked_focus.png);\n"
"}\n"
"\n"
"QRadioButton::indicator:checked {\n"
"	border: none;\n"
"	outline: none;\n"
"	image: url(./Acri/radio_checked.png);\n"
"}\n"
"\n"
"QRadioButton::indicator:checked:hover,\n"
""
                        "QRadioButton::indicator:checked:focus,\n"
"QRadioButton::indicator:checked:pressed {\n"
"	border: none;\n"
"	outline: none;\n"
"	image: url(./Acri/radio_checked_focus.png);\n"
"}\n"
"\n"
"QRadioButton::indicator:checked:disabled {\n"
"	outline: none;\n"
"	image: url(./Acri/radio_checked_disabled.png);\n"
"}\n"
"\n"
"QRadioButton::indicator:unchecked:disabled {\n"
"	image: url(./Acri/radio_unchecked_disabled.png);\n"
"}\n"
"\n"
"/* Mute CheckBox */\n"
"\n"
"MuteCheckBox {\n"
"	outline: none;\n"
"}\n"
"\n"
"MuteCheckBox::indicator:checked {\n"
"	image: url(./Dark/mute.svg);\n"
"}\n"
"\n"
"MuteCheckBox::indicator:unchecked {\n"
"	image: url(./Dark/settings/audio.svg);\n"
"}\n"
"\n"
"MuteCheckBox::indicator:unchecked:hover {\n"
"	image: url(./Dark/settings/audio.svg);\n"
"}\n"
"\n"
"MuteCheckBox::indicator:unchecked:focus {\n"
"	image: url(./Dark/settings/audio.svg);\n"
"}\n"
"MuteCheckBox::indicator:checked:hover {\n"
"	image: url(./Dark/mute.svg);\n"
"}\n"
"\n"
"MuteCheckBox::indicator:checked:focus {\n"
"	image"
                        ": url(./Dark/mute.svg);\n"
"}\n"
"\n"
"MuteCheckBox::indicator:checked:disabled {\n"
"	image: url(./Dark/mute.svg);\n"
"}\n"
"\n"
"MuteCheckBox::indicator:unchecked:disabled {\n"
"	image: url(./Dark/settings/audio.svg);\n"
"}\n"
"\n"
"OBSHotkeyLabel[hotkeyPairHover=true] {\n"
"	color: rgba(27, 96, 166);\n"
"}\n"
"\n"
"\n"
"/* Group Collapse Checkbox */\n"
"\n"
"SourceTreeSubItemCheckBox {\n"
"    background: transparent;\n"
"    outline: none;\n"
"    padding: 0px;\n"
"}\n"
"\n"
"SourceTreeSubItemCheckBox::indicator {\n"
"    width: 12px;\n"
"    height: 12px;\n"
"}\n"
"\n"
"SourceTreeSubItemCheckBox::indicator:checked,\n"
"SourceTreeSubItemCheckBox::indicator:checked:hover {\n"
"    image: url(./Dark/expand.svg);\n"
"}\n"
"\n"
"SourceTreeSubItemCheckBox::indicator:unchecked,\n"
"SourceTreeSubItemCheckBox::indicator:unchecked:hover {\n"
"    image: url(./Dark/down.svg);\n"
"}\n"
"\n"
"\n"
"/* Label warning/error */\n"
"\n"
"QLabel#warningLabel {\n"
"	color: rgb(192, 128, 0);\n"
"	font-weight: bold;\n"
"}\n"
"\n"
""
                        "QLabel#errorLabel {\n"
"	color: rgb(192, 0, 0);\n"
"	font-weight: bold;\n"
"}\n"
"\n"
"/* Settings Menu */\n"
"\n"
"#buttonBox {\n"
"	border-top: 2px solid grey;\n"
"}\n"
"\n"
"/* Special Fixes */\n"
"\n"
"OBSBasicProperties,\n"
"#OBSBasicSettings,\n"
"#OBSBasicFilters {\n"
"	background: #101010;\n"
"}\n"
"\n"
"FocusList::item {\n"
"	padding: 0px 2px;\n"
"}\n"
"\n"
"#fpsTypes {\n"
"	padding: 0px;\n"
"}\n"
"\n"
"#finishPage QLabel {\n"
"	padding: -2px 0px;\n"
"	background: transparent;\n"
"	min-height: 26px;\n"
"}\n"
"\n"
"/* About dialog */\n"
"\n"
"#OBSAbout #icon {\n"
"	padding: 0;\n"
"}\n"
"\n"
"#OBSAbout QTextBrowser {\n"
"	background: transparent;\n"
"	padding: 2px 0;\n"
"}\n"
"\n"
"* [themeID=\"aboutName\"] {\n"
"	font-size: 36px;\n"
"	font-weight: bold;\n"
"	padding: 0;\n"
"}\n"
"\n"
"* [themeID=\"aboutVersion\"] {\n"
"	font-size: 16px;\n"
"	margin-bottom: 20px;\n"
"	padding: 0;\n"
"}\n"
"\n"
"* [themeID=\"aboutInfo\"] {\n"
"	margin-bottom: 20px;\n"
"}\n"
"\n"
"* [themeID=\"aboutHLayout\"] {\n"
"	backgr"
                        "ound-color: rgb(8, 8, 11);\n"
"}\n"
"\n"
"/* Preview background color */\n"
"\n"
"OBSQTDisplay {\n"
"	qproperty-displayBackgroundColor: #28282A;\n"
"}\n"
"\n"
"/* Preview/Program labels */\n"
"\n"
"* [themeID=\"previewProgramLabels\"] {\n"
"	font-size: 18px;\n"
"	font-weight: bold;\n"
"	color: rgb(122,121,122);\n"
"}\n"
"\n"
"/* Settings Icons */\n"
"\n"
"OBSBasicSettings {\n"
"    qproperty-generalIcon: url(./Dark/settings/general.svg);\n"
"    qproperty-streamIcon: url(./Dark/settings/stream.svg);\n"
"    qproperty-outputIcon: url(./Dark/settings/output.svg);\n"
"    qproperty-audioIcon: url(./Dark/settings/audio.svg);\n"
"    qproperty-videoIcon: url(./Dark/settings/video.svg);\n"
"    qproperty-hotkeysIcon: url(./Dark/settings/hotkeys.svg);\n"
"    qproperty-advancedIcon: url(./Dark/settings/advanced.svg);\n"
"}\n"
"\n"
"OBSBasicSettings QListWidget::item {\n"
"    padding-top: 5px;\n"
"    padding-bottom: 5px;\n"
"}\n"
"\n"
"/* Locked CheckBox */\n"
"\n"
"LockedCheckBox {\n"
"    outline: none;\n"
"    ba"
                        "ckground: transparent;\n"
"    padding: 0px;\n"
"}\n"
"\n"
"LockedCheckBox::indicator {\n"
"    width: 16px;\n"
"    height: 16px;\n"
"}\n"
"\n"
"LockedCheckBox::indicator:checked {\n"
"    image: url(./Dark/locked.svg);\n"
"}\n"
"\n"
"LockedCheckBox::indicator:unchecked {\n"
"    image: url(:res/images/unlocked.svg);\n"
"}\n"
"\n"
"LockedCheckBox::indicator:checked:hover {\n"
"    image: url(./Dark/locked.svg);\n"
"}\n"
"\n"
"LockedCheckBox::indicator:unchecked:hover {\n"
"    image: url(:res/images/unlocked.svg);\n"
"}\n"
"\n"
"/* Visibilty CheckBox */\n"
"\n"
"VisibilityCheckBox {\n"
"    outline: none;\n"
"    background: transparent;\n"
"    padding: 0px;\n"
"}\n"
"\n"
"VisibilityCheckBox::indicator {\n"
"    width: 16px;\n"
"    height: 16px;\n"
"}\n"
"\n"
"VisibilityCheckBox::indicator:checked {\n"
"    image: url(./Dark/visible.svg);\n"
"}\n"
"\n"
"VisibilityCheckBox::indicator:unchecked {\n"
"    image: url(:res/images/invisible.svg);\n"
"}\n"
"\n"
"VisibilityCheckBox::indicator:checked:hover {\n"
"  "
                        "  image: url(./Dark/visible.svg);\n"
"}\n"
"\n"
"VisibilityCheckBox::indicator:unchecked:hover {\n"
"    image: url(:res/images/invisible.svg);\n"
"}\n"
"\n"
"* [themeID=\"trashIcon\"] {\n"
"    qproperty-icon: url(./Dark/trash.svg);\n"
"}\n"
"\n"
"* [themeID=\"revertIcon\"] {\n"
"    qproperty-icon: url(./Dark/revert.svg);\n"
"}\n"
"\n"
"QPushButton#extraPanelDelete {\n"
"     background: transparent;\n"
"     border: none;\n"
"}\n"
"\n"
"QPushButton#extraPanelDelete:hover {\n"
"     background-color: #2a3a75;\n"
"}\n"
"\n"
"QPushButton#extraPanelDelete:pressed {\n"
"    background-color: #161f41;\n"
"}\n"
""));
        verticalLayout = new QVBoxLayout(QSettings);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        listWidget = new QListWidget(QSettings);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/icon/ic_settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(listWidget);
        __qlistwidgetitem->setIcon(icon);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/icon/ic_lock_ringer_on_alpha.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(listWidget);
        __qlistwidgetitem1->setIcon(icon1);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/res/icon/video.svg"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem2 = new QListWidgetItem(listWidget);
        __qlistwidgetitem2->setIcon(icon2);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/res/icon/camera_white_balance_cloudy.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem3 = new QListWidgetItem(listWidget);
        __qlistwidgetitem3->setIcon(icon3);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/res/icon/stream.svg"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem4 = new QListWidgetItem(listWidget);
        __qlistwidgetitem4->setIcon(icon4);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/res/icon/ic_menu_cc_am.png"), QSize(), QIcon::Normal, QIcon::Off);
        QListWidgetItem *__qlistwidgetitem5 = new QListWidgetItem(listWidget);
        __qlistwidgetitem5->setIcon(icon5);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy1);
        listWidget->setMaximumSize(QSize(100, 16777215));
        listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget->setIconSize(QSize(32, 32));
        listWidget->setItemAlignment(Qt::AlignCenter);
        listWidget->setSortingEnabled(false);

        horizontalLayout_2->addWidget(listWidget);

        stackedWidget = new QStackedWidget(QSettings);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        nomalPage = new QWidget();
        nomalPage->setObjectName(QString::fromUtf8("nomalPage"));
        stackedWidget->addWidget(nomalPage);
        audioPage = new QWidget();
        audioPage->setObjectName(QString::fromUtf8("audioPage"));
        layoutWidget = new QWidget(audioPage);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(70, 74, 401, 30));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_volume = new QLabel(layoutWidget);
        label_volume->setObjectName(QString::fromUtf8("label_volume"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_volume->sizePolicy().hasHeightForWidth());
        label_volume->setSizePolicy(sizePolicy2);
        label_volume->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_volume);

        horizontalSlider_volume = new QSlider(layoutWidget);
        horizontalSlider_volume->setObjectName(QString::fromUtf8("horizontalSlider_volume"));
        horizontalSlider_volume->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(horizontalSlider_volume);

        layoutWidget_2 = new QWidget(audioPage);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(70, 166, 402, 30));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        labelMicrophone = new QLabel(layoutWidget_2);
        labelMicrophone->setObjectName(QString::fromUtf8("labelMicrophone"));
        sizePolicy.setHeightForWidth(labelMicrophone->sizePolicy().hasHeightForWidth());
        labelMicrophone->setSizePolicy(sizePolicy);
        labelMicrophone->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(labelMicrophone);

        comboBox_Microphone = new QComboBox(layoutWidget_2);
        comboBox_Microphone->setObjectName(QString::fromUtf8("comboBox_Microphone"));
        comboBox_Microphone->setMinimumSize(QSize(200, 0));

        horizontalLayout_5->addWidget(comboBox_Microphone);

        pushButton_Microphone = new QPushButton(layoutWidget_2);
        pushButton_Microphone->setObjectName(QString::fromUtf8("pushButton_Microphone"));
        sizePolicy.setHeightForWidth(pushButton_Microphone->sizePolicy().hasHeightForWidth());
        pushButton_Microphone->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(pushButton_Microphone);

        layoutWidget_3 = new QWidget(audioPage);
        layoutWidget_3->setObjectName(QString::fromUtf8("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(70, 210, 401, 30));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget_3);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_volume_Microphone = new QLabel(layoutWidget_3);
        label_volume_Microphone->setObjectName(QString::fromUtf8("label_volume_Microphone"));
        sizePolicy2.setHeightForWidth(label_volume_Microphone->sizePolicy().hasHeightForWidth());
        label_volume_Microphone->setSizePolicy(sizePolicy2);
        label_volume_Microphone->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(label_volume_Microphone);

        horizontalSlider_volume_Microphone = new QSlider(layoutWidget_3);
        horizontalSlider_volume_Microphone->setObjectName(QString::fromUtf8("horizontalSlider_volume_Microphone"));
        horizontalSlider_volume_Microphone->setOrientation(Qt::Horizontal);

        horizontalLayout_6->addWidget(horizontalSlider_volume_Microphone);

        widget = new QWidget(audioPage);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(70, 30, 401, 30));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_speaker = new QLabel(widget);
        label_speaker->setObjectName(QString::fromUtf8("label_speaker"));
        sizePolicy.setHeightForWidth(label_speaker->sizePolicy().hasHeightForWidth());
        label_speaker->setSizePolicy(sizePolicy);
        label_speaker->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_speaker);

        comboBox_speakerDevice = new QComboBox(widget);
        comboBox_speakerDevice->setObjectName(QString::fromUtf8("comboBox_speakerDevice"));
        comboBox_speakerDevice->setMinimumSize(QSize(200, 0));

        horizontalLayout_3->addWidget(comboBox_speakerDevice);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMaximumSize(QSize(116, 16777215));

        horizontalLayout_3->addWidget(pushButton);

        stackedWidget->addWidget(audioPage);
        videoPage = new QWidget();
        videoPage->setObjectName(QString::fromUtf8("videoPage"));
        label_cameraDevice = new QLabel(videoPage);
        label_cameraDevice->setObjectName(QString::fromUtf8("label_cameraDevice"));
        label_cameraDevice->setGeometry(QRect(20, 20, 91, 16));
        comboBox_cameraDevice = new QComboBox(videoPage);
        comboBox_cameraDevice->setObjectName(QString::fromUtf8("comboBox_cameraDevice"));
        comboBox_cameraDevice->setGeometry(QRect(120, 20, 361, 22));
        label_videoPicture = new QLabel(videoPage);
        label_videoPicture->setObjectName(QString::fromUtf8("label_videoPicture"));
        label_videoPicture->setGeometry(QRect(20, 62, 91, 20));
        openGLWidget = new QMediaPlayer(videoPage);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(120, 90, 361, 261));
        stackedWidget->addWidget(videoPage);
        virtualBGpage = new QWidget();
        virtualBGpage->setObjectName(QString::fromUtf8("virtualBGpage"));
        stackedWidget->addWidget(virtualBGpage);
        networkPage = new QWidget();
        networkPage->setObjectName(QString::fromUtf8("networkPage"));
        verticalLayout_3 = new QVBoxLayout(networkPage);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalSpacer_2 = new QSpacerItem(88, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_2);

        label_networkShow = new QLabel(networkPage);
        label_networkShow->setObjectName(QString::fromUtf8("label_networkShow"));
        sizePolicy.setHeightForWidth(label_networkShow->sizePolicy().hasHeightForWidth());
        label_networkShow->setSizePolicy(sizePolicy);
        label_networkShow->setMinimumSize(QSize(200, 200));
        label_networkShow->setPixmap(QPixmap(QString::fromUtf8(":/res/icon/stream.svg")));
        label_networkShow->setScaledContents(false);
        label_networkShow->setAlignment(Qt::AlignCenter);

        horizontalLayout_7->addWidget(label_networkShow);

        horizontalSpacer_3 = new QSpacerItem(108, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_7);

        label = new QLabel(networkPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalSpacer_4 = new QSpacerItem(78, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_4);

        pushButton_2 = new QPushButton(networkPage);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);

        horizontalLayout_8->addWidget(pushButton_2);

        horizontalSpacer_5 = new QSpacerItem(68, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);


        verticalLayout_2->addLayout(horizontalLayout_8);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        verticalLayout_3->addLayout(verticalLayout_2);

        stackedWidget->addWidget(networkPage);
        aboutPage = new QWidget();
        aboutPage->setObjectName(QString::fromUtf8("aboutPage"));
        verticalLayout_4 = new QVBoxLayout(aboutPage);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalSpacer_6 = new QSpacerItem(48, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_6);

        label_about = new QLabel(aboutPage);
        label_about->setObjectName(QString::fromUtf8("label_about"));
        sizePolicy.setHeightForWidth(label_about->sizePolicy().hasHeightForWidth());
        label_about->setSizePolicy(sizePolicy);
        label_about->setMinimumSize(QSize(200, 200));

        horizontalLayout_12->addWidget(label_about);

        horizontalSpacer_7 = new QSpacerItem(48, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_7);


        verticalLayout_4->addLayout(horizontalLayout_12);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_author = new QLabel(aboutPage);
        label_author->setObjectName(QString::fromUtf8("label_author"));
        label_author->setAlignment(Qt::AlignCenter);

        horizontalLayout_11->addWidget(label_author);

        label_2 = new QLabel(aboutPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_11->addWidget(label_2);


        verticalLayout_4->addLayout(horizontalLayout_11);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_email = new QLabel(aboutPage);
        label_email->setObjectName(QString::fromUtf8("label_email"));
        label_email->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(label_email);

        label_emailAddress = new QLabel(aboutPage);
        label_emailAddress->setObjectName(QString::fromUtf8("label_emailAddress"));
        label_emailAddress->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(label_emailAddress);


        verticalLayout_4->addLayout(horizontalLayout_10);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_github = new QLabel(aboutPage);
        label_github->setObjectName(QString::fromUtf8("label_github"));
        label_github->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(label_github);

        pushButton_github = new QPushButton(aboutPage);
        pushButton_github->setObjectName(QString::fromUtf8("pushButton_github"));
        sizePolicy.setHeightForWidth(pushButton_github->sizePolicy().hasHeightForWidth());
        pushButton_github->setSizePolicy(sizePolicy);
        pushButton_github->setMaximumSize(QSize(225, 45));

        horizontalLayout_9->addWidget(pushButton_github);


        verticalLayout_4->addLayout(horizontalLayout_9);

        stackedWidget->addWidget(aboutPage);

        horizontalLayout_2->addWidget(stackedWidget);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(228, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_OK = new QPushButton(QSettings);
        pushButton_OK->setObjectName(QString::fromUtf8("pushButton_OK"));
        pushButton_OK->setMinimumSize(QSize(81, 0));

        horizontalLayout->addWidget(pushButton_OK);

        pushButton_Cancel = new QPushButton(QSettings);
        pushButton_Cancel->setObjectName(QString::fromUtf8("pushButton_Cancel"));

        horizontalLayout->addWidget(pushButton_Cancel);

        pushButton_Applay = new QPushButton(QSettings);
        pushButton_Applay->setObjectName(QString::fromUtf8("pushButton_Applay"));

        horizontalLayout->addWidget(pushButton_Applay);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(QSettings);

        listWidget->setCurrentRow(-1);
        stackedWidget->setCurrentIndex(5);


        QMetaObject::connectSlotsByName(QSettings);
    } // setupUi

    void retranslateUi(QDialog *QSettings)
    {
        QSettings->setWindowTitle(QCoreApplication::translate("QSettings", "Dialog", nullptr));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("QSettings", "Basic.Settings.Nomal", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("QSettings", "Basic.Settings.Audio", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = listWidget->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("QSettings", "Basic.Settings.Video", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = listWidget->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("QSettings", "Basic.Settings.VirtualBG", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = listWidget->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("QSettings", "Basic.Settings.Network", nullptr));
        QListWidgetItem *___qlistwidgetitem5 = listWidget->item(5);
        ___qlistwidgetitem5->setText(QCoreApplication::translate("QSettings", "Basic.Settings.About", nullptr));
        listWidget->setSortingEnabled(__sortingEnabled);

        label_volume->setText(QCoreApplication::translate("QSettings", "Volume:", nullptr));
        labelMicrophone->setText(QCoreApplication::translate("QSettings", "Microphone\357\274\232", nullptr));
        pushButton_Microphone->setText(QCoreApplication::translate("QSettings", "Test Speaker", nullptr));
        label_volume_Microphone->setText(QCoreApplication::translate("QSettings", "Volume:", nullptr));
        label_speaker->setText(QCoreApplication::translate("QSettings", "Speaker:", nullptr));
        pushButton->setText(QCoreApplication::translate("QSettings", "Test Speaker", nullptr));
        label_cameraDevice->setText(QCoreApplication::translate("QSettings", "Choose Device:", nullptr));
        label_videoPicture->setText(QCoreApplication::translate("QSettings", "Video picture:", nullptr));
        label_networkShow->setText(QString());
        label->setText(QCoreApplication::translate("QSettings", "Network Test", nullptr));
        pushButton_2->setText(QCoreApplication::translate("QSettings", "Begin Test", nullptr));
        label_about->setText(QCoreApplication::translate("QSettings", "TextLabel", nullptr));
        label_author->setText(QCoreApplication::translate("QSettings", "Author:", nullptr));
        label_2->setText(QCoreApplication::translate("QSettings", "GhostChen", nullptr));
        label_email->setText(QCoreApplication::translate("QSettings", "Email:", nullptr));
        label_emailAddress->setText(QCoreApplication::translate("QSettings", "cxx2020@outlook.com", nullptr));
        label_github->setText(QCoreApplication::translate("QSettings", "Github:", nullptr));
        pushButton_github->setText(QCoreApplication::translate("QSettings", "https://github.com/Ghost2020", nullptr));
        pushButton_OK->setText(QCoreApplication::translate("QSettings", "OK", nullptr));
        pushButton_Cancel->setText(QCoreApplication::translate("QSettings", "Cancel", nullptr));
        pushButton_Applay->setText(QCoreApplication::translate("QSettings", "Applay", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QSettings: public Ui_QSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QSETTINGS_H
