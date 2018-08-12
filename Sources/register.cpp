#include "register.h"
#include "ui_register.h"
#include "title_bar.h"
#include "user.h"


Register::Register(Platform * platform, QWidget *parent) :
    QWidget(parent),
    m_platform(platform),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    _type=0;

    //使用自定义标题栏
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar=new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle("酒店预订服务与管理系统");
    setWindowIcon(QIcon(":/app1.png"));
    resize(413,446);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

Register::~Register()
{
    delete ui;
}

void Register::setup(){
    QRegExp phonenumberExp("1[0-9]{10}");
    ui->_phonenumberInput->setValidator(new QRegExpValidator(phonenumberExp));
    QRegExp usernameExp("[A-Za-z0-9]{20}");
    ui->_usernameInput->setValidator(new QRegExpValidator(usernameExp));
    QRegExp passwordExp("[A-Za-z0-9]{16}");
    ui->_passwordInput->setValidator(new QRegExpValidator(passwordExp));
    ui->_usernameInput->setMaxLength(20);
    ui->_passwordInput->setMaxLength(16);
    ui->_passwordInput->setEchoMode(QLineEdit::Password);
    ui->_rePasswordInput->setMaxLength(16);
    ui->_rePasswordInput->setEchoMode(QLineEdit::Password);
}


void Register::on_pushButton_clicked()
{
    EditFlag p;
    ui->warningMsg1->clear();
    ui->warningMsg2->clear();
    ui->warningMsg3->clear();
    ui->warningMsg4->clear();
    _username=ui->_usernameInput->text();
    _phonenumber=ui->_phonenumberInput->text();
    _password=ui->_passwordInput->text();
    _rePassword=ui->_rePasswordInput->text();
    if(!_username.count())
    {
        ui->warningMsg4->setText("请输入用户名");
    }
    if(!_phonenumber.count())
    {
        ui->warningMsg1->setText("请输入手机号");
        ui->_passwordInput->clear();
        ui->_rePasswordInput->clear();
    }
    else if(_phonenumber.count()!=11)
    {
        ui->warningMsg1->setText("该手机号不是11位");
        ui->_passwordInput->clear();
        ui->_rePasswordInput->clear();
    }
    if(_password.count()<6)
    {
        ui->warningMsg2->setText("请输入6-16位密码");
    }
    else if(_password.count()>=6&&_rePassword.count()<6)
    {
        ui->warningMsg3->setText("请再次输入6-16位密码");
        ui->_rePasswordInput->clear();
    }
    else if(_password.count()>=6&&_rePassword.count()>=6&&ui->_passwordInput->text()!=ui->_rePasswordInput->text())
    {
        ui->warningMsg3->setText("密码不一致");
        ui->_rePasswordInput->clear();
    }
    else
    {
        if(m_platform->registerSearch(_phonenumber,1))
        {
            ui->warningMsg1->setText("该手机号已被注册");
        }
        else if(m_platform->registerSearch(_username,2))
        {
            ui->warningMsg4->setText("该用户名已被使用");
        }
        else
        {
            if(_type==0)
            {
                p.id=m_platform->getMaxID(_type);
                p.tbname="customer";
                p.operType=3;
                m_platform->m_customer[p.id]=new Customer(p.id,_username,_phonenumber,_password,0,0);
                m_platform->addOperatingWord(p);
            }
            else if(_type==2)
            {
                p.id=m_platform->getMaxID(_type);
                p.tbname="hotelrepre";
                p.operType=3;
                m_platform->m_hotelrepre[p.id]=new MyUser(p.id,_username,_phonenumber,_password);
                m_platform->addOperatingWord(p);
            }
            ui->_usernameInput->clear();
            ui->_phonenumberInput->clear();
            ui->_passwordInput->clear();
            ui->_rePasswordInput->clear();
            this->close();
        }
    }
}

void Register::on_comboBox_currentIndexChanged(int index)
{
    if(index==0)
    {
        _type=index;
    }
    else if(index==1)
    {
        _type=2;
    }
}
