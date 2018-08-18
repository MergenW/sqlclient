#include "mainwindow.h"
#include <QMessageBox>
#include <QIcon>
#include <QMdiSubWindow>
#include <QStandardItemModel>
#include <QTableView>
#include "logindlg.h"
#include "scriptdlg.h"

//所有有关mysql API的函数调用，都是在mymysql这个类里实现的
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("mysql客户端数据管理系统"));
    setWindowIcon(QIcon("main.png")); //设置光标

    //QMdiArea只能在QMainWindow当中使用，是用来实现多文档界面的必备控件
    mdiArea = new QMdiArea;
    setCentralWidget(mdiArea);

    //当子窗口的范围超过父窗口的显示范围时，父窗口自动添加横向滚动条
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    createActions(); //建立行为的函数
    createMenus(); //建立菜单的函数

    //设置主窗口背景颜色或图片
    mdiArea->setBackground(Qt::NoBrush);
    //引号内用的CSS
    //mdiArea->setStyleSheet("background-image: url(1.jpg);");//背景设置为1.jpg，子窗口大小可调整
    mdiArea->setStyleSheet("border-image: url(1.jpg)");
}

MainWindow::~MainWindow()
{

}


//closeEvent函数是在一个widget退出之前被自动调用的函数,
//参数event的accept方法代表接受退出，ignore方法代表不接受退出
void MainWindow::closeEvent(QCloseEvent *event)
{
    //通过question返回值来判断用户点击的到底是yes还是no
    QMessageBox::StandardButton button = QMessageBox::question(this,tr("退出")
                                         ,QString(tr("是否退出程序")),QMessageBox::Yes
                                                               | QMessageBox::No);
    if(button == QMessageBox::Yes)
    {
        event->accept();//如果用户点击yes,接受退出
    }
    else
    {
        event->ignore();//否则不接受退出
    }
}

