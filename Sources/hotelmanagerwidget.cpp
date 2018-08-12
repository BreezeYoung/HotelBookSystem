#include "hotelmanagerwidget.h"
#include "ui_hotelmanagerwidget.h"
#include "user.h"
#include "platform.h"
#include "hotel.h"
#include "mainwindow.h"
#include "hoteldetails.h"
#include "commonhelper.h"
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QObject>

HotelManagerWidget::HotelManagerWidget(Platform *platform,MainWindow *parent) :
    QWidget(parent),
    m_platform(platform),
    ui(new Ui::HotelManagerWidget)
{
    ui->setupUi(this);
    found_ui=NULL;
    details_ui=NULL;
    manageRoom_ui=NULL;
    msgDetails_ui = NULL;
    connect(ui->exitButton,SIGNAL(clicked(bool)),parent,SLOT(LogOut()));
    connect(ui->msgTableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(replyOrCheck(int,int)));
    connect(ui->orderTableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(buttonRespond(int,int)));
    //使用自定义标题栏
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar=new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle("酒店预订服务与管理系统");
    setWindowIcon(QIcon(":/app1.png"));
    resize(1052,705);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

HotelManagerWidget::~HotelManagerWidget()
{
    delete ui;
    if(details_ui!=NULL)
        delete details_ui;
    if(found_ui!=NULL)
        delete found_ui;
}

void HotelManagerWidget::setup()
{
    ui->copyRight->setStyleSheet("QLabel{color:rgb(0, 155, 188);}");
    QMap<int, MyUser *>::iterator it;
    for(it=m_platform->m_hotelrepre.begin();it!=m_platform->m_hotelrepre.end();it++)
    {
        if(it.value()->getPhoneNumber()==m_phonenumber)
        {
            break;
        }
    }
    m_id=it.value()->getID();
    m_username=it.value()->getUsername();
    m_password=it.value()->getPassword();
    ui->welcomeMsg1->setText("欢迎!");
    QString content=QString("用户 %1").arg(m_username);
    ui->welcomeMsg2->setText(content);
    ui->stackedWidget->setCurrentWidget(ui->hotelManagePage);

    //初始化表格的一些属性
    ui->hotelTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->hotelTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->hotelTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->orderTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->orderTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->orderTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->msgTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->msgTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->msgTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->hotelTableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->hotelTableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
    ui->hotelTableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->orderTableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->orderTableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->orderTableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->orderTableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    ui->orderTableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
    ui->orderTableWidget->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    ui->orderTableWidget->horizontalHeader()->setSectionResizeMode(6,QHeaderView::ResizeToContents);
    ui->orderTableWidget->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);
    ui->msgTableWidget->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    ui->msgTableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->msgTableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);

    //个人信息页面初始化
    ui->usernameEdit->setText(m_username);
    ui->phonenumberEdit->setText(m_phonenumber);
    ui->passwordEdit->setText(m_password);
    ui->usernameEdit->setReadOnly(true);
    ui->phonenumberEdit->setReadOnly(true);
    ui->passwordEdit->setReadOnly(true);
    ui->saveInfoButton->setEnabled(false);
    QRegExp phonenumberExp("1[0-9]{10}");
    ui->phonenumberEdit->setValidator(new QRegExpValidator(phonenumberExp));
    QRegExp usernameExp("[A-Za-z0-9]{20}");
    ui->usernameEdit->setValidator(new QRegExpValidator(usernameExp));
    QRegExp passwordExp("[A-Za-z0-9]{16}");
    ui->passwordEdit->setValidator(new QRegExpValidator(passwordExp));

    //管理酒店页面初始化
    this->setupHotelPage();

    //管理订单页面初始化
    this->setupOrderPage();
    ui->urgeButton->setEnabled(false);
    ui->refundButton->setEnabled(false);

    //我的信息页面初始化
    this->setupMsgPage();

}

