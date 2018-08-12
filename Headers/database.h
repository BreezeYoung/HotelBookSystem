#ifndef DATABASE_H
#define DATABASE_H
#include<QTextCodec>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QSqlError>
#include<QMessageBox>
#include<QString>
#include<QVariant>

//不同表格的表格参数
static const QString customer_value="(id int primary key, username varchar(20), phonenumber varchar(11), password varchar(16), ordernum int, balance real)";
static const QString admin_value="(id int primary key, username varchar(20), phonenumber varchar(11), password varchar(16))";
static const QString hotelrepre_value="(id int primary key, username varchar(20), phonenumber varchar(11), password varchar(16))";
static const QString myorder_value="(id int primary key, roomid int, customerid int, roomnum int, checkinday varchar(10), checkoutday varchar(10), cost real, state int)";
static const QString room_value="(id int primary key, hotelid int, number int, reserve int, price real, rate real, isdiscount bit, cate varchar(20), description varchar(100), filename varchar(100))";
static const QString hotel_value="(id int primary key, founderid int, hotelname varchar(20), city varchar(10), description varchar(100), isverify bit, filename varchar(100))";
static const QString comment_value="(id int primary key, orderid int, hotelid int, stars int)";
static const QString message_value="(id int primary key, sender varchar(20), receiver varchar(20), senddate varchar(20), content varchar(100), replyid int)";

class Platform;

class DataBase
{
public:
    DataBase(QString dbname);
    DataBase(int type);
    DataBase(){}
    ~DataBase(){}

    //设置成员属性的接口
    void setTableName(QString tbname);
    void setParameter(QString parameter)
    {
        _parameter=parameter;
    }
    void insertForHotel(int id,QString a1,QString a2,QString a3,QString a4,int a5);
    void insertForMessage(int id, QString a1, QString a2, QString a3, QString a4);
    void insert(int id,QString a1,QString a2,QString a3,double a4,int a5,int a6,int a7,double a8);     //插入数据
    void update(int id,Platform *p);
    QStringList select(QString columnname);                //获得columnname的所有结果集
    bool searchOfString(QString columnname,QString search_value);
    bool searchOfInt(QString columnname,int search_value);
    void clear();                                          //清空数据表
    void deleteData(QString columnname,QString value);
    void deleteData(QString columnname, int value);

    int count();
private:
    QSqlDatabase openDataBase(QString name);
    QString _dbname;          //数据库名称
    QString _tbname;          //表格名称
    QString _parameter;       //参数名
    QString _primaryKey;      //主键
    QSqlDatabase _db;
    int m_type;               //m_type作为标识，0表示顾客，1表示平台管理员，2表示酒店代表，3表示订单，4表示客房，5表示酒店，6表示评价,7表示消息
};

#endif // DATABASE_H
