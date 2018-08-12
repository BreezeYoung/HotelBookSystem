#include "manageroomwidget.h"
#include "ui_manageroomwidget.h"
#include "platform.h"
#include "title_bar.h"
#include "hotel.h"
#include "room.h"
#include<QHBoxLayout>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QMap>
#include<QHeaderView>

ManageRoomWidget::ManageRoomWidget(Platform *p, QWidget *parent) :
    QWidget(parent),
    m_platform(p),
    ui(new Ui::ManageRoomWidget)
{
    ui->setupUi(this);
    details_ui=NULL;

    //使用自定义标题栏
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar=new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle("酒店预订服务与管理系统");
    setWindowIcon(QIcon(":/app1.png"));
    resize(948,616);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

ManageRoomWidget::~ManageRoomWidget()
{
    delete ui;
    if(details_ui!=NULL)
        delete details_ui;
}

void ManageRoomWidget::setup()
{
    QMap<int,Hotel *>::iterator it;
    for(it=m_platform->m_hotel.begin();it!=m_platform->m_hotel.end();it++)
    {
        if(it.value()->getID()==m_hotelID)
        {
            ui->cityLine->setText(it.value()->getCity());
            m_hotelName=it.value()->getHotelName();
            ui->hotelNameLine->setText(m_hotelName);
            break;
        }
    }
    setupTablePage();

    //初始化表格的一些属性
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(6,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(7,QHeaderView::ResizeToContents);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
}

void ManageRoomWidget::setupTablePage()
{
    ui->tableWidget->clearContents();
    int row = ui->tableWidget->rowCount();
    for(int i=row;i>0;i--)
    {
        ui->tableWidget->removeRow(i-1);
    }
    row = 0;
    QMap<int,Room *>::iterator it;
    for(it=m_platform->m_room.begin();it!=m_platform->m_room.end();it++)
    {
        if(it.value()->getHotelID()==m_hotelID)
        {
            m_roomID=it.value()->getID();
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(m_roomID)));
            ui->tableWidget->setItem(row,1,new QTableWidgetItem(it.value()->getCate()));
            ui->tableWidget->setItem(row,2,new QTableWidgetItem(m_hotelName));
            ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::number(it.value()->getPrice())));
            ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString::number(it.value()->getNumber())));
            ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString::number(it.value()->getReserve())));
            ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString::number(it.value()->getRate())));
            if(it.value()->getIsDiscount())
            {
                ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString("是")));
            }
            else
            {
                ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString("否")));
            }
            ui->tableWidget->setItem(row,8,new QTableWidgetItem(it.value()->getDescription()));
            row++;
        }
    }
}

void ManageRoomWidget::on_updateButton_clicked()
{
    this->setupTablePage();
}

void ManageRoomWidget::on_deleteRoomButton_clicked()
{
    EditFlag p;
    int index=ui->tableWidget->currentRow();
    int roomid;
    QMap<int,Order *>::iterator it;
    QMap<int,Room *>::iterator it1;
    bool flag = false;
    if(index!=-1)
    {
        int r=QMessageBox::information(this,"Information","是否确认删除该房型?",QObject::tr("确认"),QObject::tr("取消"));
        if(r==QObject::tr("确认").toInt())
        {
            roomid=ui->tableWidget->item(index,0)->text().toInt();
            for(it=m_platform->m_myorder.begin();it!=m_platform->m_myorder.end();it++)
            {
                if(it.value()->getRoomID()==roomid)
                {
                    flag = true;
                    break;
                }
            }
            if(!flag)
            {
                for(it1=m_platform->m_room.begin();it1!=m_platform->m_room.end();it1++)
                {
                    if(it1.key()==roomid)
                        break;
                }
                m_platform->m_room.erase(it1);
                //delete m_platform->m_room[roomid];
                p.id=roomid;
                p.operType=2;
                p.tbname="room";
                m_platform->addOperatingWord(p);
                QMessageBox::information(this,"Information","删除成功!");
                this->setupTablePage();
            }
            else
            {
                QMessageBox::warning(this,"Warning","该房型有订单关联，无法删除");
            }
        }
    }
}

void ManageRoomWidget::on_createRoomButton_clicked()
{
    if(details_ui!=NULL)
    {
        delete details_ui;
        details_ui=NULL;
    }
    details_ui=new RoomDetails(m_platform);
    details_ui->setHotelID(m_hotelID);
    details_ui->setupForCreate();
}

void ManageRoomWidget::on_manageInfoButton_clicked()
{
    int index=ui->tableWidget->currentRow();
    if(index!=-1)
    {
        if(details_ui!=NULL)
        {
            delete details_ui;
            details_ui=NULL;
        }
        details_ui=new RoomDetails(m_platform);
        details_ui->setHotelID(m_hotelID);
        details_ui->setRoomID(ui->tableWidget->item(index,0)->text().toInt());
        details_ui->setupForManageInfo();
    }
}