void HotelManagerWidget::setupMsgPage()
{
    ui->msgTableWidget->clearContents();
    int row=ui->msgTableWidget->rowCount();
    for(int i=row;i>0;i--)
    {
        ui->msgTableWidget->removeRow(i-1);
    }
    row = 0;
    QMap<int,Message *>::iterator it;
    for(it=m_platform->m_message.begin();it!=m_platform->m_message.end();it++)
    {
        if(it.value()->getReceiverName()==m_username)
        {
            ui->msgTableWidget->insertRow(row);
            ui->msgTableWidget->setItem(row,0,new QTableWidgetItem(QString::number(it.key())));
            ui->msgTableWidget->setItem(row,1,new QTableWidgetItem(it.value()->getSenderName()));
            ui->msgTableWidget->setItem(row,2,new QTableWidgetItem(m_username));
            ui->msgTableWidget->setItem(row,3,new QTableWidgetItem(it.value()->getSendDate()));
            ui->msgTableWidget->setItem(row,4,new QTableWidgetItem(it.value()->getContent()));
            if(it.value()->getReplyID()==-1)
                ui->msgTableWidget->setItem(row,5,new QTableWidgetItem(QString("回复")));
            else
                ui->msgTableWidget->setItem(row,5,new QTableWidgetItem(QString("查看")));
            row++;
        }
    }
    for(it=m_platform->m_message.begin();it!=m_platform->m_message.end();it++)
    {
        if(it.value()->getSenderName()==m_username)
        {
            ui->msgTableWidget->insertRow(row);
            ui->msgTableWidget->setItem(row,0,new QTableWidgetItem(QString::number(it.key())));
            ui->msgTableWidget->setItem(row,1,new QTableWidgetItem(m_username));
            ui->msgTableWidget->setItem(row,2,new QTableWidgetItem(it.value()->getReceiverName()));
            ui->msgTableWidget->setItem(row,3,new QTableWidgetItem(it.value()->getSendDate()));
            ui->msgTableWidget->setItem(row,4,new QTableWidgetItem(it.value()->getContent()));
            ui->msgTableWidget->setItem(row,5,new QTableWidgetItem(QString("查看")));
            row++;
        }
    }
}

void HotelManagerWidget::setupHotelPage()
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
        if(it.value()->getFounderID()==m_id)
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
}

void HotelManagerWidget::setupOrderPage()
{
    ui->orderTableWidget->clearContents();
    int rows=ui->orderTableWidget->rowCount();
    for(int i=rows;i>0;i--)
    {
        ui->orderTableWidget->removeRow(i-1);
    }
    rows = 0;
    QMap<int,Order *>::iterator it;
    int roomid = 0;
    for(it=m_platform->m_myorder.begin();it!=m_platform->m_myorder.end();it++)
    {
        roomid=it.value()->getRoomID();
        if(m_platform->belongToHotels(roomid,m_id))
        {
            ui->orderTableWidget->insertRow(rows);
            ui->orderTableWidget->setItem(rows,0,new QTableWidgetItem(QString::number(it.key())));
            ui->orderTableWidget->setItem(rows,1,new QTableWidgetItem(m_platform->m_customer[it.value()->getCustomerID()]->getUsername()));
            ui->orderTableWidget->setItem(rows,2,new QTableWidgetItem(m_platform->m_room[roomid]->getCate()));
            ui->orderTableWidget->setItem(rows,3,new QTableWidgetItem(QString::number(it.value()->getRoomNum())));
            ui->orderTableWidget->setItem(rows,4,new QTableWidgetItem(it.value()->getCheckInDay()));
            ui->orderTableWidget->setItem(rows,5,new QTableWidgetItem(it.value()->getCheckOutDay()));
            ui->orderTableWidget->setItem(rows,6,new QTableWidgetItem(QString::number(it.value()->getCost())));
            ui->orderTableWidget->setItem(rows,7,new QTableWidgetItem(it.value()->toStringBystate()));
            rows++;
        }
    }
}

void HotelManagerWidget::on_myMessageButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->messageInfo);
    ui->welcomeMsg2->setText("当前界面:我的消息");
    QString content=QString("用户 %1").arg(m_username);
    ui->welcomeMsg1->setText(content);
}

void HotelManagerWidget::on_hotelManageButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->hotelManagePage);
    ui->welcomeMsg2->setText("当前界面:管理酒店");
    QString content=QString("用户 %1").arg(m_username);
    ui->welcomeMsg1->setText(content);
}

