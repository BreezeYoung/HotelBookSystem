#ifndef MESSAGE_H
#define MESSAGE_H
#include<QString>
class Message
{
public:
    Message(int id=0,QString sendername="",QString receivername="",QString senddate="",QString content="",int replyid=0)
        :m_id(id),m_senderName(sendername),m_receiverName(receivername),m_sendDate(senddate),m_content(content),m_replyid(replyid)
    {}
    ~Message(){}

    //获得成员属性的接口
    int getID() const
    {
        return m_id;
    }
    QString getSenderName() const
    {
        return m_senderName;
    }
    QString getReceiverName() const
    {
        return m_receiverName;
    }
    QString getSendDate() const
    {
        return m_sendDate;
    }
    QString getContent() const
    {
        return m_content;
    }
    int getReplyID() const
    {
        return m_replyid;
    }

    //修改成员属性的接口
    void setReplyID(int replyid)
    {
        m_replyid=replyid;
    }
private:
    int m_id;                      //消息编号
    QString m_senderName;          //发送者名称
    QString m_receiverName;        //接收者名称
    QString m_sendDate;            //发送日期
    QString m_content;             //发送内容
    int m_replyid;                 //回复消息的ID,-1表示该消息暂无回复
};

#endif // MESSAGE_H
