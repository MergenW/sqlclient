#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class loginDlg : public QDialog
{
    Q_OBJECT
public:
    loginDlg();
private:
    QLabel *label0;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;

    QLineEdit *lineEditUserID;
    QLineEdit *lineEditPasswd;
    QLineEdit *lineEditDBName;
    QLineEdit *lineEditHostIP;

    QPushButton *loginBtn;
    QPushButton *logoutBtn;

private slots:
    void loginBtnOnclick();
    void logoutBtnOnclick();
public:
    QString userid;
    QString passwd;
    QString dbname;
    QString hostip;
    bool islogin;
};

#endif // LOGINDLG_H