void MainWindow::createMenus()
{
    adminMenu = menuBar()->addMenu(tr("管理"));
    adminMenu->addAction(loginAction);
    adminMenu->addAction(logoutAction);
    adminMenu->addSeparator();
    adminMenu->addAction(exitAction);

    dataMenu = menuBar()->addMenu(tr("数据"));
    dataMenu->addSeparator();
    dataMenu->addAction(scriptAction);

    windowMenu = menuBar()->addMenu(tr("窗口"));
    windowMenu->addAction(cascadeAction);
    windowMenu->addAction(tileAction);

    helpMenu =menuBar()->addMenu((tr("帮助")));
    helpMenu->addAction(helpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
}

void MainWindow::createActions()
{
    loginAction = new QAction(tr("登录"),this);
    loginAction->setShortcut(tr("Ctrl+u"));
    connect(loginAction,SIGNAL(triggered()),this,SLOT(on_login()));

    logoutAction = new QAction(tr("注销"),this);
    logoutAction->setShortcut(tr("Ctrl+b"));
    connect(logoutAction,SIGNAL(triggered()),this,SLOT(on_logout()));

    exitAction = new QAction(tr("退出"),this);
    exitAction->setShortcut(tr("Ctrl+w"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(on_exit()));

    scriptAction = new QAction(tr("执行脚本"),this);
    scriptAction->setShortcut((tr("Ctrl+p")));
    scriptAction->setEnabled(false);
    connect(scriptAction,SIGNAL(triggered()),this,SLOT(on_script()));

    cascadeAction = new QAction(tr("层叠窗口"),this);
    cascadeAction->setShortcut(tr("Ctrl+Q"));
    connect(cascadeAction,SIGNAL(triggered()),this,SLOT(cascadeSubWindows()));

    tileAction = new QAction(tr("并列窗口"),this);
    tileAction->setShortcut(tr("Ctrl+R"));
    connect(tileAction,SIGNAL(triggered()),this,SLOT(tileSubWindows()));

    helpAction = new QAction(tr("帮助"),this);
    helpAction->setShortcut(tr("Ctrl+S"));
    connect(helpAction, SIGNAL(triggered()),this, SLOT(on_help()));

    aboutAction = new QAction(tr("关于"),this);
    aboutAction->setShortcut(tr("Ctrl+T"));
    connect(aboutAction, SIGNAL(triggered()),this, SLOT(on_about()));



}

void MainWindow::showsub()
{
    QWidget *w1 = new QWidget;
    w1->setAttribute(Qt::WA_DeleteOnClose);//关闭这个widget的时候，自动将这个widget释放掉
    mdiArea->addSubWindow(w1);
    w1->setWindowTitle("SQL");
    w1->setStyleSheet("border-image: url(2.jpg)");

    w1->show();
    mdiArea->activeSubWindow()->resize(width()-100,height()-100);//设置widget窗口的大小
}

void MainWindow::showview()
{
    QStandardItemModel *model =new QStandardItemModel(5,3);

    model->setHeaderData(0,Qt::Horizontal,"name"); //设置model的列头名称
    //设置model每个单元格的内容
    model->setData(model->index(0,0,QModelIndex()),"AAA");
    model->setData(model->index(1,0,QModelIndex()),"BBB");
    QTableView *view1 = new QTableView;
    //view在close的时候会自动delete，这时如果view有model的话，这个model会被view自动释放
    view1->setAttribute(Qt::WA_DeleteOnClose);
    mdiArea->addSubWindow(view1);
    view1->setStyleSheet("border-image: url(2.jpg)");
    //view1继承自widget，若无model则view1不会显示任何数据, 数据来自于model
    view1->setModel(model);
    view1->show();
    mdiArea->activeSubWindow()->resize(width()-100,height()-100);
}

void MainWindow::script_msg(const char *SQL)
{
    int res;
    if (strncmp(SQL,"SELECT",6) == 0 || strncmp(SQL,"select",6) == 0
            ||strncmp(SQL,"SHOW",4) == 0 || strncmp(SQL,"show",4) == 0
            ||strncmp(SQL,"DESC",4) == 0 || strncmp(SQL,"desc",4) == 0)
    {
        QStandardItemModel *model;
        res = db.sql_open(SQL,&model);

        QTableView *view1 = new QTableView;
        //view在close的时候会自动delete，这时如果view有model的话，这个model会被view自动释放
        view1->setAttribute(Qt::WA_DeleteOnClose);
        mdiArea->addSubWindow(view1);
        view1->setStyleSheet("border-image: url(2.jpg)");

        //view1继承自widget，若无model则view1不会显示任何数据, 数据来自于model
        view1->setModel(model);
        view1->show();
        mdiArea->activeSubWindow()->resize(width()-100,height()-100);
    }
    else
    {
        res = db.sql_exec(SQL);
    }
    if(res == -1)
    {
        QMessageBox::information(this,tr("失败"),db.geterror());
    }
    else
    {
        QMessageBox::information(this,tr("提示"),tr("执行成功"));
    }
}

void MainWindow::on_login()
{
    loginDlg dlg;//将对话框实例化
    dlg.exec(); //调用exec产生一个模式对话框，程序在exec这个地方阻塞了，所以dlg这个栈变量一直存在

    if(dlg.islogin) //代表用户点击了登录按钮
    {
        //将QString转化成char *
        int res = db.sql_connect(dlg.hostip.toStdString().data(),
                       dlg.userid.toStdString().data(),
                       dlg.passwd.toStdString().data(),
                       dlg.dbname.toStdString().data());
        if (res == -1)
        {
            QMessageBox::information(this,"登录失败",db.geterror());
        }
        else
        {
            QMessageBox::information(this,"登录成功","登录成功");
            scriptAction->setEnabled(true);
        }

    }
}

void MainWindow::on_logout()
{
    QMessageBox::StandardButton button =  QMessageBox::question(this,tr("退出")
                                          ,QString(tr("是否注销用户"))
                                          ,QMessageBox::Yes | QMessageBox::No);
    if(button == QMessageBox::Yes)
    {
         db.sql_disconnect();
         scriptAction->setEnabled(false);
    }
}

void MainWindow::on_exit()
{
    close();
}

void MainWindow::on_script()
{
    scriptDlg dlg;
    dlg.exec();
    if(dlg.islogin)
    {
        script_msg(dlg.SQL.toStdString().data());
    }
}

void MainWindow::on_help()
{
    QMessageBox::information(this,tr("帮助"),tr("自行学习"));
}

void MainWindow::on_about()
{
    QMessageBox::about(this,tr("关于数据管理系统"),tr("中国电信CDMA无线基站管理系统\n\n作者：WR\n2018版权所有\n"));
}

void MainWindow::cascadeSubWindows()
{
    mdiArea->cascadeSubWindows();
}

void MainWindow::tileSubWindows()
{
    mdiArea->tileSubWindows();
}
