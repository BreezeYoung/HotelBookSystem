#include "database.h"
#include "user.h"
#include "platform.h"
#include<QSqlQuery>
#include<QDebug>
#include<QSqlDatabase>
#include<QSqlError>
#include<QVector>

DataBase::DataBase(QString dbname)
    :_dbname(dbname)
{
    _tbname="";
    _parameter="";
    _primaryKey="id";
}

DataBase::DataBase(int type)           //用于生成与User相关联的数据库对象
{
    _dbname="HotelBookSystem";
    _tbname=Identity[type];
    _primaryKey="id";
    m_type=type;
    switch (type) {
    case 0:
        _parameter=customer_value;
        break;
    case 1:
        _parameter=admin_value;
        break;
    case 2:
        _parameter=hotelrepre_value;
        break;
    default:
        break;
    }
}

void DataBase::setTableName(QString tbname)
{
    _tbname=tbname;
    if(tbname=="customer")
    {
        m_type=0;
    }
    else if(tbname=="admin")
    {
        m_type=1;
    }
    else if(tbname=="hotelrepre")
    {
        m_type=2;
    }
    else if(tbname=="myorder")
    {
        m_type=3;
    }
    else if(tbname=="room")
    {
        m_type=4;
    }
    else if(tbname=="hotel")
    {
        m_type=5;
    }
    else if(tbname=="comment")
    {
        m_type=6;
    }
    else if(tbname=="message")
    {
        m_type=7;
    }
}

void DataBase::insertForMessage(int id,QString a1,QString a2,QString a3,QString a4)    //专门用于消息的插入数据库操作
{
    _db=openDataBase(_dbname);
    QSqlQuery query(_db);
    QString create=QString("create table %1 %2").arg(_tbname).arg(_parameter);
    query.exec(create);
    int replyid = -1;
    QString p1=QString("INSERT INTO %1 VALUES(?,?,?,?,?,?)").arg(_tbname);
    query.prepare(p1);
    query.bindValue(0,id);
    query.bindValue(1,a1);
    query.bindValue(2,a2);
    query.bindValue(3,a3);
    query.bindValue(4,a4);
    query.bindValue(5,replyid);           //-1表示该消息暂无回复
    query.exec();
}


void DataBase::insertForHotel(int id, QString a1, QString a2, QString a3, QString a4, int a5)
{
    _db=openDataBase(_dbname);
    QSqlQuery query(_db);
    QString create=QString("create table %1 %2").arg(_tbname).arg(_parameter);
    query.exec(create);
    QString p1=QString("INSERT INTO %1 VALUES(?,?,?,?,?,?,?)").arg(_tbname);
    query.prepare(p1);
    query.bindValue(0,id);
    query.bindValue(1,a5);
    query.bindValue(2,a1);
    query.bindValue(3,a2);
    query.bindValue(4,a3);
    query.bindValue(5,0);
    query.bindValue(6,a4);
    query.exec();
}

