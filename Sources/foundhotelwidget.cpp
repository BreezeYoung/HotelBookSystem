#include "foundhotelwidget.h"
#include "ui_foundhotelwidget.h"
#include<QRegExp>
#include<QRegExpValidator>
#include<QMessageBox>
#include"hotelmanagerwidget.h"
#include"platform.h"
#include"hotel.h"
#include"user.h"
#include<QFile>
#include<QFileDialog>
#include<QPixmap>

foundHotelWidget::foundHotelWidget(Platform *p,HotelManagerWidget *parent) :
    QWidget(parent),
    m_platform(p),
    ui(new Ui::foundHotelWidget)
{
    ui->setupUi(this);
    m_filename="";

    //使用自定义标题栏
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar=new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle("酒店预订服务与管理系统");
    setWindowIcon(QIcon(":/app1.png"));
    resize(770,491);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

foundHotelWidget::~foundHotelWidget()
{
    delete ui;
}

void foundHotelWidget::setup()
{
    QRegExp editExp("^[A-Za-z\u4e00-\u9fa5]*$");
    ui->hotelNameEdit->setValidator(new QRegExpValidator(editExp));
    ui->cityEdit->setValidator(new QRegExpValidator(editExp));
    ui->hotelNameEdit->setMaxLength(20);
    ui->cityEdit->setMaxLength(10);
    ui->descriptionEdit->setPlaceholderText("请输入一百字以内的描述");
}

void foundHotelWidget::on_createButton_clicked()
{
    EditFlag p;
    int id;
    m_hotelName=ui->hotelNameEdit->text();
    m_city=ui->cityEdit->text();
    m_description=ui->descriptionEdit->toPlainText();
    if(!m_city.count())
    {
        QMessageBox::warning(this,"Warning","所在城市不能为空!");
    }
    if(!m_hotelName.count())
    {
        QMessageBox::warning(this,"Warning","酒店名不能为空!");
    }
    else if(m_platform->foundSearch(m_hotelName,m_city))
    {
        QMessageBox::warning(this,"Warning","该酒店已存在!");
        ui->hotelNameEdit->clear();
        ui->cityEdit->clear();
    }
    else if(m_description.count()>100||m_description.count()==0)
    {
        if(m_description.count()>100)
            QMessageBox::warning(this,"Warning","描述字数超过100!");
        if(m_description.count()==0)
            QMessageBox::warning(this,"Warning","描述不能为空!");
    }
    else if(m_filename.isEmpty())
    {
        QMessageBox::warning(this,"Warning","请上传房间图片");
    }
    else
    {
        id=m_platform->getMaxID(5);
        m_platform->m_hotel[id]=new Hotel(id,m_founderID,m_hotelName,m_city,m_description,false,m_filename);
        ui->descriptionEdit->clear();
        ui->hotelNameEdit->clear();
        ui->cityEdit->clear();
        p.id=id;
        p.tbname="hotel";
        p.operType=3;
        m_platform->addOperatingWord(p);
        QMessageBox::information(this,"Information","酒店创建成功");
        this->close();
    }
}

void foundHotelWidget::on_ButtonUpdatePhoto_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select image"), "./", tr("Image Files(*.png *.jpg *.bmg )"));
    if(!filename.isEmpty())
    {
        ui->showPhoto->setPixmap(QPixmap(filename));
        ui->showPhoto->setScaledContents(true);
        m_filename=filename;
    }
    else
    {
        QMessageBox::warning(this,"Warning","请选择一张图片");
    }
}
