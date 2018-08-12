#ifndef MESSAGEDETAILS_H
#define MESSAGEDETAILS_H

#include <QWidget>
#include <QString>

class Platform;

namespace Ui {
class MessageDetails;
}

class MessageDetails : public QWidget
{
    Q_OBJECT

public:
    explicit MessageDetails(Platform *p,QWidget *parent = 0);
    ~MessageDetails();

    void setID(int id)
    {
        m_id=id;
    }
    void setSenderName(QString sendername)
    {
        m_senderName=sendername;
    }
    void setReceiverName(QString receivername)
    {
        m_receiverName=receivername;
    }
    void setupForReply();
    void setupForCheck();
    void setupForDialog();
private slots:
    void on_pushButton_clicked();

    void on_sendButton_clicked();

private:
    Ui::MessageDetails *ui;
    Platform *m_platform;

    int m_id;
    QString m_senderName;
    QString m_receiverName;
    QString m_content;
    QString m_replycontent;
};

#endif // MESSAGEDETAILS_H