void DataBase::insert(int id,QString a1,QString a2,QString a3,double a4,int a5,int a6,int a7,double a8)
{
    _db=openDataBase(_dbname);
    int isdiscount = 0;
    QSqlQuery sqlQuery(_db);
    QString create=QString("create table %1 %2").arg(_tbname).arg(_parameter);
    sqlQuery.exec(create);
    QString p1=QString("INSERT INTO %1 VALUES(?,?,?,?,?,?)").arg(_tbname);
    QString p2=QString("INSERT INTO %1 VALUES(?,?,?,?)").arg(_tbname);
    QString p3=QString("INSERT INTO %1 VALUES(?,?,?,?,?,?,?,?,?,?)").arg(_tbname);
    QString p4=QString("INSERT INTO %1 VALUES(?,?,?,?,?,?,?,?)").arg(_tbname);
    QString p5=QString("INSERT INTO %1 VALUES(?,?,?,?,?)").arg(_tbname);
    switch (m_type) {
    case 0:
        sqlQuery.prepare(p1);
        sqlQuery.bindValue(0,id);
        sqlQuery.bindValue(1,a1);   //输入用户名昵称
        sqlQuery.bindValue(2,a2);   //输入手机号
        sqlQuery.bindValue(3,a3);   //输入密码
        sqlQuery.bindValue(4,a5);   //输入所持订单数
        sqlQuery.bindValue(5,a8);   //输入余额
        sqlQuery.exec();
        break;
    case 1:
        sqlQuery.prepare(p2);
        sqlQuery.bindValue(0,id);
        sqlQuery.bindValue(1,a1);   //输入用户名昵称
        sqlQuery.bindValue(2,a2);   //输入手机号
        sqlQuery.bindValue(3,a3);   //输入密码
        sqlQuery.exec();
        break;
    case 2:
        sqlQuery.prepare(p2);
        sqlQuery.bindValue(0,id);
        sqlQuery.bindValue(1,a1);   //输入用户名昵称
        sqlQuery.bindValue(2,a2);   //输入手机号
        sqlQuery.bindValue(3,a3);   //输入密码
        sqlQuery.exec();
        break;
    case 3:
        sqlQuery.prepare(p4);
        sqlQuery.bindValue(0,id);
        sqlQuery.bindValue(1,a5);   //输入客房编号
        sqlQuery.bindValue(2,a6);   //输入用户编号
        sqlQuery.bindValue(3,a7);   //输入订房数量
        sqlQuery.bindValue(4,a1);   //输入入住日期
        sqlQuery.bindValue(5,a2);   //输入退房日期
        sqlQuery.bindValue(6,a8);   //输入需付金额
        sqlQuery.bindValue(7,a4);   //输入订单状态，详情请查看下行注释
        sqlQuery.exec();            //0表示未支付，1表示已支付，2表示待退款，3表示已退款，4表示已完成（即退房），5表示取消订单
        break;
    case 4:
        sqlQuery.prepare(p3);
        sqlQuery.bindValue(0,id);
        sqlQuery.bindValue(1,a5);   //输入酒店编号
        sqlQuery.bindValue(2,a6);   //输入客房数量
        sqlQuery.bindValue(3,a7);   //输入已预定的数量
        sqlQuery.bindValue(4,a4);   //输入价格
        sqlQuery.bindValue(5,a8);   //输入折率
        sqlQuery.bindValue(6,isdiscount);   //输入是否优惠(插入时该值已自动作0处理)
        sqlQuery.bindValue(7,a1);   //输入客房类型
        sqlQuery.bindValue(8,a2);   //输入描述
        sqlQuery.bindValue(9,a3);   //输入图片地址
        sqlQuery.exec();
        break;
    case 6:
        sqlQuery.prepare(p2);
        sqlQuery.bindValue(0,id);
        sqlQuery.bindValue(1,a5);   //输入订单编号
        sqlQuery.bindValue(2,a6);   //输入酒店编号
        sqlQuery.bindValue(3,a7);   //输入评价星数
        sqlQuery.exec();
        break;
    default:
        break;
    }
}

