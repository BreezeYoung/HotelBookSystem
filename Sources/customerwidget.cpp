#include "customerwidget.h"
#include "ui_customerwidget.h"
#include "platform.h"
#include "user.h"
#include "mainwindow.h"
#include "messagedetails.h"
#include <QString>
#include <QDate>
#include <QDateTime>
#include <QMessageBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QTableWidget>

CustomerWidget::CustomerWidget(Platform *platform,MainWindow *parent) :
    QWidget(parent),
    m_platform(platform),
    ui(new Ui::CustomerWidget)
{
    ui->setupUi(this);
    msgDetails_ui=NULL;
    details_ui=NULL;
    orderBy=0;

    connect(this->ui->exitButton,SIGNAL(clicked(bool)),parent,SLOT(LogOut()));
    connect(this->ui->orderTableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(buttonRespond(int,int)));
    connect(this->ui->msgTableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(replyOrCheck(int,int)));
    connect(this->ui->checkOutDayEdit,SIGNAL(dateChanged(QDate)),this,SLOT(checkOutValid(QDate)));
    connect(this->ui->roomTableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(roomNumBoxChangeValue(int,int)));
    connect(this->ui->roomTableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(clickToGetDetails(int,int)));
    connect(this->ui->roomTableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(clickToDialog(int,int)));
    //自动搜索(笔者程序的一大亮点!)
    connect(this->ui->hotelNameInput,SIGNAL(textEdited(QString)),this,SLOT(autoSearchForHname(QString)));
    connect(this->ui->cityNameInput,SIGNAL(textEdited(QString)),this,SLOT(autoSearchForCity(QString)));
    connect(this->ui->roomNameInput,SIGNAL(textEdited(QString)),this,SLOT(autoSearchForRcate(QString)));
    //排序
    connect(this->ui->roomTableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(orderByClement(int)));
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

CustomerWidget::~CustomerWidget()
{
    delete ui;
    if(msgDetails_ui!=NULL)
        delete msgDetails_ui;
}

void CustomerWidget::setup()
{
    ui->copyRight->setStyleSheet("QLabel{color:rgb(0, 155, 188);}");
    QMap<int, Customer *>::iterator it;
    for(it=m_platform->m_customer.begin();it!=m_platform->m_customer.end();it++)
    {
        if(it.value()->getPhoneNumber()==m_phonenumber)
        {
            break;
        }
    }
    m_id=it.value()->getID();
    m_username=it.value()->getUsername();
    m_password=it.value()->getPassword();
    m_ordernum=it.value()->getOrderNum();
    m_balance=it.value()->getBalance();
    ui->stackedWidget->setCurrentWidget(ui->homePage);
    ui->welcomeMsg1->setText("欢迎！");
    QString content=QString("用户 %1").arg(m_username);
    ui->welcomeMsg2->setText(content);
    ui->balanceEdit->isReadOnly();

    //我的主页页面初始化
    ui->homePageInfo->setTabText(0,"我的消息");
    ui->homePageInfo->setTabText(1,"我的订单");
    ui->payButton->setEnabled(false);
    ui->refundButton->setEnabled(false);
    ui->cancelButton->setEnabled(false);
    ui->finishButton->setEnabled(false);
    ui->commentButton->setEnabled(false);
    this->setupMsgPage();
    this->setupOrderPage();
    ui->orderTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->orderTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->orderTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->msgTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->msgTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->msgTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->msgTableWidget->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    ui->msgTableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->msgTableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);

    //个人信息页面初始化
    ui->usernameEdit->setText(m_username);
    ui->phonenumberEdit->setText(m_phonenumber);
    ui->passwordEdit->setText(m_password);
    ui->balanceEdit->setText(QString::number(m_balance,'f',2));
    ui->usernameEdit->setReadOnly(true);
    ui->phonenumberEdit->setReadOnly(true);
    ui->passwordEdit->setReadOnly(true);
    ui->balanceEdit->setReadOnly(true);
    ui->rechargeEdit->setText(QString::number(0,'f',2));
    ui->rechargeEdit->setReadOnly(true);
    ui->saveInfoButton->setEnabled(false);
    ui->rechargeEdit->setMaxLength(5);
    QRegExp rechargeExp("[0-9]{5}");
    ui->rechargeEdit->setValidator(new QRegExpValidator(rechargeExp));
    QRegExp phonenumberExp("1[0-9]{10}");
    ui->phonenumberEdit->setValidator(new QRegExpValidator(phonenumberExp));
    QRegExp usernameExp("[A-Za-z0-9]{20}");
    ui->usernameEdit->setValidator(new QRegExpValidator(usernameExp));
    QRegExp passwordExp("[A-Za-z0-9]{16}");
    ui->passwordEdit->setValidator(new QRegExpValidator(passwordExp));

    //预订服务页面初始化
    ui->roomTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->roomTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->roomTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->roomTableWidget->horizontalHeader()->setSectionsClickable(true);
    ui->roomTableWidget->horizontalHeader()->setSortIndicator(4,Qt::AscendingOrder);
    ui->roomTableWidget->horizontalHeader()->setSortIndicator(10,Qt::AscendingOrder);
    ui->roomTableWidget->horizontalHeader()->setSortIndicatorShown(true);
    ui->checkInDayEdit->setDate(QDate::currentDate());
    ui->checkOutDayEdit->setDate(QDate::currentDate());
    ui->roomNumBox->setWrapping(true);
    this->setupRoomPage();
}

