#include "hoteldetails.h"
#include "ui_hoteldetails.h"
#include "platform.h"
#include <QPixmap>
#include <QMessageBox>

HotelDetails::HotelDetails(Platform *p,QWidget *parent) :
    QWidget(parent),
    m_platform(p),
    ui(new Ui::HotelDetails)
{
    ui->setupUi(this);
    m_roomType="";
    m_roomNum=0;
    m_filename="";

    //使用自定义标题栏
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar=new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle("酒店预订服务与管理系统");
    setWindowIcon(QIcon(":/app1.png"));
    resize(430,509);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

HotelDetails::~HotelDetails()
{
    delete ui;
}

void HotelDetails::setup()
{
    ui->cityLine->setReadOnly(true);
    ui->cityLine->setText(m_city);
    ui->descriptionLine->setReadOnly(true);
    ui->descriptionLine->setPlainText(m_description);
    ui->hotelNameLine->setReadOnly(true);
    ui->hotelNameLine->setPlainText(m_hotelName);
    ui->roomNumLine->setReadOnly(true);
    ui->roomTypeLine->setReadOnly(true);
    m_filename=m_platform->m_hotel[m_hotelID]->getFilename();
    QPixmap pixmap(m_filename);
    ui->imageOfHotel->setPixmap(pixmap);
    ui->imageOfHotel->setScaledContents(true);
    ui->saveButton->setEnabled(false);

    QMap<int,Room *>::iterator it;
    for(it=m_platform->m_room.begin();it!=m_platform->m_room.end();it++)
    {
        if(it.value()->getHotelID()==m_hotelID)
        {
            if(m_roomType=="")
                m_roomType=m_roomType+it.value()->getCate();
            else
                m_roomType=m_roomType+","+it.value()->getCate();
            m_roomNum +=it.value()->getNumber();
        }
    }
    ui->roomNumLine->setText(QString::number(m_roomNum));
    ui->roomTypeLine->setText(m_roomType);
}

void HotelDetails::on_editButton_pressed()
{
    ui->editButton->setEnabled(false);
    ui->saveButton->setEnabled(true);
    ui->descriptionLine->setReadOnly(false);
    ui->hotelNameLine->setReadOnly(false);
}

void HotelDetails::on_saveButton_pressed()
{
    EditFlag p;
    int warningtimes = 0;
    if(ui->hotelNameLine->toPlainText().count()==0)
    {
        QMessageBox::warning(this,"Warning","酒店名称不能为空!");
        ui->hotelNameLine->setPlainText(m_hotelName);
        warningtimes++;
    }
    if(ui->descriptionLine->toPlainText().count()==0)
    {
        QMessageBox::warning(this,"Warning","酒店描述不能为空!");
        ui->descriptionLine->setPlainText(m_description);
        warningtimes++;
    }
    if(m_platform->foundSearch(ui->hotelNameLine->toPlainText(),m_city)&&ui->hotelNameLine->toPlainText()!=m_hotelName)
    {
        QMessageBox::warning(this,"Warning","在该地区的酒店名已存在!");
        ui->hotelNameLine->setPlainText(m_hotelName);
        warningtimes++;
    }
    else
    {
        m_hotelName=ui->hotelNameLine->toPlainText();
        m_description=ui->descriptionLine->toPlainText();
        m_platform->m_hotel[m_hotelID]->setHotelName(m_hotelName);
        m_platform->m_hotel[m_hotelID]->setDescription(m_description);
        p.id=m_hotelID;
        p.operType=1;
        p.tbname="hotel";
        m_platform->addOperatingWord(p);
        QMessageBox::information(this,"Information","保存成功");
        ui->saveButton->setEnabled(false);
        ui->editButton->setEnabled(true);
        ui->descriptionLine->setReadOnly(true);
        ui->hotelNameLine->setReadOnly(true);
    }
}
