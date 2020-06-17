/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QMediaPlayer.h>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_10;
    QWidget *MainBar;
    QHBoxLayout *horizontalLayout_MainBar;
    QPushButton *pushButton_4;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_Minmum;
    QPushButton *pushButton_Maxmum;
    QPushButton *pushButton_Exit;
    QGridLayout *gridLayout;
    QMediaPlayer *screen;
    QSpacerItem *verticalSpacer;
    QWidget *Toolbar;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_Audio;
    QLabel *label_Audio;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_Video;
    QLabel *label_Video;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButton_ShareScreen;
    QLabel *label_ShareScreen;
    QVBoxLayout *verticalLayout_4;
    QPushButton *pushButton_ManageMenber;
    QLabel *label_ManageMenber;
    QVBoxLayout *verticalLayout_9;
    QPushButton *pushButton_Invite;
    QLabel *label_Invite;
    QVBoxLayout *verticalLayout_5;
    QPushButton *pushButton_Talk;
    QLabel *label_Talk;
    QVBoxLayout *verticalLayout_6;
    QPushButton *pushButton_Expression;
    QLabel *label_Expression;
    QVBoxLayout *verticalLayout_7;
    QPushButton *pushButton_Document;
    QLabel *label_Document;
    QVBoxLayout *verticalLayout_8;
    QPushButton *pushButton_Settings;
    QLabel *label_Settings;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1151, 706);
        MainWindow->setMouseTracking(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/icon/logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("/* XiaoTheme, main QApplication palette and QML values */\n"
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
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_10 = new QVBoxLayout(centralwidget);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(5, 1, 5, -1);
        MainBar = new QWidget(centralwidget);
        MainBar->setObjectName(QString::fromUtf8("MainBar"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainBar->sizePolicy().hasHeightForWidth());
        MainBar->setSizePolicy(sizePolicy);
        MainBar->setMaximumSize(QSize(16777215, 25));
        horizontalLayout_MainBar = new QHBoxLayout(MainBar);
        horizontalLayout_MainBar->setSpacing(0);
        horizontalLayout_MainBar->setObjectName(QString::fromUtf8("horizontalLayout_MainBar"));
        horizontalLayout_MainBar->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalLayout_MainBar->setContentsMargins(0, 0, 0, 0);
        pushButton_4 = new QPushButton(MainBar);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setMaximumSize(QSize(32, 32));
        pushButton_4->setIcon(icon);
        pushButton_4->setIconSize(QSize(32, 32));

        horizontalLayout_MainBar->addWidget(pushButton_4);

        label = new QLabel(MainBar);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_MainBar->addWidget(label);

        horizontalSpacer_3 = new QSpacerItem(248, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_MainBar->addItem(horizontalSpacer_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(1);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButton_Minmum = new QPushButton(MainBar);
        pushButton_Minmum->setObjectName(QString::fromUtf8("pushButton_Minmum"));
        pushButton_Minmum->setMaximumSize(QSize(26, 18));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/icon/minimize.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Minmum->setIcon(icon1);
        pushButton_Minmum->setIconSize(QSize(26, 18));

        horizontalLayout_3->addWidget(pushButton_Minmum);

        pushButton_Maxmum = new QPushButton(MainBar);
        pushButton_Maxmum->setObjectName(QString::fromUtf8("pushButton_Maxmum"));
        pushButton_Maxmum->setMaximumSize(QSize(25, 18));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/res/icon/maximize.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Maxmum->setIcon(icon2);
        pushButton_Maxmum->setIconSize(QSize(25, 18));

        horizontalLayout_3->addWidget(pushButton_Maxmum);

        pushButton_Exit = new QPushButton(MainBar);
        pushButton_Exit->setObjectName(QString::fromUtf8("pushButton_Exit"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_Exit->sizePolicy().hasHeightForWidth());
        pushButton_Exit->setSizePolicy(sizePolicy1);
        pushButton_Exit->setMaximumSize(QSize(25, 18));
        pushButton_Exit->setStyleSheet(QString::fromUtf8("QPushButton#pushButton_exit:hover { border - image: url(: /ui/res/btn_close.png);}"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/res/icon/close-white.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Exit->setIcon(icon3);
        pushButton_Exit->setIconSize(QSize(25, 18));

        horizontalLayout_3->addWidget(pushButton_Exit);


        horizontalLayout_MainBar->addLayout(horizontalLayout_3);


        verticalLayout_10->addWidget(MainBar);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        screen = new QMediaPlayer(centralwidget);
        screen->setObjectName(QString::fromUtf8("screen"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(screen->sizePolicy().hasHeightForWidth());
        screen->setSizePolicy(sizePolicy2);
        screen->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(screen, 0, 0, 1, 1);


        verticalLayout_10->addLayout(gridLayout);

        verticalSpacer = new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_10->addItem(verticalSpacer);

        Toolbar = new QWidget(centralwidget);
        Toolbar->setObjectName(QString::fromUtf8("Toolbar"));
        horizontalLayout = new QHBoxLayout(Toolbar);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton_Audio = new QPushButton(Toolbar);
        pushButton_Audio->setObjectName(QString::fromUtf8("pushButton_Audio"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(pushButton_Audio->sizePolicy().hasHeightForWidth());
        pushButton_Audio->setSizePolicy(sizePolicy3);
        pushButton_Audio->setMaximumSize(QSize(128, 128));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/res/icon/ic_lock_ringer_on_alpha.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Audio->setIcon(icon4);
        pushButton_Audio->setIconSize(QSize(48, 48));

        verticalLayout->addWidget(pushButton_Audio);

        label_Audio = new QLabel(Toolbar);
        label_Audio->setObjectName(QString::fromUtf8("label_Audio"));
        label_Audio->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_Audio);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButton_Video = new QPushButton(Toolbar);
        pushButton_Video->setObjectName(QString::fromUtf8("pushButton_Video"));
        pushButton_Video->setMaximumSize(QSize(16777215, 64));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/res/icon/AnswerWithVideo.scale-200.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Video->setIcon(icon5);
        pushButton_Video->setIconSize(QSize(48, 48));

        verticalLayout_2->addWidget(pushButton_Video);

        label_Video = new QLabel(Toolbar);
        label_Video->setObjectName(QString::fromUtf8("label_Video"));
        label_Video->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_Video);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        pushButton_ShareScreen = new QPushButton(Toolbar);
        pushButton_ShareScreen->setObjectName(QString::fromUtf8("pushButton_ShareScreen"));
        pushButton_ShareScreen->setMaximumSize(QSize(16777215, 64));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/res/icon/ic_dialog_usb.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_ShareScreen->setIcon(icon6);
        pushButton_ShareScreen->setIconSize(QSize(48, 48));

        verticalLayout_3->addWidget(pushButton_ShareScreen);

        label_ShareScreen = new QLabel(Toolbar);
        label_ShareScreen->setObjectName(QString::fromUtf8("label_ShareScreen"));
        label_ShareScreen->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_ShareScreen);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        pushButton_ManageMenber = new QPushButton(Toolbar);
        pushButton_ManageMenber->setObjectName(QString::fromUtf8("pushButton_ManageMenber"));
        pushButton_ManageMenber->setMaximumSize(QSize(16777215, 64));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/res/icon/ic_menu_allfriends.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_ManageMenber->setIcon(icon7);
        pushButton_ManageMenber->setIconSize(QSize(48, 48));

        verticalLayout_4->addWidget(pushButton_ManageMenber);

        label_ManageMenber = new QLabel(Toolbar);
        label_ManageMenber->setObjectName(QString::fromUtf8("label_ManageMenber"));
        label_ManageMenber->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_ManageMenber);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        pushButton_Invite = new QPushButton(Toolbar);
        pushButton_Invite->setObjectName(QString::fromUtf8("pushButton_Invite"));
        pushButton_Invite->setMaximumSize(QSize(16777215, 64));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/res/icon/ic_menu_invite.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Invite->setIcon(icon8);
        pushButton_Invite->setIconSize(QSize(48, 48));

        verticalLayout_9->addWidget(pushButton_Invite);

        label_Invite = new QLabel(Toolbar);
        label_Invite->setObjectName(QString::fromUtf8("label_Invite"));
        label_Invite->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(label_Invite);


        horizontalLayout->addLayout(verticalLayout_9);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        pushButton_Talk = new QPushButton(Toolbar);
        pushButton_Talk->setObjectName(QString::fromUtf8("pushButton_Talk"));
        pushButton_Talk->setMaximumSize(QSize(16777215, 64));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/res/icon/ic_menu_start_conversation.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Talk->setIcon(icon9);
        pushButton_Talk->setIconSize(QSize(48, 48));

        verticalLayout_5->addWidget(pushButton_Talk);

        label_Talk = new QLabel(Toolbar);
        label_Talk->setObjectName(QString::fromUtf8("label_Talk"));
        label_Talk->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_Talk);


        horizontalLayout->addLayout(verticalLayout_5);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        pushButton_Expression = new QPushButton(Toolbar);
        pushButton_Expression->setObjectName(QString::fromUtf8("pushButton_Expression"));
        pushButton_Expression->setMaximumSize(QSize(16777215, 64));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/res/icon/ic_menu_emoticons.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Expression->setIcon(icon10);
        pushButton_Expression->setIconSize(QSize(48, 48));

        verticalLayout_6->addWidget(pushButton_Expression);

        label_Expression = new QLabel(Toolbar);
        label_Expression->setObjectName(QString::fromUtf8("label_Expression"));
        label_Expression->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_Expression);


        horizontalLayout->addLayout(verticalLayout_6);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        pushButton_Document = new QPushButton(Toolbar);
        pushButton_Document->setObjectName(QString::fromUtf8("pushButton_Document"));
        pushButton_Document->setMaximumSize(QSize(16777215, 64));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/res/icon/ic_aggregated.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Document->setIcon(icon11);
        pushButton_Document->setIconSize(QSize(48, 48));

        verticalLayout_7->addWidget(pushButton_Document);

        label_Document = new QLabel(Toolbar);
        label_Document->setObjectName(QString::fromUtf8("label_Document"));
        label_Document->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_Document);


        horizontalLayout->addLayout(verticalLayout_7);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        pushButton_Settings = new QPushButton(Toolbar);
        pushButton_Settings->setObjectName(QString::fromUtf8("pushButton_Settings"));
        pushButton_Settings->setMaximumSize(QSize(16777215, 64));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/res/icon/ic_settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Settings->setIcon(icon12);
        pushButton_Settings->setIconSize(QSize(48, 48));

        verticalLayout_8->addWidget(pushButton_Settings);

        label_Settings = new QLabel(Toolbar);
        label_Settings->setObjectName(QString::fromUtf8("label_Settings"));
        label_Settings->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(label_Settings);


        horizontalLayout->addLayout(verticalLayout_8);

        horizontalSpacer_2 = new QSpacerItem(108, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_10->addWidget(Toolbar);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_4->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "Xiao-V1.0.0", nullptr));
        pushButton_Minmum->setText(QString());
        pushButton_Maxmum->setText(QString());
        pushButton_Exit->setText(QString());
        pushButton_Audio->setText(QString());
        label_Audio->setText(QCoreApplication::translate("MainWindow", "\351\237\263\351\242\221", nullptr));
        pushButton_Video->setText(QString());
        label_Video->setText(QCoreApplication::translate("MainWindow", "\350\247\206\351\242\221", nullptr));
        pushButton_ShareScreen->setText(QString());
        label_ShareScreen->setText(QCoreApplication::translate("MainWindow", "\345\205\261\344\272\253\345\261\217\345\271\225", nullptr));
        pushButton_ManageMenber->setText(QString());
        label_ManageMenber->setText(QCoreApplication::translate("MainWindow", "\347\256\241\347\220\206\346\210\220\345\221\230", nullptr));
        pushButton_Invite->setText(QString());
        label_Invite->setText(QCoreApplication::translate("MainWindow", "\351\202\200\350\257\267", nullptr));
        pushButton_Talk->setText(QString());
        label_Talk->setText(QCoreApplication::translate("MainWindow", "\350\201\212\345\244\251", nullptr));
        pushButton_Expression->setText(QString());
        label_Expression->setText(QCoreApplication::translate("MainWindow", "\350\241\250\346\203\205", nullptr));
        pushButton_Document->setText(QString());
        label_Document->setText(QCoreApplication::translate("MainWindow", "\346\226\207\346\241\243", nullptr));
        pushButton_Settings->setText(QString());
        label_Settings->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
