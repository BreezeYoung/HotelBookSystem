#include "roomdetails.h"
#include "ui_roomdetails.h"
#include "title_bar.h"
#include "room.h"
#include "platform.h"
#include<QHBoxLayout>
#include<QRegExp>
#include<QRegExpValidator>
#include<QMessageBox>
#include<QFile>
#include<QFileDialog>
#include<QPixmap>

RoomDetails::RoomDetails(Platform *p,QWidget *parent) :
    QWidget(parent),
    m_platform(p),
    ui(new Ui::RoomDetails)
{
    ui->setupUi(this);
    isDiscount=false;
    m_filename="";

    //使用自定义标题栏
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar=new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle("酒店预订服务与管理系统");
    setWindowIcon(QIcon(":/app1.png"));
    resize(897,468);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

RoomDetails::~RoomDetails()
{
    delete ui;
}

void RoomDetails::setupForCreate()
{
    ui->stackedWidget->setCurrentWidget(ui->createPage);
    ui->rateEdit->setEnabled(false);
    QRegExp editExp("^[A-Za-z\u4e00-\u9fa5]*$");
    ui->roomCateEdit->setValidator(new QRegExpValidator(editExp));
    ui->roomDescripEdit->setPlaceholderText("请输入一百字以内的描述");
    ui->roomCateEdit->setMaxLength(10);
    QRegExp numberExp("[0-9]{5}");
    ui->roomNumEdit->setValidator(new QRegExpValidator(numberExp));
    ui->roomPriceEdit->setValidator(new QRegExpValidator(numberExp));
    ui->roomNumEdit->setMaxLength(5);        //房型数量最大99999
    this->show();
}

void RoomDetails::setupForManageInfo()
{
    ui->stackedWidget->setCurrentWidget(ui->detailsPage);
    QMap<int,Room *>::iterator it;
    for(it=m_platform->m_room.begin();it!=m_platform->m_room.end();it++)
    {
        if(it.key()==m_roomID)
        {
            break;
        }
    }
    m_cate=it.value()->getCate();
    m_descri=it.value()->getDescription();
    isDiscount=it.value()->getIsDiscount();
    m_price=it.value()->getPrice();
    m_rate=it.value()->getRate();
    m_number=it.value()->getNumber();
    m_filename=it.value()->getFilename();
    m_hotelName=m_platform->m_hotel[m_hotelID]->getHotelName();
    ui->roomCateEdit_2->setText(m_cate);
    ui->roomCateEdit_2->setReadOnly(true);
    ui->roomDescripEdit_2->setPlainText(m_descri);
    ui->roomDescripEdit_2->setReadOnly(true);
    ui->roomNumEdit_2->setText(QString::number(m_number));
    ui->roomNumEdit_2->setReadOnly(true);
    ui->roomPriceEdit_2->setText(QString::number(m_price));
    ui->roomPriceEdit_2->setReadOnly(true);
    ui->hotelNameLine_2->setText(m_hotelName);
    ui->hotelNameLine_2->setReadOnly(true);
    ui->rateEdit_2->setText(QString::number(m_rate));
    ui->rateEdit_2->setReadOnly(true);
    if(isDiscount)
        ui->comboBox_2->setCurrentIndex(1);
    else
        ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_2->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->ButtonUpdatePhoto_2->setEnabled(false);
    ui->showPhoto_2->setPixmap(QPixmap(m_filename));
    ui->showPhoto_2->setScaledContents(true);
    this->show();
}

void RoomDetails::on_comboBox_currentIndexChanged(int index)
{
    if(index==1)
    {
        isDiscount=true;
        ui->rateEdit->setEnabled(true);
    }
    else if(index==0)
    {
        isDiscount=false;
        ui->rateEdit->clear();
        ui->rateEdit->setEnabled(false);
    }
}

void RoomDetails::on_pushButton_clicked()
{
    EditFlag p;
    int id;
    m_number=ui->roomNumEdit->text().toInt();
    m_cate=ui->roomCateEdit->text();
    m_descri=ui->roomDescripEdit->toPlainText();
    m_price=ui->roomPriceEdit->text().toDouble();
    if(isDiscount)
        m_rate=ui->rateEdit->text().toDouble();
    else
        m_rate=0;
    if(!m_descri.count())
    {
        QMessageBox::warning(this,"Warning","客房描述不能为空");
    }
    if(!m_cate.count())
    {
        QMessageBox::warning(this,"Warning","客房类型不能为空");
    }
    else if(m_platform->roomFoundSearch(m_hotelID,m_cate))
    {
        QMessageBox::warning(this,"Warning","该酒店的该类型房间已创建过");
        ui->roomCateEdit->clear();
    }
    else if(ui->roomDescripEdit->toPlainText().count()>100)
    {
        QMessageBox::warning(this,"Warning","客房描述超过一百字");
    }
    else if(m_filename.isEmpty())
    {
        QMessageBox::warning(this,"Warning","请上传房间图片");
    }
    else
    {
        id=m_platform->getMaxID(4);
        m_platform->m_room[id]=new Room(id,m_hotelID,m_number,0,m_price,m_rate,isDiscount,m_cate,m_descri,m_filename);
        p.id=id;
        p.tbname="room";
        p.operType=3;
        m_platform->addOperatingWord(p);
        QMessageBox::information(this,"Information","房间创建成功");
        this->close();
    }
}

void RoomDetails::on_editButton_clicked()
{
    ui->editButton->setEnabled(false);
    ui->saveButton->setEnabled(true);
    ui->roomCateEdit_2->setReadOnly(false);
    ui->roomDescripEdit_2->setReadOnly(false);
    ui->roomNumEdit_2->setReadOnly(false);
    ui->roomPriceEdit_2->setReadOnly(false);
    ui->comboBox_2->setEnabled(true);
    ui->rateEdit_2->setReadOnly(false);
    ui->ButtonUpdatePhoto_2->setEnabled(true);
}



void RoomDetails::on_saveButton_clicked()
{
    EditFlag p;
    if(!ui->roomCateEdit_2->text().count())
    {
        QMessageBox::warning(this,"Warning","客房类型不能为空!");
        ui->roomCateEdit_2->setText(m_cate);
    }
    if(!ui->roomDescripEdit_2->toPlainText().count())
    {
        QMessageBox::warning(this,"Warning","客房描述不能为空!");
        ui->roomDescripEdit_2->setPlainText(m_descri);
    }
    if(m_platform->roomFoundSearch(m_hotelID,ui->roomCateEdit_2->text())&&m_cate!=ui->roomCateEdit_2->text())
    {
        QMessageBox::warning(this,"Warning","该房型在酒店已存在!");
        ui->roomCateEdit_2->setText(m_cate);
    }
    else
    {
        if(ui->roomCateEdit_2->text()!=m_cate)
        {
            m_platform->m_room[m_roomID]->setCate(ui->roomCateEdit_2->text());
        }
        if(ui->roomNumEdit_2->text().toInt()!=m_number)
        {
            m_number=ui->roomNumEdit_2->text().toInt();
            m_platform->m_room[m_roomID]->setNumber(m_number);
        }
        if(ui->roomPriceEdit_2->text().toDouble()!=m_price)
        {
            m_price=ui->roomPriceEdit_2->text().toDouble();
            m_platform->m_room[m_roomID]->setPrice(m_price);
        }
        if(ui->roomDescripEdit_2->toPlainText()!=m_descri)
        {
            m_descri=ui->roomDescripEdit_2->toPlainText();
            m_platform->m_room[m_roomID]->setDescription(m_descri);
        }
        if(ui->comboBox_2->currentIndex())
        {
            isDiscount=true;
            m_platform->m_room[m_roomID]->setIsDiscount(true);
        }
        else
        {
            isDiscount=false;
            m_platform->m_room[m_roomID]->setIsDiscount(false);
        }
        if(ui->rateEdit_2->text().toDouble()!=m_rate)
        {
            m_rate=ui->rateEdit_2->text().toDouble();
            m_platform->m_room[m_roomID]->setRate(m_rate);
        }
        m_platform->m_room[m_roomID]->setFilename(m_filename);
        p.id=m_roomID;
        p.operType=1;
        p.tbname="room";
        m_platform->addOperatingWord(p);
        ui->saveButton->setEnabled(false);
        ui->editButton->setEnabled(true);
        QMessageBox::information(this,"Information","保存成功!");
        ui->roomCateEdit_2->setReadOnly(true);
        ui->roomDescripEdit_2->setReadOnly(true);
        ui->roomNumEdit_2->setReadOnly(true);
        ui->roomPriceEdit_2->setReadOnly(true);
        ui->comboBox_2->setEnabled(false);
        ui->rateEdit_2->setReadOnly(true);
        ui->ButtonUpdatePhoto_2->setEnabled(false);
    }
}

void RoomDetails::on_comboBox_2_currentIndexChanged(int index)
{
    if(index==1)
    {
        isDiscount=true;
        ui->rateEdit_2->setEnabled(true);
    }
    else if(index==0)
    {
        isDiscount=false;
        ui->rateEdit_2->setEnabled(false);
    }
}

void RoomDetails::on_ButtonUpdatePhoto_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select image"), "./", tr("Image Files(*.png *.jpg *.bmg )"));
    if(!filename.isEmpty())
    {
        ui->showPhoto->setPixmap(QPixmap(filename));
        ui->showPhoto->setScaledContents(true);
        m_filename = filename;
    }
    else
    {
        QMessageBox::warning(this,"Warning","请选择一张图片");
    }
}

void RoomDetails::on_ButtonUpdatePhoto_2_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select image"), "./", tr("Image Files(*.png *.jpg *.bmg )"));
    if(!filename.isEmpty())
    {
        ui->showPhoto->setPixmap(QPixmap(filename));
        ui->showPhoto->setScaledContents(true);
        m_filename = filename;
    }
}