void CustomerWidget::setupMsgPage()
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

void CustomerWidget::setupOrderPage()
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
    QStringList text;
    text<<"一星好评"<<"二星好评"<<"三星好评"<<"四星好评"<<"五星好评";
    for(it=m_platform->m_myorder.begin();it!=m_platform->m_myorder.end();it++)
    {
        roomid=it.value()->getRoomID();
        if(it.value()->getCustomerID()==m_id)
        {
            ui->orderTableWidget->insertRow(rows);
            ui->orderTableWidget->setItem(rows,0,new QTableWidgetItem(QString::number(it.key())));
            ui->orderTableWidget->setItem(rows,1,new QTableWidgetItem(m_username));
            ui->orderTableWidget->setItem(rows,2,new QTableWidgetItem(m_platform->m_room[roomid]->getCate()));
            ui->orderTableWidget->setItem(rows,3,new QTableWidgetItem(QString::number(it.value()->getRoomNum())));
            ui->orderTableWidget->setItem(rows,4,new QTableWidgetItem(it.value()->getCheckInDay()));
            ui->orderTableWidget->setItem(rows,5,new QTableWidgetItem(it.value()->getCheckOutDay()));
            ui->orderTableWidget->setItem(rows,6,new QTableWidgetItem(QString::number(it.value()->getCost(),'f',2)));
            ui->orderTableWidget->setItem(rows,7,new QTableWidgetItem(it.value()->toStringBystate()));
            QComboBox *forComment=new QComboBox(this);
            forComment->addItems(text);
            forComment->setCurrentIndex(0);
            ui->orderTableWidget->setCellWidget(rows,8,(QWidget *)forComment);
            rows++;
        }
    }
}

void CustomerWidget::setupRoomPage()
{
    ui->roomTableWidget->clearContents();
    int rows=ui->roomTableWidget->rowCount();
    for(int i=rows;i>0;i--)
    {
        ui->roomTableWidget->removeRow(i-1);
    }
    rows = 0;
    int hotelid;
    int founderid;
    QMap<int,Room *>::iterator it;
    for(it=m_platform->m_room.begin();it!=m_platform->m_room.end();it++)
    {
        hotelid=it.value()->getHotelID();
        founderid=m_platform->m_hotel[hotelid]->getFounderID();
        if(m_platform->m_hotel[hotelid]->getIsVerify())
        {
            ui->roomTableWidget->insertRow(rows);
            ui->roomTableWidget->setItem(rows,0,new QTableWidgetItem(QString::number(it.value()->getID())));
            ui->roomTableWidget->setItem(rows,1,new QTableWidgetItem(it.value()->getCate()));
            ui->roomTableWidget->setItem(rows,2,new QTableWidgetItem(m_platform->m_hotel[hotelid]->getHotelName()));
            ui->roomTableWidget->setItem(rows,3,new QTableWidgetItem(m_platform->m_hotel[hotelid]->getCity()));
            ui->roomTableWidget->setItem(rows,4,new QTableWidgetItem(QString::number(it.value()->getPrice(),'f',2)));
            ui->roomTableWidget->setItem(rows,5,new QTableWidgetItem(QString::number(it.value()->getNumber())));
            ui->roomTableWidget->setItem(rows,6,new QTableWidgetItem(QString::number(it.value()->getReserve())));
            ui->roomTableWidget->setItem(rows,7,new QTableWidgetItem(QString::number(it.value()->getRate())));
            if(it.value()->getIsDiscount())
            {
                ui->roomTableWidget->setItem(rows,8,new QTableWidgetItem(QString("是")));
            }
            else
            {
                ui->roomTableWidget->setItem(rows,8,new QTableWidgetItem(QString("否")));
            }
            ui->roomTableWidget->setItem(rows,9,new QTableWidgetItem(it.value()->getDescription()));
            ui->roomTableWidget->setItem(rows,10,new QTableWidgetItem(QString::number(m_platform->getCommentGrade(hotelid),'f',1)));
            ui->roomTableWidget->setItem(rows,11,new QTableWidgetItem(m_platform->m_hotelrepre[founderid]->getPhoneNumber()));
            ui->roomTableWidget->setItem(rows,12,new QTableWidgetItem(m_platform->m_hotelrepre[founderid]->getUsername()));
            ui->roomTableWidget->setItem(rows,13,new QTableWidgetItem(QString("查看详情")));
            rows++;
        }
    }
}

