#include "getdetails.h"
#include "ui_getdetails.h"
#include "platform.h"
#include "commonhelper.h"
#include<QFile>
#include<QFileDialog>
#include<QPixmap>

getDetails::getDetails(Platform *p, QWidget *parent) :
    QWidget(parent),
    m_platform(p),
    ui(new Ui::getDetails)
{
    ui->setupUi(this);
    m_roomFilename="";
    m_hotelFilename="";
    CommonHelper::setTitleBar(this);
    resize(813,521);
}

getDetails::~getDetails()
{
    delete ui;
}

void getDetails::setup()
{
    int founderid=m_platform->m_hotel[m_hotelid]->getFounderID();
    ui->cityLine->setText(m_platform->m_hotel[m_hotelid]->getCity());
    ui->cityLine->setReadOnly(true);
    ui->hotelDesLine->setPlainText(m_platform->m_hotel[m_hotelid]->getDescription());
    ui->hotelDesLine->setReadOnly(true);
    ui->hotelNameLine->setText(m_platform->m_hotel[m_hotelid]->getHotelName());
    ui->roomDesLine->setPlainText(m_platform->m_room[m_roomid]->getDescription());
    ui->roomDesLine->setReadOnly(true);
    ui->roomTypeLine->setText(m_platform->m_room[m_roomid]->getCate());
    ui->roomTypeLine->setReadOnly(true);
    ui->roomNumLine->setText(QString::number(m_platform->m_room[m_roomid]->getNumber()));
    ui->roomNumLine->setReadOnly(true);
    ui->reserveLine->setText(QString::number(m_platform->m_room[m_roomid]->getReserve()));
    ui->reserveLine->setReadOnly(true);
    ui->rateLine->setText(QString::number(m_platform->m_room[m_roomid]->getRate(),'f',1));
    ui->rateLine->setReadOnly(true);
    if(m_platform->m_room[m_roomid]->getIsDiscount())
    {
        ui->isdiscountLine->setText(QString("是"));
    }
    else
    {
        ui->isdiscountLine->setText(QString("否"));
    }
    ui->isdiscountLine->setReadOnly(true);
    ui->founderLine->setText(m_platform->m_hotelrepre[founderid]->getUsername());
    ui->founderLine->setReadOnly(true);
    ui->phonenumberLine->setText(m_platform->m_hotelrepre[founderid]->getPhoneNumber());
    ui->phonenumberLine->setReadOnly(true);
    m_roomFilename=m_platform->m_room[m_roomid]->getFilename();
    m_hotelFilename=m_platform->m_hotel[m_hotelid]->getFilename();
    QPixmap pixmap1(m_hotelFilename);
    ui->imageOfHotel->setPixmap(pixmap1);
    ui->imageOfHotel->setScaledContents(true);
    QPixmap pixmap2(m_roomFilename);
    ui->imageOfRoom->setPixmap(pixmap2);
    ui->imageOfRoom->setScaledContents(true);
}
