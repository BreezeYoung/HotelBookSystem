#include "adminwidget.h"
#include "ui_adminwidget.h"
#include "commonhelper.h"
#include "platform.h"
#include "mainwindow.h"
#include<QMessageBox>

AdminWidget::AdminWidget(Platform *p,MainWindow *parent) :
    QWidget(parent),
    m_platform(p),
    ui(new Ui::AdminWidget)
{
    ui->setupUi(this);
    CommonHelper::setTitleBar(this);
    resize(974,673);
    connect(ui->exitButton,SIGNAL(clicked(bool)),parent,SLOT(LogOut()));
    connect(ui->hotelTableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(buttonRespond(int,int)));
}

AdminWidget::~AdminWidget()
{
    delete ui;
}

void AdminWidget::setupHotelPage()
{
    ui->hotelTableWidget->clearContents();
    int rows=ui->hotelTableWidget->rowCount();
    for(int i=rows;i>0;i--)
    {
        ui->hotelTableWidget->removeRow(i-1);
    }
    int row = 0;
    int hotelid;
    QMap<int, Hotel *>::iterator it;
    for(it=m_platform->m_hotel.begin();it!=m_platform->m_hotel.end();it++)
    {
        ui->hotelTableWidget->insertRow(row);
        ui->hotelTableWidget->setItem(row,0,new QTableWidgetItem(QString::number(it.value()->getID())));
        ui->hotelTableWidget->setItem(row,1,new QTableWidgetItem(it.value()->getHotelName()));
        ui->hotelTableWidget->setItem(row,2,new QTableWidgetItem(it.value()->getCity()));
        ui->hotelTableWidget->setItem(row,3,new QTableWidgetItem(it.value()->getDescription()));
        if(it.value()->getIsVerify())
        {
            ui->hotelTableWidget->setItem(row,4,new QTableWidgetItem(QString("已核实")));
        }
        else
        {
            ui->hotelTableWidget->setItem(row,4,new QTableWidgetItem(QString("待核实")));
        }
        hotelid=it.value()->getID();
        ui->hotelTableWidget->setItem(row,5,new QTableWidgetItem(QString::number(m_platform->getCommentGrade(hotelid),'f',1)));
        row++;
    }
}

void AdminWidget::setupInfoPage()
{
    ui->usernameEdit->setText(m_username);
    ui->usernameEdit->setReadOnly(true);
    ui->phonenumberEdit->setText(m_phonenumber);
    ui->phonenumberEdit->setReadOnly(true);
    ui->passwordEdit->setText(m_password);
    ui->passwordEdit->setReadOnly(true);
    ui->saveInfoButton->setEnabled(false);
}

void AdminWidget::setupUserPage()
{
    ui->userTableWidget->clearContents();
    int rows=ui->userTableWidget->rowCount();
    for(int i=rows;i>0;i--)
    {
        ui->userTableWidget->removeRow(i-1);
    }
    rows = 0;
    QMap<int,MyUser *>::iterator it;
    QMap<int,Customer *>::iterator it1;
    for(it=m_platform->m_hotelrepre.begin();it!=m_platform->m_hotelrepre.end();it++)
    {
        ui->userTableWidget->insertRow(rows);
        ui->userTableWidget->setItem(rows,0,new QTableWidgetItem(it.value()->getUsername()));
        ui->userTableWidget->setItem(rows,1,new QTableWidgetItem(it.value()->getPhoneNumber()));
        ui->userTableWidget->setItem(rows,2,new QTableWidgetItem(it.value()->getPassword()));
        ui->userTableWidget->setItem(rows,3,new QTableWidgetItem(QString("酒店管理员")));
        rows++;
    }
    for(it1=m_platform->m_customer.begin();it1!=m_platform->m_customer.end();it1++)
    {
        ui->userTableWidget->insertRow(rows);
        ui->userTableWidget->setItem(rows,0,new QTableWidgetItem(it1.value()->getUsername()));
        ui->userTableWidget->setItem(rows,1,new QTableWidgetItem(it1.value()->getPhoneNumber()));
        ui->userTableWidget->setItem(rows,2,new QTableWidgetItem(it1.value()->getPassword()));
        ui->userTableWidget->setItem(rows,3,new QTableWidgetItem(QString("顾客")));
        rows++;
    }
}

void AdminWidget::setup()
{
    //个人数据初始化
    QMap<int,MyUser *>::iterator it;
    for(it=m_platform->m_admin.begin();it!=m_platform->m_admin.end();it++)
    {
        if(it.value()->getPhoneNumber()==m_phonenumber)
        {
            break;
        }
    }
    m_password = it.value()->getPassword();
    m_id = it.key();
    m_username = it.value()->getUsername();
    ui->welcomeMsg1->setText("欢迎!");
    ui->welcomeMsg2->setText(QString("用户 %1").arg(m_username));
    ui->hotelTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->hotelTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->hotelTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->userTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->userTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->userTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    //个人信息页面初始化
    setupInfoPage();

    //管理酒店页面初始化
    setupHotelPage();

    //查看用户页面初始化
    setupUserPage();

    ui->stackedWidget->setCurrentIndex(0);
    ui->passButton->setEnabled(false);
    ui->reverifyButton->setEnabled(false);
}