void CustomerWidget::autoSearchForCity(const QString &text)
{
    int rows=ui->roomTableWidget->rowCount();
    QString currentCity="";
    for(int i=0;i<rows;i++)
    {
        currentCity=ui->roomTableWidget->item(i,3)->text();
        if(!currentCity.contains(text)&&!ui->roomTableWidget->isRowHidden(i))
            ui->roomTableWidget->hideRow(i);
        if(currentCity.contains(text)&&ui->roomTableWidget->isRowHidden(i))
            ui->roomTableWidget->showRow(i);
    }
}

void CustomerWidget::autoSearchForHname(const QString &text)
{
    int rows=ui->roomTableWidget->rowCount();
    QString currentHname="";
    for(int i=0;i<rows;i++)
    {
        currentHname=ui->roomTableWidget->item(i,2)->text();
        if(!currentHname.contains(text)&&!ui->roomTableWidget->isRowHidden(i))
            ui->roomTableWidget->hideRow(i);
        if(currentHname.contains(text)&&ui->roomTableWidget->isRowHidden(i))
            ui->roomTableWidget->showRow(i);
    }
}

void CustomerWidget::autoSearchForRcate(const QString &text)
{
    int rows=ui->roomTableWidget->rowCount();
    QString currentRcate="";
    for(int i=0;i<rows;i++)
    {
        currentRcate=ui->roomTableWidget->item(i,1)->text();
        if(!currentRcate.contains(text)&&!ui->roomTableWidget->isRowHidden(i))
            ui->roomTableWidget->hideRow(i);
        if(currentRcate.contains(text)&&ui->roomTableWidget->isRowHidden(i))
            ui->roomTableWidget->showRow(i);
    }
}

void CustomerWidget::on_personInfoButton_clicked()
{
    QString content=QString("用户 %1").arg(m_username);
    ui->welcomeMsg1->setText(content);
    ui->welcomeMsg2->setText("当前页面:个人信息");
    ui->stackedWidget->setCurrentWidget(ui->personInfo);
}

void CustomerWidget::on_homePageButton_clicked()
{
    QString content=QString("用户 %1").arg(m_username);
    ui->welcomeMsg1->setText(content);
    ui->welcomeMsg2->setText("当前页面:我的主页");
    ui->stackedWidget->setCurrentWidget(ui->homePage);
}

void CustomerWidget::on_editInfoButton_pressed()
{
    ui->saveInfoButton->setEnabled(true);
    ui->usernameEdit->setReadOnly(false);
    ui->passwordEdit->setReadOnly(false);
    ui->phonenumberEdit->setReadOnly(false);
    ui->editInfoButton->setEnabled(false);
    ui->rechargeEdit->setReadOnly(false);
}

