#include "messagedetails.h"
#include "ui_messagedetails.h"
#include "commonhelper.h"
#include "platform.h"
#include<QMessageBox>
#include<QDate>

MessageDetails::MessageDetails(Platform *p,QWidget *parent) :
    QWidget(parent),
    m_platform(p),
    ui(new Ui::MessageDetails)
{
    ui->setupUi(this);
    CommonHelper::setTitleBar(this);
    resize(400,561);
}

MessageDetails::~MessageDetails()
{
    delete ui;
}

void MessageDetails::setupForDialog()
{
    ui->senderNameLine->setText(m_senderName);
    ui->receiverNameLine->setText(m_receiverName);
    ui->senderNameLine->setReadOnly(true);
    ui->receiverNameLine->setReadOnly(true);
    ui->contentText->setPlaceholderText("请输入一百字以内的内容");
    ui->replyContentText->setReadOnly(true);
    ui->pushButton->setVisible(false);
}

void MessageDetails::setupForReply()
{
    m_senderName = m_platform->m_message[m_id]->getSenderName();
    m_receiverName = m_platform->m_message[m_id]->getReceiverName();
    m_content = m_platform->m_message[m_id]->getContent();
    ui->senderNameLine->setText(m_senderName);
    ui->receiverNameLine->setText(m_receiverName);
    ui->contentText->setPlainText(m_content);
    ui->replyContentText->setPlaceholderText("请输入一百字以内的内容");
    ui->receiverNameLine->setReadOnly(true);
    ui->senderNameLine->setReadOnly(true);
    ui->contentText->setReadOnly(true);
    ui->sendButton->setVisible(false);
}

void MessageDetails::setupForCheck()
{
    setupForReply();
    int replyid = m_platform->m_message[m_id]->getReplyID();
    if(replyid==-1)
    {
        m_replycontent = QString("当前暂无回复");
    }
    else
    {
        m_replycontent = m_platform->m_message[replyid]->getContent();
    }
    ui->replyContentText->setPlainText(m_replycontent);
    ui->replyContentText->setReadOnly(true);
    ui->pushButton->setVisible(false);
    ui->sendButton->setVisible(false);
}

void MessageDetails::on_pushButton_clicked()
{
    EditFlag p;
    EditFlag p1;
    int id;
    if(!ui->replyContentText->toPlainText().count())
    {
        QMessageBox::warning(this,"Warning","回复内容不能为空!");
    }
    else
    {
        id = m_platform->getMaxID(7);
        m_replycontent = ui->replyContentText->toPlainText();
        m_platform->m_message[m_id]->setReplyID(id);
        m_platform->m_message[id] = new Message(id,m_receiverName,m_senderName,QDate::currentDate().toString("yyyyMMdd"),m_replycontent,-1);
        p.id = id;
        p.tbname = "message";
        p.operType = 3;
        p1.id = m_id;
        p1.tbname = "message";
        p1.operType = 1;
        m_platform->addOperatingWord(p);
        m_platform->addOperatingWord(p1);
        QMessageBox::information(this,"Information","回复成功");
        this->close();
    }
}

void MessageDetails::on_sendButton_clicked()
{
    EditFlag p;
    int id;
    if(!ui->contentText->toPlainText().count())
    {
        QMessageBox::warning(this,"Warning","发送内容不能为空");
    }
    else
    {
        m_content = ui->contentText->toPlainText();
        id = m_platform->getMaxID(7);
        m_platform->m_message[id] = new Message(id,m_senderName,m_receiverName,QDate::currentDate().toString("yyyyMMdd"),m_content,-1);
        p.id=id;
        p.tbname="message";
        p.operType=3;
        m_platform->addOperatingWord(p);
        QMessageBox::information(this,"Information","发送成功");
        this->close();
    }
}
