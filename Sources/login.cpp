#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"

Login::Login(Platform *p, MainWindow *parent) :
    QWidget(parent),
    m_platform(p),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    m_type=0;
    register_ui=NULL;
    isLogin = false;
    connect(this->ui->_signIn,SIGNAL(clicked(bool)),parent,SLOT(isLogin()));

    //使用自定义标题栏
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar=new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle("酒店预订服务与管理系统");
    setWindowIcon(QIcon(":/app1.png"));
    resize(510,389);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

Login::~Login()
{
    delete ui;
    delete register_ui;
}

void Login::setup(){
    ui->copyRight->setStyleSheet("QLabel{color:rgb(0, 155, 188);}");
    QRegExp phonenumberExp("1[0-9]{10}");
    ui->_phonenumberInput->setValidator(new QRegExpValidator(phonenumberExp));
    QRegExp passwordExp("[A-Za-z0-9]{16}");
    ui->_passwordInput->setValidator(new QRegExpValidator(passwordExp));
    ui->_passwordInput->setMaxLength(16);
    ui->_passwordInput->setEchoMode(QLineEdit::Password);

}

void Login::on_comboBox_currentIndexChanged(int index)
{
    m_type=index;
    switch (index) {
    case 0:
        ui->comboLabel->setText("当前用户身份：顾客");
        break;
    case 1:
        ui->comboLabel->setText("当前用户身份：平台管理员");
        break;
    case 2:
        ui->comboLabel->setText("当前用户身份：酒店管理员");
        break;
    default:
        break;
    }
}

void Login::on__signIn_clicked()
{
    ui->_errorMsg1->clear();
    ui->_errorMsg2->clear();
    _phonenumber=ui->_phonenumberInput->text();
    _password=ui->_passwordInput->text();
    if(!_phonenumber.count())
    {
        ui->_errorMsg1->setText("请输入用户名");
    }
    else if(_phonenumber.count()!=11)
    {
        ui->_errorMsg1->setText("手机号不正确");
    }
    else
    {
        if(!m_platform->registerSearch(_phonenumber,1))
        {
            ui->_errorMsg1->setText("手机号未注册");
            ui->_passwordInput->clear();
        }
        else if(!m_platform->checkValid(_phonenumber,_password,m_type))
        {
            ui->_errorMsg2->setText("密码不正确或用户身份不匹配");
            ui->_passwordInput->clear();
        }
        else
        {
            ui->_phonenumberInput->clear();
            ui->_passwordInput->clear();
            this->close();
            isLogin = true;
        }
    }
}



void Login::on__signUp_clicked()
{
    if(register_ui!=NULL)
    {
        delete register_ui;
        register_ui=NULL;
    }
    register_ui=new Register(m_platform);
    register_ui->setup();
    register_ui->show();
}