void CustomerWidget::on_saveInfoButton_pressed()
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
            m_platform->m_customer[m_id]->setUsername(m_username);
        }
        if(m_phonenumber!=ui->phonenumberEdit->text())
        {
            m_phonenumber=ui->phonenumberEdit->text();
            m_platform->m_customer[m_id]->setPhoneNumber(m_phonenumber);
        }
        if(m_password!=ui->passwordEdit->text())
        {
            m_password=ui->passwordEdit->text();
            m_platform->m_customer[m_id]->setPassword(m_password);
        }
        m_balance=m_balance+ui->rechargeEdit->text().toDouble();
        m_platform->m_customer[m_id]->setBalance(m_balance);
        ui->rechargeEdit->setText(QString::number(0,'f',2));
        ui->balanceEdit->setText(QString::number(m_balance,'f',2));
        p.id=m_id;
        p.tbname="customer";
        p.operType=1;
        m_platform->addOperatingWord(p);
        QString content=QString("用户 %1").arg(m_username);
        ui->welcomeMsg1->setText(content);
        ui->editInfoButton->setEnabled(true);
        ui->usernameEdit->setReadOnly(true);
        ui->passwordEdit->setReadOnly(true);
        ui->phonenumberEdit->setReadOnly(true);
        ui->rechargeEdit->setReadOnly(true);
        ui->saveInfoButton->setEnabled(false);
        QMessageBox::information(this,"Information","更改成功");
    }
}


void CustomerWidget::on_gotoBookButton_clicked()
{
    QString content=QString("用户 %1").arg(m_username);
    ui->welcomeMsg1->setText(content);
    ui->welcomeMsg2->setText("当前页面:预订服务");
    ui->stackedWidget->setCurrentWidget(ui->bookServe);
}

void CustomerWidget::buttonRespond(int row, int column)
{
    int orderid=ui->orderTableWidget->item(row,0)->text().toInt();
    stateType state=m_platform->m_myorder[orderid]->getState();
    switch (state) {
    case 0:          //未支付时可以进行支付或者取消订单
        ui->payButton->setEnabled(true);
        ui->refundButton->setEnabled(false);
        ui->cancelButton->setEnabled(true);
        ui->finishButton->setEnabled(false);
        ui->commentButton->setEnabled(false);
        break;
    case 1:          //支付后不允许取消订单除非先退款,也可以点击完成订单
        ui->payButton->setEnabled(false);
        ui->refundButton->setEnabled(true);
        ui->cancelButton->setEnabled(false);
        ui->finishButton->setEnabled(true);
        ui->commentButton->setEnabled(false);
        break;
    case 2:          //申请退款期间不允许做任何操作
        ui->payButton->setEnabled(false);
        ui->refundButton->setEnabled(false);
        ui->cancelButton->setEnabled(false);
        ui->finishButton->setEnabled(false);
        ui->commentButton->setEnabled(false);
        break;
    case 3:          //退款完成后可取消订单，但无法进行支付,订单也无法完成
        ui->payButton->setEnabled(false);
        ui->refundButton->setEnabled(false);
        ui->cancelButton->setEnabled(true);
        ui->finishButton->setEnabled(false);
        ui->commentButton->setEnabled(false);
        break;
    case 4:          //完成订单后可进行评价
        ui->payButton->setEnabled(false);
        ui->refundButton->setEnabled(false);
        ui->cancelButton->setEnabled(false);
        ui->finishButton->setEnabled(false);
        ui->commentButton->setEnabled(true);
        break;
    default:         //取消订单后无法对订单进行操作
        ui->payButton->setEnabled(false);
        ui->refundButton->setEnabled(false);
        ui->cancelButton->setEnabled(false);
        ui->finishButton->setEnabled(false);
        ui->commentButton->setEnabled(false);
        break;
    }
}

void CustomerWidget::on_updateButton_clicked()
{
    int index=ui->stackedWidget->currentIndex();
    switch (index) {
    case 0:
        this->setupOrderPage();
        this->setupMsgPage();
        break;
    case 1:
        break;
    default:
        break;
    }
}

void CustomerWidget::replyOrCheck(int row, int column)
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

void CustomerWidget::checkOutValid(const QDate &date)
{
    QDate checkinday=ui->checkInDayEdit->date();
    if(date<checkinday)
        ui->checkOutDayEdit->setDate(checkinday);
}

