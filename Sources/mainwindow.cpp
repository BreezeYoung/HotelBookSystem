#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "title_bar.h"
MainWindow::MainWindow(Platform *p, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_platform(p)
{
    ui->setupUi(this);
    login_ui=NULL;
    cust_ui=NULL;
    hotel_ui=NULL;
    admin_ui=NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
    if(login_ui!=NULL)
    {
        delete login_ui;
    }
    if(cust_ui!=NULL)
    {
        delete cust_ui;
    }
    if(hotel_ui!=NULL)
    {
        delete hotel_ui;
    }
    if(admin_ui!=NULL)
    {
        delete admin_ui;
    }
}
void MainWindow::setup()
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setWindowTitle("酒店预订服务与管理系统");
    setWindowIcon(QIcon(":/app1.png"));

    resize(510,389);
    this->show();
    login_ui=new Login(m_platform,this);
    setCentralWidget(login_ui);
    login_ui->setup();
    login_ui->show();
}

void MainWindow::isLogin()
{
    if(login_ui->getIsLogin())
    {
        if(login_ui->getType()==0)
        {
            cust_ui=new CustomerWidget(m_platform,this);
            setCentralWidget(cust_ui);
            cust_ui->setPhonenumber(login_ui->getPhonenumber());
            resize(1052,705);
            cust_ui->setup();
            delete login_ui;
            login_ui=NULL;
            cust_ui->show();
        }
        else if(login_ui->getType()==1)
        {
            admin_ui = new AdminWidget(m_platform,this);
            setCentralWidget(admin_ui);
            resize(974,673);
            admin_ui->setPhoneNumber(login_ui->getPhonenumber());
            admin_ui->setup();
            delete login_ui;
            login_ui=NULL;
            admin_ui->show();

        }
        else if(login_ui->getType()==2)
        {
            hotel_ui=new HotelManagerWidget(m_platform,this);
            setCentralWidget(hotel_ui);
            resize(1052,705);
            hotel_ui->setPhonenumber(login_ui->getPhonenumber());
            hotel_ui->setup();
            delete login_ui;
            login_ui=NULL;
            hotel_ui->show();
        }
    }
}

void MainWindow::LogOut()
{
    if(this->centralWidget()==cust_ui)
    {
        cust_ui->close();
        delete cust_ui;
        cust_ui=NULL;
        login_ui=new Login(m_platform,this);
        resize(510,389);
        setCentralWidget(login_ui);
        login_ui->setup();
        login_ui->show();
    }
    else if(this->centralWidget()==hotel_ui)
    {
        hotel_ui->close();
        delete hotel_ui;
        hotel_ui=NULL;
        login_ui=new Login(m_platform,this);
        resize(510,389);
        setCentralWidget(login_ui);
        login_ui->setup();
        login_ui->show();
    }
    else if(this->centralWidget()==admin_ui)
    {
        admin_ui->close();
        delete admin_ui;
        admin_ui=NULL;
        login_ui = new Login(m_platform,this);
        resize(510,389);
        login_ui->setup();
        login_ui->show();
    }
}
