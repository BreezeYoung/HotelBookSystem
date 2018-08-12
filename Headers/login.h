#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "register.h"
#include "title_bar.h"
#include "commonhelper.h"
#include "customerwidget.h"
#include "platform.h"

class MainWindow;

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(Platform *p, MainWindow *parent = 0);
    ~Login();

    void setup();

    QString getPhonenumber() const
    {
        return _phonenumber;
    }
    int getType() const
    {
        return m_type;
    }
    bool getIsLogin() const
    {
        return isLogin;
    }


private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on__signIn_clicked();
    void on__signUp_clicked();

private:
    Ui::Login *ui;               //登陆界面
    Register *register_ui;       //注册界面

    int m_type;
    QString _phonenumber;
    QString _password;
    bool isLogin;                //标志是否登陆成功
    Platform * m_platform;
};

#endif // LOGIN_H