void CustomerWidget::on_bookButton_clicked()
{
    EditFlag p;
    EditFlag p1;
    int index=ui->roomTableWidget->currentRow();
    int id;
    int roomid;
    int roomnum;
    double cost;
    if(index!=-1)
    {
        int r=QMessageBox::information(this,"Information","是否确认预订?",QObject::tr("确认"),QObject::tr("取消"));
        if(r==QObject::tr("确认").toInt())
        {
            id=m_platform->getMaxID(3);
            roomid=ui->roomTableWidget->item(index,0)->text().toInt();
            roomnum=ui->roomNumBox->text().toInt();
            cost = m_platform->getCost(ui->checkInDayEdit->date(),ui->checkOutDayEdit->date(),ui->roomNumBox->text().toInt(),m_platform->m_room[roomid]->getIsDiscount(),m_platform->m_room[roomid]->getRate(),m_platform->m_room[roomid]->getPrice());
            m_platform->m_myorder[id] = new Order(id,roomid,m_id,roomnum,ui->checkInDayEdit->date().toString("yyyyMMdd"),ui->checkOutDayEdit->date().toString("yyyyMMdd"),cost,0);
            p.id=id;
            p.operType=3;
            p.tbname="myorder";
            m_platform->addOperatingWord(p);
            m_platform->m_room[roomid]->addReserve(roomnum);
            p1.id=roomid;
            p1.tbname="room";
            p1.operType=1;
            m_platform->addOperatingWord(p1);
            QMessageBox::information(this,"Information","创建订单成功");
            ui->checkInDayEdit->setDate(QDate::currentDate());
            ui->checkOutDayEdit->setDate(QDate::currentDate());
            ui->roomNumBox->setValue(0);
            this->setupRoomPage();
        }
    }
}

void CustomerWidget::roomNumBoxChangeValue(int row, int column)
{
    int maxnum=ui->roomTableWidget->item(row,5)->text().toInt()-ui->roomTableWidget->item(row,6)->text().toInt();
    ui->roomNumBox->setRange(0,maxnum);
}

void CustomerWidget::clickToDialog(int row, int column)            //由双击打开的与该酒店经理对话的对话框
{
    int roomid=ui->roomTableWidget->item(row,0)->text().toInt();
    int hotelid=m_platform->m_room[roomid]->getHotelID();
    int hotelrepreid=m_platform->m_hotel[hotelid]->getFounderID();
    QString managerName=m_platform->m_hotelrepre[hotelrepreid]->getUsername();
    if(msgDetails_ui!=NULL)
    {
        delete msgDetails_ui;
        msgDetails_ui=NULL;
    }
    msgDetails_ui = new MessageDetails(m_platform);
    msgDetails_ui->setSenderName(m_username);
    msgDetails_ui->setReceiverName(managerName);
    msgDetails_ui->setupForDialog();
    msgDetails_ui->show();
}

void CustomerWidget::on_payButton_clicked()
{
    EditFlag p;
    EditFlag p1;
    int index=ui->orderTableWidget->currentRow();
    int orderid=ui->orderTableWidget->item(index,0)->text().toInt();
    double cost=ui->orderTableWidget->item(index,6)->text().toDouble();
    int r=QMessageBox::information(this,"Information","是否确认支付?",QObject::tr("确认"),QObject::tr("取消"));
    if(r==QObject::tr("确认").toInt())
    {
        if(cost>m_balance)
        {
            QMessageBox::warning(this,"Warning","余额不足");
        }
        else
        {
            m_balance=m_balance-cost;
            ui->balanceEdit->setText(QString::number(m_balance,'f',2));
            m_platform->m_customer[m_id]->setBalance(m_balance);
            p.id=m_id;
            p.tbname="customer";
            p.operType=1;
            m_platform->addOperatingWord(p);
            m_platform->m_myorder[orderid]->setState(1);
            p1.id=orderid;
            p1.tbname="myorder";
            p1.operType=1;
            m_platform->addOperatingWord(p1);
            QMessageBox::information(this,"Information","支付成功");
            this->setupOrderPage();
            ui->payButton->setEnabled(false);
            ui->cancelButton->setEnabled(false);
        }
    }
}

void CustomerWidget::on_refundButton_clicked()
{
    EditFlag p;
    int index=ui->orderTableWidget->currentRow();
    int orderid=ui->orderTableWidget->item(index,0)->text().toInt();
    int r=QMessageBox::information(this,"Information","是否确认申请退款?",QObject::tr("确认"),QObject::tr("取消"));
    if(r==QObject::tr("确认").toInt())
    {
        m_platform->m_myorder[orderid]->setState(2);
        p.id=orderid;
        p.tbname="myorder";
        p.operType=1;
        m_platform->addOperatingWord(p);
        QMessageBox::information(this,"Information","申请成功");
        this->setupOrderPage();
        ui->refundButton->setEnabled(false);
        ui->finishButton->setEnabled(false);
    }
}