void DataBase::update(int id,Platform *p)
{
    _db=openDataBase(_dbname);
    QSqlQuery sqlQuery(_db);
    QSqlQuery query1(_db);
    QString update_content;
    QString create=QString("create table %1 %2").arg(_tbname).arg(_parameter);
    sqlQuery.exec(create);
    //更新时同时进行与数据库原数据的比较和修改
    QString selectcontent=QString("select * from %1 where id = %2").arg(_tbname).arg(id);
    sqlQuery.exec(selectcontent);
    //指向数据库的结果集
    sqlQuery.next();
    switch (m_type) {
    case 0:
        if(sqlQuery.value(1).toString()!=p->m_customer[id]->getUsername())
        {
            update_content=QString("update customer set username = '%1' where username = '%2'").arg(p->m_customer[id]->getUsername()).arg(sqlQuery.value(1).toString());
            query1.exec(update_content);
        }
        if(sqlQuery.value(2).toString()!=p->m_customer[id]->getPhoneNumber())
        {
            update_content=QString("update customer set phonenumber = '%1' where phonenumber = '%2'").arg(p->m_customer[id]->getPhoneNumber()).arg(sqlQuery.value(2).toString());
            query1.exec(update_content);
        }
        if(sqlQuery.value(3).toString()!=p->m_customer[id]->getPassword())
        {
            update_content=QString("update customer set password ='%1' where id = %2").arg(p->m_customer[id]->getPassword()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        if(sqlQuery.value(4).toInt()!=p->m_customer[id]->getOrderNum())
        {
            update_content=QString("update customer set ordernum = %1 where id = %2").arg(p->m_customer[id]->getOrderNum()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        if(sqlQuery.value(5).toDouble()!=p->m_customer[id]->getBalance())
        {
            update_content=QString("update customer set balance = %1 where id = %2").arg(p->m_customer[id]->getBalance()).arg(sqlQuery.value(0).toDouble());
            query1.exec(update_content);
        }
        break;
    case 1:
        if(sqlQuery.value(1).toString()!=p->m_admin[id]->getUsername())
        {
            update_content=QString("update admin set username = '%1' where username = '%2'").arg(p->m_admin[id]->getUsername()).arg(sqlQuery.value(1).toString());
            query1.exec(update_content);
        }
        if(sqlQuery.value(2).toString()!=p->m_admin[id]->getPhoneNumber())
        {
            update_content=QString("update admin set phonenumber = '%1' where phonenumber = '%2'").arg(p->m_admin[id]->getPhoneNumber()).arg(sqlQuery.value(2).toString());
            query1.exec(update_content);
        }
        if(sqlQuery.value(3).toString()!=p->m_admin[id]->getPassword())
        {
            update_content=QString("update admin set password ='%1' where id = %2").arg(p->m_admin[id]->getPassword()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        break;
    case 2:
        if(sqlQuery.value(1).toString()!=p->m_hotelrepre[id]->getUsername())
        {
            update_content=QString("update hotelrepre set username = '%1' where username = '%2'").arg(p->m_hotelrepre[id]->getUsername()).arg(sqlQuery.value(1).toString());
            query1.exec(update_content);
        }
        if(sqlQuery.value(2).toString()!=p->m_hotelrepre[id]->getPhoneNumber())
        {
            update_content=QString("update hotelrepre set phonenumber = '%1' where phonenumber = '%2'").arg(p->m_hotelrepre[id]->getPhoneNumber()).arg(sqlQuery.value(2).toString());
            query1.exec(update_content);
        }
        if(sqlQuery.value(3).toString()!=p->m_hotelrepre[id]->getPassword())
        {
            update_content=QString("update hotelrepre set password ='%1' where id = %2").arg(p->m_hotelrepre[id]->getPassword()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        break;
    case 3:          //订单只允许修改state成员
        if(sqlQuery.value(7).toInt()!=p->m_myorder[id]->getState())
        {
            update_content=QString("update myorder set state = %1 where id = %2").arg(p->m_myorder[id]->getState()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        break;
    case 4:
        if(sqlQuery.value(2).toInt()!=p->m_room[id]->getNumber())
        {
            update_content=QString("update room set number = %1 where id = %2").arg(p->m_room[id]->getNumber()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        if(sqlQuery.value(3).toInt()!=p->m_room[id]->getReserve())
        {
            update_content=QString("update room set reserve = %1 where id = %2").arg(p->m_room[id]->getReserve()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        if(sqlQuery.value(4).toDouble()!=p->m_room[id]->getPrice())
        {
            update_content=QString("update room set price = %1 where id = %2").arg(p->m_room[id]->getPrice()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        if(sqlQuery.value(5).toDouble()!=p->m_room[id]->getRate())
        {
            update_content=QString("update room set rate = %1 where id = %2").arg(p->m_room[id]->getRate()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        if(sqlQuery.value(6).toBool()!=p->m_room[id]->getIsDiscount())
        {
            update_content=QString("update room set isdiscount = %1 where id = %2").arg(p->m_room[id]->getIsDiscountInt()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        if(sqlQuery.value(7).toString()!=p->m_room[id]->getCate())
        {
            update_content=QString("update room set cate = '%1' where id = %2").arg(p->m_room[id]->getCate()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        if(sqlQuery.value(8).toString()!=p->m_room[id]->getDescription())
        {
            update_content=QString("update room set description = '%1' where id = %2").arg(p->m_room[id]->getDescription()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        if(sqlQuery.value(9).toString()!=p->m_room[id]->getFilename())
        {
            update_content=QString("update room set filename = '%1' where id = %2").arg(p->m_room[id]->getFilename()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        break;
    case 5:
        if(sqlQuery.value(2).toString()!=p->m_hotel[id]->getHotelName())
        {
            update_content=QString("update hotel set hotelname = '%1' where id = %2").arg(p->m_hotel[id]->getHotelName()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        if(sqlQuery.value(4).toString()!=p->m_hotel[id]->getDescription())
        {
            update_content=QString("update hotel set description = '%1' where id = %2").arg(p->m_hotel[id]->getDescription()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        if(sqlQuery.value(5).toInt()!=p->m_hotel[id]->getIsVerifyInt())
        {
            update_content=QString("update hotel set isverify = %1 where id = %2").arg(p->m_hotel[id]->getIsVerifyInt()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        break;
    case 6:         //评论一旦做出不允许修改
        break;
    case 7:
        if(sqlQuery.value(5).toInt()!=p->m_message[id]->getReplyID())
        {
            update_content=QString("update message set replyid = %1 where id = %2").arg(p->m_message[id]->getReplyID()).arg(sqlQuery.value(0).toInt());
            query1.exec(update_content);
        }
        break;
    default:
        break;
    }
    _db.close();
}

QStringList DataBase::select(QString columnname)
{
    _db=openDataBase(_dbname);
    QSqlQuery sqlQuery(_db);
    QString create=QString("create table %1 %2").arg(_tbname).arg(_parameter);
    sqlQuery.exec(create);
    QStringList result;
    QString selectcontent=QString("select %1 from %2").arg(columnname).arg(_tbname);
    if(sqlQuery.exec(selectcontent))
    {
        while(sqlQuery.next())
        {
            result.append(sqlQuery.value(0).toString());
        }
    }
    _db.close();
    return result;
}

bool DataBase::searchOfString(QString columnname, QString search_value)
{
    int flag=0;
    _db=openDataBase(_dbname);
    QSqlQuery sqlQuery(_db);
    QString create=QString("create table %1 %2").arg(_tbname).arg(_parameter);
    sqlQuery.exec(create);
    QString selectcontent=QString("select %1 from %2").arg(columnname).arg(_tbname);
    if(sqlQuery.exec(selectcontent))
    {
        while(sqlQuery.next())
        {
            if(sqlQuery.value(0).toString()==search_value)
            {
                flag=1;
                break;
            }
        }
    }
    _db.close();
    if(flag!=0)
        return true;       //查找到结果返回true
    else
        return false;
}

bool DataBase::searchOfInt(QString columnname, int search_value)
{
    int flag=0;
    _db=openDataBase(_dbname);
    QSqlQuery sqlQuery(_db);
    QString create=QString("create table %1 %2").arg(_tbname).arg(_parameter);
    sqlQuery.exec(create);
    QString selectcontent=QString("select %1 from %2").arg(columnname).arg(_tbname);
    if(sqlQuery.exec(selectcontent))
    {
        while(sqlQuery.next())
        {
            if(sqlQuery.value(0).toInt()==search_value)
            {
                flag=1;
                break;
            }
        }
    }
    _db.close();
    if(flag!=0)
        return true;       //查找到结果返回true
    else
        return false;
}

void DataBase::clear()
{
    _db=openDataBase(_dbname);
    QSqlQuery sqlQuery(_db);
    QString create=QString("create table %1 %2").arg(_tbname).arg(_parameter);
    sqlQuery.exec(create);
    QString clearcontent=QString("delete from %1").arg(_tbname);
    sqlQuery.prepare(clearcontent);
    if(!sqlQuery.exec())
    {
        qDebug()<<sqlQuery.lastError();
    }
    _db.close();
}

void DataBase::deleteData(QString columnname, QString value)
{
    _db=openDataBase(_dbname);
    QSqlQuery sqlQuery(_db);
    QString deletecontent=QString("delete from %1 where %2 = ?").arg(_tbname).arg(columnname);
    sqlQuery.prepare(deletecontent);
    sqlQuery.addBindValue(value);
    if(!sqlQuery.exec())
    {
        qDebug()<<sqlQuery.lastError();
    }
    _db.close();
}

void DataBase::deleteData(QString columnname, int value)
{
    _db=openDataBase(_dbname);
    QSqlQuery sqlQuery(_db);
    QString deletecontent=QString("delete from %1 where %2 = %3").arg(_tbname).arg(columnname).arg(value);
    sqlQuery.exec(deletecontent);
    _db.close();
}

int DataBase::count()
{
    _db=openDataBase(_dbname);
    return select(_primaryKey).count();
}

QSqlDatabase DataBase::openDataBase(QString name)
{
    QSqlDatabase db;
    QString name_create=QString("%1.db").arg(name);
    if(QSqlDatabase::contains(name_create))
    {
        db=QSqlDatabase::database(name_create);
    }
    else
    {
        db=QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(name_create);
    }
    if(!db.open())
    {
        qDebug()<<db.lastError();
    }
    return db;
}
