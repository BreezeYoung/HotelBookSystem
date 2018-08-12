#ifndef PLATFORM_H
#define PLATFORM_H
#include<QMap>
#include<QVector>
#include<QTextCodec>
#include<QSqlQuery>
#include<QString>
#include<QMessageBox>
#include<QSqlDatabase>
#include<QSqlError>
#include<QVariant>
#include<QTableWidget>
#include<QDate>
#include "user.h"
#include "order.h"
#include "hotel.h"
#include "room.h"
#include "message.h"
#include "comment.h"
#include "database.h"

typedef int AT;   //AT表示AlterType，即改变类型，1表示修改，2表示删除，3表示增加
typedef struct
{
    int id;
    QString tbname;
    AT operType;
}EditFlag;

class Platform
{
public:
    Platform(){}

    //将以下漏到内存的变量作为公有成员，便于修改
    QMap<int,Customer*> m_customer;   //type0
    QMap<int,MyUser*> m_admin;        //type1
    QMap<int,MyUser*> m_hotelrepre;   //type2
    QMap<int,Order *> m_myorder;      //type3
    QMap<int,Room *> m_room;          //type4
    QMap<int,Hotel *> m_hotel;        //type5
    QMap<int,Comment *> m_comments;   //type6
    QMap<int,Message *> m_message;    //type7

    //公有函数
    void ReadDatabase();
    void WriteIntoDatabase();
    void addOperatingWord(EditFlag flag){
        operatingWord<<flag;
    }
    bool checkValid(QString phonenumber,QString password,int type);     //用于检查登录是否有效
    int getMaxID(int type);
    bool registerSearch(QString value,int searchItem);
    bool foundSearch(QString value,QString city);
    void clear();
    bool roomFoundSearch(int hotelid,QString roomcate);
    bool belongToHotels(int roomid,int founderid);       //判断订单是否属于酒店管理员旗下的酒店
    bool refund(QString username,double cost);           //退款函数
    double getCommentGrade(int hotelid);                 //得到酒店的平均好评，若无人评价则起始值为零
    double getCost(QDate checkinday,QDate checkoutday,int roomnum,bool isdiscout,double rate,double price);       //计算订单费用


private:
    QVector<EditFlag> operatingWord;       //使用QVector容器类存储需要对数据库进行增删改的语句
    int m_id;
    int m_hotelID;
    int m_roomID;
    int m_customerID;
    int m_roomNum;
    stateType m_state;
    bool m_isPaid;
    double m_cost;
    QString m_checkInDay;
    QString m_roomCat;
    QString m_comment;
    QString m_username;
    QString m_phonenumber;
    QString m_password;
    QString m_filename;
    int m_ordernum;
    double m_balance;
};

#endif // PLATFORM_H
