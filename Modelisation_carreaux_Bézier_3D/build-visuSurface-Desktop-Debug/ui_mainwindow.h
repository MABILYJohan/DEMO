/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <myopenglwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionA_propos;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QFrame *line_5;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QFrame *line_3;
    QLabel *label_7;
    QCheckBox *checkBox_PtCtrl;
    QCheckBox *checkBox_Aretes;
    QCheckBox *checkBox_Points;
    QFrame *line;
    QLabel *label_8;
    QCheckBox *checkBox_Carreaux;
    QCheckBox *checkBoxTrianguler;
    QCheckBox *checkBox_surface;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QSlider *horizontalSliderU;
    QSpinBox *spinBoxU;
    QLabel *label_6;
    QSlider *horizontalSliderV;
    QSpinBox *spinBoxV;
    QFrame *line_4;
    QLabel *label;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBoxU;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBoxV;
    QFrame *line_8;
    QSpacerItem *verticalSpacer;
    QFrame *line_6;
    QFrame *line_2;
    myOpenGLWidget *openGLWidget;
    QMenuBar *menuBar;
    QMenu *menuAide;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::ApplicationModal);
        MainWindow->resize(904, 643);
        actionA_propos = new QAction(MainWindow);
        actionA_propos->setObjectName(QStringLiteral("actionA_propos"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        line_5 = new QFrame(centralWidget);
        line_5->setObjectName(QStringLiteral("line_5"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(line_5->sizePolicy().hasHeightForWidth());
        line_5->setSizePolicy(sizePolicy);
        line_5->setFrameShape(QFrame::VLine);
        line_5->setFrameShadow(QFrame::Sunken);

        horizontalLayout_2->addWidget(line_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line_3);

        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_7);

        checkBox_PtCtrl = new QCheckBox(centralWidget);
        checkBox_PtCtrl->setObjectName(QStringLiteral("checkBox_PtCtrl"));

        verticalLayout_2->addWidget(checkBox_PtCtrl);

        checkBox_Aretes = new QCheckBox(centralWidget);
        checkBox_Aretes->setObjectName(QStringLiteral("checkBox_Aretes"));
        checkBox_Aretes->setChecked(true);

        verticalLayout_2->addWidget(checkBox_Aretes);

        checkBox_Points = new QCheckBox(centralWidget);
        checkBox_Points->setObjectName(QStringLiteral("checkBox_Points"));

        verticalLayout_2->addWidget(checkBox_Points);


        verticalLayout_3->addLayout(verticalLayout_2);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_8);

        checkBox_Carreaux = new QCheckBox(centralWidget);
        checkBox_Carreaux->setObjectName(QStringLiteral("checkBox_Carreaux"));
        checkBox_Carreaux->setChecked(true);

        verticalLayout_3->addWidget(checkBox_Carreaux);

        checkBoxTrianguler = new QCheckBox(centralWidget);
        checkBoxTrianguler->setObjectName(QStringLiteral("checkBoxTrianguler"));
        checkBoxTrianguler->setChecked(true);

        verticalLayout_3->addWidget(checkBoxTrianguler);

        checkBox_surface = new QCheckBox(centralWidget);
        checkBox_surface->setObjectName(QStringLiteral("checkBox_surface"));

        verticalLayout_3->addWidget(checkBox_surface);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        horizontalSliderU = new QSlider(centralWidget);
        horizontalSliderU->setObjectName(QStringLiteral("horizontalSliderU"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(horizontalSliderU->sizePolicy().hasHeightForWidth());
        horizontalSliderU->setSizePolicy(sizePolicy1);
        horizontalSliderU->setMinimum(1);
        horizontalSliderU->setMaximum(100);
        horizontalSliderU->setValue(1);
        horizontalSliderU->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSliderU, 0, 1, 1, 1);

        spinBoxU = new QSpinBox(centralWidget);
        spinBoxU->setObjectName(QStringLiteral("spinBoxU"));
        spinBoxU->setMinimum(1);
        spinBoxU->setMaximum(100);
        spinBoxU->setValue(4);

        gridLayout->addWidget(spinBoxU, 0, 2, 1, 1);

        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 1, 0, 1, 1);

        horizontalSliderV = new QSlider(centralWidget);
        horizontalSliderV->setObjectName(QStringLiteral("horizontalSliderV"));
        sizePolicy1.setHeightForWidth(horizontalSliderV->sizePolicy().hasHeightForWidth());
        horizontalSliderV->setSizePolicy(sizePolicy1);
        horizontalSliderV->setMinimum(1);
        horizontalSliderV->setMaximum(100);
        horizontalSliderV->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSliderV, 1, 1, 1, 1);

        spinBoxV = new QSpinBox(centralWidget);
        spinBoxV->setObjectName(QStringLiteral("spinBoxV"));
        spinBoxV->setMinimum(1);
        spinBoxV->setMaximum(100);
        spinBoxV->setValue(4);

        gridLayout->addWidget(spinBoxV, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);


        verticalLayout_3->addLayout(verticalLayout);


        verticalLayout_4->addLayout(verticalLayout_3);

        line_4 = new QFrame(centralWidget);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout_4->addWidget(line_4);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        doubleSpinBoxU = new QDoubleSpinBox(centralWidget);
        doubleSpinBoxU->setObjectName(QStringLiteral("doubleSpinBoxU"));
        doubleSpinBoxU->setMaximum(1);
        doubleSpinBoxU->setSingleStep(0.01);

        gridLayout_2->addWidget(doubleSpinBoxU, 0, 1, 1, 1);

        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        doubleSpinBoxV = new QDoubleSpinBox(centralWidget);
        doubleSpinBoxV->setObjectName(QStringLiteral("doubleSpinBoxV"));
        doubleSpinBoxV->setMaximum(1);
        doubleSpinBoxV->setSingleStep(0.01);

        gridLayout_2->addWidget(doubleSpinBoxV, 1, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout_2);

        line_8 = new QFrame(centralWidget);
        line_8->setObjectName(QStringLiteral("line_8"));
        line_8->setFrameShape(QFrame::HLine);
        line_8->setFrameShadow(QFrame::Sunken);

        verticalLayout_4->addWidget(line_8);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        line_6 = new QFrame(centralWidget);
        line_6->setObjectName(QStringLiteral("line_6"));
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);

        verticalLayout_4->addWidget(line_6);


        horizontalLayout->addLayout(verticalLayout_4);

        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        sizePolicy.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
        line_2->setSizePolicy(sizePolicy);
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line_2);

        openGLWidget = new myOpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy2);
        openGLWidget->setMinimumSize(QSize(500, 350));

        horizontalLayout->addWidget(openGLWidget);


        horizontalLayout_2->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 904, 22));
        menuAide = new QMenu(menuBar);
        menuAide->setObjectName(QStringLiteral("menuAide"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuAide->menuAction());
        menuAide->addAction(actionA_propos);

        retranslateUi(MainWindow);
        QObject::connect(horizontalSliderU, SIGNAL(valueChanged(int)), spinBoxU, SLOT(setValue(int)));
        QObject::connect(horizontalSliderV, SIGNAL(valueChanged(int)), spinBoxV, SLOT(setValue(int)));
        QObject::connect(spinBoxU, SIGNAL(valueChanged(int)), horizontalSliderU, SLOT(setValue(int)));
        QObject::connect(spinBoxV, SIGNAL(valueChanged(int)), horizontalSliderV, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "TP M1GIG", Q_NULLPTR));
        actionA_propos->setText(QApplication::translate("MainWindow", "A propos", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "Options", Q_NULLPTR));
        checkBox_PtCtrl->setText(QApplication::translate("MainWindow", "Polygone de contr\303\264le", Q_NULLPTR));
        checkBox_Aretes->setText(QApplication::translate("MainWindow", "Aretes", Q_NULLPTR));
        checkBox_Points->setText(QApplication::translate("MainWindow", "Sommets", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Carreaux", Q_NULLPTR));
        checkBox_Carreaux->setText(QApplication::translate("MainWindow", "Afficher", Q_NULLPTR));
        checkBoxTrianguler->setText(QApplication::translate("MainWindow", "Trianguler", Q_NULLPTR));
        checkBox_surface->setText(QApplication::translate("MainWindow", "Surface pleine", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "nombre de param\303\250tres", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "u", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "v", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Afficher Point", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "param U", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "param V", Q_NULLPTR));
        menuAide->setTitle(QApplication::translate("MainWindow", "Aide", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
