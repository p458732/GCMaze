/********************************************************************************
** Form generated from reading UI file 'maze.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAZE_H
#define UI_MAZE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#include <openglwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MazeClass
{
public:
    QAction *actionRead_Maze;
    QAction *actionExit;
    QWidget *centralWidget;
    OpenGLWidget *widget;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MazeClass)
    {
        if (MazeClass->objectName().isEmpty())
            MazeClass->setObjectName(QString::fromUtf8("MazeClass"));
        MazeClass->resize(382, 415);
        MazeClass->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        actionRead_Maze = new QAction(MazeClass);
        actionRead_Maze->setObjectName(QString::fromUtf8("actionRead_Maze"));
        actionExit = new QAction(MazeClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralWidget = new QWidget(MazeClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        widget = new OpenGLWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 1200, 600));
        widget->setLayoutDirection(Qt::LeftToRight);
        MazeClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MazeClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 382, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MazeClass->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionRead_Maze);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);

        retranslateUi(MazeClass);

        QMetaObject::connectSlotsByName(MazeClass);
    } // setupUi

    void retranslateUi(QMainWindow *MazeClass)
    {
        MazeClass->setWindowTitle(QCoreApplication::translate("MazeClass", "Project 2 Maze ", nullptr));
        actionRead_Maze->setText(QCoreApplication::translate("MazeClass", "Read Maze", nullptr));
        actionExit->setText(QCoreApplication::translate("MazeClass", "Exit", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MazeClass", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MazeClass: public Ui_MazeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAZE_H
