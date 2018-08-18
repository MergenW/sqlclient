#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QCloseEvent>
#include <QMdiArea>
#include <QStandardItemModel>
#include "mymysql.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);//函数重写

private:
    void createMenus();
    void createActions();
    void showsub();
    void showview();
    void script_msg(const char *SQL);
    QMdiArea *mdiArea;
    QMenu *adminMenu;
    QMenu *dataMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;

    QAction *loginAction;
    QAction *logoutAction;
    QAction *exitAction;

    QAction *scriptAction;

    QAction *cascadeAction; //层叠
    QAction *tileAction; //并列

    QAction *helpAction;
    QAction *aboutAction;

     mymysql db;
private slots:
    void on_login();
    void on_logout();
    void on_exit();
    void on_script();
    void on_help();
    void on_about();

    void cascadeSubWindows();
    void tileSubWindows();
};



#endif // MAINWINDOW_H