void HotelManagerWidget::on_orderManageButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->orderManagePage);
    ui->welcomeMsg2->setText("当前界面:管理订单");
    QString content=QString("用户 %1").arg(m_username);
    ui->welcomeMsg1->setText(content);
}

void HotelManagerWidget::on_editInfoButton_pressed()
{
    ui->saveInfoButton->setEnabled(true);
    ui->usernameEdit->setReadOnly(false);
    ui->passwordEdit->setReadOnly(false);
    ui->phonenumberEdit->setReadOnly(false);
    ui->editInfoButton->setEnabled(false);
}

void HotelManagerWidget::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->personInfoPage);
    ui->welcomeMsg2->setText("当前界面:个人信息");
    QString content=QString("用户 %1").arg(m_username);
    ui->welcomeMsg1->setText(content);
}

void HotelManagerWidget::on_saveInfoButton_pressed()
{
    EditFlag p;
    int warningTimes = 0;         //用于检查是否修改成功
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
            m_platform->m_hotelrepre[m_id]->setUsername(m_username);
        }
        if(m_phonenumber!=ui->phonenumberEdit->text())
        {
            m_phonenumber=ui->phonenumberEdit->text();
            m_platform->m_hotelrepre[m_id]->setPhoneNumber(m_phonenumber);
        }
        if(m_password!=ui->passwordEdit->text())
        {
            m_password=ui->passwordEdit->text();
            m_platform->m_hotelrepre[m_id]->setPassword(m_password);
        }
        p.id=m_id;
        p.tbname="hotelrepre";
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

void HotelManagerWidget::on_newHotelButton_clicked()
{
    if(found_ui!=NULL)
    {
        delete found_ui;
        found_ui=NULL;
    }
    found_ui=new foundHotelWidget(m_platform);
    found_ui->setFounderID(m_id);
    found_ui->setup();
    found_ui->move(326,107);
    found_ui->show();
}

void HotelManagerWidget::on_getInfoButton_clicked()
{
    int index=ui->hotelTableWidget->currentRow();
    if(index!=-1)
    {
        if(details_ui!=NULL)
        {
            delete details_ui;
            details_ui=NULL;
        }
        details_ui=new HotelDetails(m_platform);
        details_ui->setHotelID(ui->hotelTableWidget->item(index,0)->text().toInt());
        details_ui->setHotelName(ui->hotelTableWidget->item(index,1)->text());
        details_ui->setCity(ui->hotelTableWidget->item(index,2)->text());
        details_ui->setDescription(ui->hotelTableWidget->item(index,3)->text());
        details_ui->setup();
        details_ui->show();
    }
}

void HotelManagerWidget::on_deleteHotelButton_clicked()
{
    EditFlag p;
    int index=ui->hotelTableWidget->currentRow();
    int hotelid;
    QMap<int,Room *>::iterator it;
    QMap<int,Hotel *>::iterator it1;
    bool flag = false;
    if(index!=-1)
    {
        int r=QMessageBox::information(this,"Information","是否确认删除该酒店?",QObject::tr("确认"),QObject::tr("取消"));
        if(r==QObject::tr("确认").toInt())
        {
            hotelid=ui->hotelTableWidget->item(index,0)->text().toInt();
            for(it=m_platform->m_room.begin();it!=m_platform->m_room.end();it++)
            {
                if(it.value()->getHotelID()==hotelid)
                {
                    flag = true;
                    break;
                }
            }
            if(!flag)
            {
                for(it1=m_platform->m_hotel.begin();it1!=m_platform->m_hotel.end();it1++)
                {
                    if(it1.key()==hotelid)
                        break;
                }
                m_platform->m_hotel.erase(it1);
                //delete m_platform->m_hotel[hotelid];
                p.id=hotelid;
                p.operType=2;
                p.tbname="hotel";
                m_platform->addOperatingWord(p);
                QMessageBox::information(this,"Information","删除成功");
                this->setupHotelPage();
            }
            else
            {
                QMessageBox::warning(this,"Warning","该酒店存在房型无法删除");
            }
        }
    }
}

