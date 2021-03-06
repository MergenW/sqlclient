#include "scriptdlg.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>

scriptDlg::scriptDlg()
{
    islogin = false;
    label0 = new QLabel;
    label0->setText(tr("请输入SQL"));
    textEditSQL = new QTextEdit;
    okBtn = new QPushButton;
    okBtn->setText(tr("执行"));
    cancelBtn = new QPushButton;
    cancelBtn->setText(tr("取消"));
    QGridLayout *layout1 = new QGridLayout(this);
    layout1->addWidget(label0,0,0);
    layout1->addWidget(textEditSQL,0,1);

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(okBtn);
    layout2->addWidget(cancelBtn);

    layout1->addLayout(layout2,1,1);

    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtnOnclick()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtnOnclick()));
}

void scriptDlg::okBtnOnclick()
{
    //TextEdit没有text函数，得到TextEdit当中用户输入内容的函数是toPlainText()
    SQL =textEditSQL->toPlainText();
    islogin =true;
    close();
}

void scriptDlg::cancelBtnOnclick()
{
    close();
}
