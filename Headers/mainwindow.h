#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"customerwidget.h"
#include"login.h"
#include"register.h"
#include"hotelmanagerwidget.h"
#include"adminwidget.h"

class Platform;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Platform *p, QWidget *parent = 0);
    ~MainWindow();

    void setup();

public slots:
    void isLogin();
    void LogOut();
private:
    Ui::MainWindow *ui;

    Platform *m_platform;
    Login *login_ui;
    CustomerWidget *cust_ui;
    HotelManagerWidget *hotel_ui;
    AdminWidget *admin_ui;
};

#endif // MAINWINDOW_H