void CustomerWidget::on_finishButton_clicked()
{
    EditFlag p;
    int index=ui->orderTableWidget->currentRow();
    int orderid=ui->orderTableWidget->item(index,0)->text().toInt();
    int roomnum=m_platform->m_myorder[orderid]->getRoomNum();
    int roomid=m_platform->m_myorder[orderid]->getRoomID();
    int r=QMessageBox::information(this,"Information","是否确认完成订单?",QObject::tr("确认"),QObject::tr("取消"));
    if(r==QObject::tr("确认").toInt())
    {
        m_platform->m_myorder[orderid]->setState(4);
        p.id=orderid;
        p.tbname="myorder";
        p.operType=1;
        m_platform->addOperatingWord(p);
        m_platform->m_room[roomid]->declineReserve(roomnum);
        p.id=roomid;
        p.tbname="room";
        p.operType=1;
        m_platform->addOperatingWord(p);
        QMessageBox::information(this,"Information","操作成功");
        this->setupOrderPage();
        ui->finishButton->setEnabled(false);
        ui->refundButton->setEnabled(false);
    }
}

void CustomerWidget::on_cancelButton_clicked()
{
    EditFlag p;
    int index=ui->orderTableWidget->currentRow();
    int orderid=ui->orderTableWidget->item(index,0)->text().toInt();
    int roomnum=m_platform->m_myorder[orderid]->getRoomNum();
    int roomid=m_platform->m_myorder[orderid]->getRoomID();
    int r=QMessageBox::information(this,"Information","是否确认取消订单?",QObject::tr("确认"),QObject::tr("取消"));
    if(r==QObject::tr("确认").toInt())
    {
        m_platform->m_myorder[orderid]->setState(5);
        p.id=orderid;
        p.tbname="myorder";
        p.operType=1;
        m_platform->addOperatingWord(p);
        m_platform->m_room[roomid]->declineReserve(roomnum);
        p.id=roomid;
        p.tbname="room";
        p.operType=1;
        m_platform->addOperatingWord(p);
        QMessageBox::information(this,"Information","操作成功");
        this->setupOrderPage();
        ui->payButton->setEnabled(false);
        ui->cancelButton->setEnabled(false);
    }
}

void CustomerWidget::on_commentButton_clicked()
{
    EditFlag p;
    QMap<int,Comment *>::iterator it;
    bool flag;           //用于寻找之前是否已经评论过
    int id;
    int index=ui->orderTableWidget->currentRow();
    int orderid=ui->orderTableWidget->item(index,0)->text().toInt();
    int hotelid=m_platform->m_room[m_platform->m_myorder[orderid]->getRoomID()]->getHotelID();
    QComboBox *stars=(QComboBox *)ui->orderTableWidget->cellWidget(index,8);
    int r=QMessageBox::information(this,"Information","是否确认完成评价?",QObject::tr("确认"),QObject::tr("取消"));
    if(r==QObject::tr("确认").toInt())
    {
        for(it=m_platform->m_comments.begin();it!=m_platform->m_comments.end();it++)
        {
            if(it.value()->getOrderID()==orderid)
            {
                flag=true;
                break;
            }
        }
        if(!flag)
        {
            id=m_platform->getMaxID(6);
            m_platform->m_comments[id] = new Comment(id,orderid,hotelid,stars->currentIndex()+1);
            p.id=id;
            p.tbname="comment";
            p.operType=3;
            m_platform->addOperatingWord(p);
            QMessageBox::information(this,"Information","评价成功");
        }
        else
        {
            QMessageBox::warning(this,"Warning","您已做出过评价");
        }
    }
}

void CustomerWidget::orderByClement(int logicalIndex)
{
    if(logicalIndex>=0&&logicalIndex<=10)
    {
        switch (orderBy) {
        case 0:
        case 2:
            ui->roomTableWidget->sortItems(logicalIndex,Qt::AscendingOrder);
            orderBy = 1;
            break;
        case 1:
            ui->roomTableWidget->sortItems(logicalIndex,Qt::DescendingOrder);
            orderBy = 2;
            break;
        default:
            break;
        }
    }
}

void CustomerWidget::clickToGetDetails(int row, int column)
{
    int roomid=ui->roomTableWidget->item(row,0)->text().toInt();
    int hotelid=m_platform->m_room[roomid]->getHotelID();
    if(column==13&&ui->roomTableWidget->item(row,column)->text()=="查看详情")
    {
        if(details_ui!=NULL)
        {
            delete details_ui;
            details_ui=NULL;
        }
        details_ui = new getDetails(m_platform);
        details_ui->setHotelID(hotelid);
        details_ui->setRoomID(roomid);
        details_ui->setup();
        details_ui->show();
    }
}