void AdminWidget::on_editInfoButton_clicked()
{
    ui->editInfoButton->setEnabled(false);
    ui->saveInfoButton->setEnabled(true);
    ui->usernameEdit->setReadOnly(false);
    ui->passwordEdit->setReadOnly(false);
    ui->phonenumberEdit->setReadOnly(false);
}

void AdminWidget::on_saveInfoButton_clicked()
{
    EditFlag p;
    int warningTimes = 0;      //用于检查是否修改成功
    if(ui->phonenumberEdit->text().count()<11)
    {
        QMessageBox::warning(this,"Warning","非11位有效的手机号");
        ui->phonenumberEdit->setText(m_phonenumber);
        warningTimes++;
    }
    else if(m_platform->registerSearch(ui->phonenumberEdit->text(),1)&&ui->phonenumberEdit->text()!=m_phonenumber)
    {
        QMessageBox::warning(this,"Warning","该手机号已被注册");
        ui->phonenumberEdit->setText(m_phonenumber);
        warningTimes++;
    }
    if(m_platform->registerSearch(ui->usernameEdit->text(),2)&&ui->usernameEdit->text()!=m_username)
    {
        QMessageBox::warning(this,"Warning","该用户名已被使用");
        ui->usernameEdit->setText(m_username);
        warningTimes++;
    }
    if(ui->passwordEdit->text().count()<6)
    {
        QMessageBox::warning(this,"Warning","密码位数不足六位");
        ui->passwordEdit->setText(m_password);
        warningTimes++;
    }
    if(warningTimes==0)
    {
        if(m_username!=ui->usernameEdit->text())
        {
            m_username=ui->usernameEdit->text();
            m_platform->m_admin[m_id]->setUsername(m_username);
        }
        if(m_phonenumber!=ui->phonenumberEdit->text())
        {
            m_phonenumber=ui->phonenumberEdit->text();
            m_platform->m_admin[m_id]->setPhoneNumber(m_phonenumber);
        }
        if(m_password!=ui->passwordEdit->text())
        {
            m_password=ui->passwordEdit->text();
            m_platform->m_admin[m_id]->setPassword(m_password);
        }
        p.id=m_id;
        p.tbname="admin";
        p.operType=1;
        m_platform->addOperatingWord(p);
        QString content=QString("用户 %1").arg(m_username);
        ui->welcomeMsg1->setText(content);
        ui->editInfoButton->setEnabled(true);
        ui->usernameEdit->setReadOnly(true);
        ui->passwordEdit->setReadOnly(true);
        ui->phonenumberEdit->setReadOnly(true);
        ui->saveInfoButton->setEnabled(false);
        QMessageBox::information(this,"Information","更改成功");
    }
}

void AdminWidget::buttonRespond(int row, int column)
{
    int hotelid = ui->hotelTableWidget->item(row,0)->text().toInt();
    if(m_platform->m_hotel[hotelid]->getIsVerify())
    {
        ui->passButton->setEnabled(false);
        ui->reverifyButton->setEnabled(true);
    }
    else
    {
        ui->passButton->setEnabled(true);
        ui->reverifyButton->setEnabled(false);
    }
}

void AdminWidget::on_passButton_clicked()
{
    EditFlag p;
    int index = ui->hotelTableWidget->currentRow();
    int hotelid = ui->hotelTableWidget->item(index,0)->text().toInt();
    m_platform->m_hotel[hotelid]->setIsverify(true);
    p.id=hotelid;
    p.tbname="hotel";
    p.operType=1;
    m_platform->addOperatingWord(p);
    QMessageBox::information(this,"Information","操作成功");
    this->setupHotelPage();
}

void AdminWidget::on_reverifyButton_clicked()
{
    EditFlag p;
    int index = ui->hotelTableWidget->currentRow();
    int hotelid = ui->hotelTableWidget->item(index,0)->text().toInt();
    m_platform->m_hotel[hotelid]->setIsverify(false);
    p.id=hotelid;
    p.tbname="hotel";
    p.operType=1;
    m_platform->addOperatingWord(p);
    QMessageBox::information(this,"Information","操作成功");
    this->setupHotelPage();
}

void AdminWidget::on_personInfoButton_clicked()
{
    QString content=QString("用户 %1").arg(m_username);
    ui->welcomeMsg1->setText(content);
    ui->welcomeMsg2->setText("当前页面:个人信息");
    ui->stackedWidget->setCurrentIndex(1);
}

void AdminWidget::on_pushButton_clicked()
{
    QString content=QString("用户 %1").arg(m_username);
    ui->welcomeMsg1->setText(content);
    ui->welcomeMsg2->setText("当前页面:管理酒店");
    ui->stackedWidget->setCurrentIndex(0);
}

void AdminWidget::on_checkUserButton_clicked()
{
    QString content=QString("用户 %1").arg(m_username);
    ui->welcomeMsg1->setText(content);
    ui->welcomeMsg2->setText("当前页面:查看用户");
    ui->stackedWidget->setCurrentIndex(2);
}