void HotelManagerWidget::on_updateButton_clicked()
{
    int index=ui->stackedWidget->currentIndex();
    switch (index) {
    case 0:
        this->setupMsgPage();
        break;
    case 1:
        this->setupHotelPage();
        break;
    case 2:
        this->setupOrderPage();
        break;
    default:
        break;
    }
}

void HotelManagerWidget::on_manageRoomButton_clicked()
{
    int index=ui->hotelTableWidget->currentRow();
    if(index!=-1)
    {
        if(manageRoom_ui!=NULL)
        {
            delete manageRoom_ui;
            manageRoom_ui=NULL;
        }
        manageRoom_ui=new ManageRoomWidget(m_platform);
        manageRoom_ui->setHotelID(ui->hotelTableWidget->item(index,0)->text().toInt());
        manageRoom_ui->setup();
        manageRoom_ui->show();
    }
}

void HotelManagerWidget::replyOrCheck(int row, int column)
{
    int msgID = ui->msgTableWidget->item(row,0)->text().toInt();
    if(msgDetails_ui!=NULL)
    {
        delete msgDetails_ui;
        msgDetails_ui=NULL;
    }
    if(ui->msgTableWidget->item(row,column)->text()=="回复"&&column==5)
    {
        msgDetails_ui = new MessageDetails(m_platform);
        msgDetails_ui->setID(msgID);
        msgDetails_ui->setupForReply();
        msgDetails_ui->show();
    }
    else if(ui->msgTableWidget->item(row,column)->text()=="查看"&&column==5)
    {
        msgDetails_ui = new MessageDetails(m_platform);
        msgDetails_ui->setID(msgID);
        msgDetails_ui->setupForCheck();
        msgDetails_ui->show();
    }
}

void HotelManagerWidget::on_urgeButton_clicked()
{
    EditFlag p;
    int id;
    QString receiverName;
    QString content;
    int index=ui->orderTableWidget->currentRow();
    if(index!=-1)
    {
        receiverName=ui->orderTableWidget->item(index,1)->text();
        content=QString("请尽快支付订单号为%1的金额").arg(ui->orderTableWidget->item(index,0)->text().toInt());
        id=m_platform->getMaxID(7);
        m_platform->m_message[id] = new Message(id,m_username,receiverName,QDate::currentDate().toString("yyyyMMdd"),content,-1);
        p.id=id;
        p.tbname="message";
        p.operType=3;
        m_platform->addOperatingWord(p);
        QMessageBox::information(this,"Information","发送成功");
    }
}

void HotelManagerWidget::on_refundButton_clicked()
{
    EditFlag p;
    int orderid;
    int index=ui->orderTableWidget->currentRow();
    if(index!=-1)
    {
        int r=QMessageBox::information(this,"Information","是否确认退款?",QObject::tr("确认"),QObject::tr("取消"));
        if(r==QObject::tr("确认").toInt())
        {
            orderid=ui->orderTableWidget->item(index,0)->text().toInt();
            m_platform->m_myorder[orderid]->setState(3);
            p.id=orderid;
            p.tbname="myorder";
            p.operType=1;
            m_platform->addOperatingWord(p);
            if(m_platform->refund(ui->orderTableWidget->item(index,1)->text(),ui->orderTableWidget->item(index,6)->text().toDouble()))
            {
                QMessageBox::information(this,"Information","退款成功");
            }
            else
            {
                QMessageBox::information(this,"Information","退款失败");
            }
            this->setupOrderPage();
            ui->refundButton->setEnabled(false);
        }
    }
}

void HotelManagerWidget::buttonRespond(int row, int column)
{
    int orderid = ui->orderTableWidget->item(row,0)->text().toInt();
    stateType currentState = m_platform->m_myorder[orderid]->getState();
    switch (currentState) {
    case 0:        //订单未支付前可以催促付款
        ui->urgeButton->setEnabled(true);
        ui->refundButton->setEnabled(false);
        break;
    case 2:        //用户申请退款只能执行确定退款
        ui->urgeButton->setEnabled(false);
        ui->refundButton->setEnabled(true);
        break;
    default:       //其他情况无法做出任何操作
        ui->urgeButton->setEnabled(false);
        ui->refundButton->setEnabled(false);
        break;
    }
}
