#include<QDebug>
#include<QDate>
#include "platform.h"
#include "user.h"
void Platform::ReadDatabase()
{
    QSqlDatabase db;
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("HotelBookSystem.db");
    if(!db.open())
        qDebug()<<"not open";
    QSqlQuery query(db);

    //读取数据库中customer的内容
    if(query.exec("select * from customer"))
    {
        while(query.next())
        {
            m_id = query.value(0).toInt();
            m_username = query.value(1).toString();
            m_phonenumber = query.value(2).toString();
            m_password = query.value(3).toString();
            m_ordernum = query.value(4).toInt();
            m_balance = query.value(5).toDouble();
            m_customer[m_id] = new Customer(m_id,m_username,m_phonenumber,m_password,m_ordernum,m_balance);
        }
    }

    //读取数据库中admin的内容
    if(query.exec("select * from admin"))
    {
        while(query.next())
        {
            m_id = query.value(0).toInt();
            m_username = query.value(1).toString();
            m_phonenumber = query.value(2).toString();
            m_password = query.value(3).toString();
            m_admin[m_id] = new MyUser(m_id,m_username,m_phonenumber,m_password);
        }
    }

    //读取数据库中hotelrepre的内容
    if(query.exec("select * from hotelrepre"))
    {
        while(query.next())
        {
            m_id = query.value(0).toInt();
            m_username = query.value(1).toString();
            m_phonenumber = query.value(2).toString();
            m_password = query.value(3).toString();
            m_hotelrepre[m_id] = new MyUser(m_id,m_username,m_phonenumber,m_password);
        }
    }

    //读取数据库中myorder的内容
    if(query.exec("select * from myorder"))
    {
        while(query.next())
        {
            m_id = query.value(0).toInt();
            m_roomID = query.value(1).toInt();
            m_customerID = query.value(2).toInt();
            m_roomNum = query.value(3).toInt();
            m_checkInDay = query.value(4).toString();
            m_username = query.value(5).toString();    //实际含义为checkoutday
            m_cost = query.value(6).toDouble();
            m_state = query.value(7).toInt();
            m_myorder[m_id] = new Order(m_id,m_roomID,m_customerID,m_roomNum,m_checkInDay,m_username,m_cost,m_state);
        }
    }

    //读取数据库中room的内容
    if(query.exec("select * from room"))
    {
        while(query.next())
        {
            m_id = query.value(0).toInt();
            m_hotelID = query.value(1).toInt();
            m_roomNum = query.value(2).toInt();
            m_roomID = query.value(3).toInt();     //实际含义为已预定的客房数
            m_balance = query.value(4).toDouble(); //实际含义为价格
            m_cost = query.value(5).toDouble();    //实际含义为折率
            m_isPaid = query.value(6).toBool();    //实际含义为是否优惠
            m_roomCat = query.value(7).toString();
            m_comment =query.value(8).toString();  //实际含义为客房描述
            m_filename = query.value(9).toString();
            m_room[m_id] = new Room(m_id,m_hotelID,m_roomNum,m_roomID,m_balance,m_cost,m_isPaid,m_roomCat,m_comment,m_filename);
        }
    }


    //读取数据库中hotel的内容
    if(query.exec("select * from hotel"))
    {
        while(query.next())
        {
            m_id = query.value(0).toInt();
            m_hotelID = query.value(1).toInt();           //实际含义为创建者ID
            m_username = query.value(2).toString();       //实际含义为酒店名称
            m_checkInDay = query.value(3).toString();     //实际含义为所在城市
            m_roomCat = query.value(4).toString();        //实际含义为酒店描述
            m_isPaid = query.value(5).toBool();           //实际含义为是否核实
            m_filename = query.value(6).toString();
            m_hotel[m_id] = new Hotel(m_id,m_hotelID,m_username,m_checkInDay,m_roomCat,m_isPaid,m_filename);
        }
    }

    //读取数据库中comment的内容
    if(query.exec("select * from comment"))
    {
        while(query.next())
        {
            m_id = query.value(0).toInt();
            m_roomID = query.value(1).toInt();            //实际含义为订单号
            m_hotelID = query.value(2).toInt();
            m_roomNum = query.value(3).toInt();           //实际含义为星数
            m_comments[m_id] = new Comment(m_id,m_roomID,m_hotelID,m_roomNum);
        }
    }

    //读取数据库中message的内容
    if(query.exec("select * from message"))
    {
        while(query.next())
        {
            m_id = query.value(0).toInt();
            m_username = query.value(1).toString();          //实际含义为发送者名称
            m_password = query.value(2).toString();          //实际含义为接收者名称
            m_checkInDay = query.value(3).toString();        //实际含义为发送日期
            m_comment = query.value(4).toString();           //实际含义为消息内容
            m_roomID = query.value(5).toInt();               //实际含义为回复的ID
            m_message[m_id] = new Message(m_id,m_username,m_password,m_checkInDay,m_comment,m_roomID);
        }
    }
}

//程序即将运行结束，执行对数据库内更新数据的操作
void Platform::WriteIntoDatabase()
{
    QString p1;
    QString p2;
    QString p3;
    double p4;
    int p5;
    int p6;
    int p7;
    double p8;
    QString p9;
    DataBase database("HotelBookSystem");
    QVector<EditFlag>::iterator it;
    QVector<EditFlag>::iterator it1;
    bool flag = false;      //用于判断酒店和房间创建语句之后又有相应的删除语句
    for(it=operatingWord.begin();it!=operatingWord.end();it++)
    {
        database.setTableName(it->tbname);
        if(it->tbname=="customer")
        {
            database.setParameter(customer_value);
        }
        else if(it->tbname=="admin")
        {
            database.setParameter(admin_value);
        }
        else if(it->tbname=="hotelrepre")
        {
            database.setParameter(hotelrepre_value);
        }
        else if(it->tbname=="myorder")
        {
            database.setParameter(myorder_value);
        }
        else if(it->tbname=="room")
        {
            database.setParameter(room_value);
        }
        else if(it->tbname=="hotel")
        {
            database.setParameter(hotel_value);
        }
        else if(it->tbname=="comment")
        {
            database.setParameter(comment_value);
        }
        else if(it->tbname=="message")
        {
            database.setParameter(message_value);
        }
        if(it->operType==1)
        {
            database.update(it->id,this);
        }
        else if(it->operType==2)
        {
            database.deleteData("id",it->id);
        }
        else if(it->operType==3)
        {
            if(it->tbname=="customer")
            {
                p1=m_customer[it->id]->getUsername();
                p2=m_customer[it->id]->getPhoneNumber();
                p3=m_customer[it->id]->getPassword();
                database.insert(it->id,p1,p2,p3,0,0,0,0,0);
            }
            else if(it->tbname=="hotelrepre")
            {
                p1=m_hotelrepre[it->id]->getUsername();
                p2=m_hotelrepre[it->id]->getPhoneNumber();
                p3=m_hotelrepre[it->id]->getPassword();
                database.insert(it->id,p1,p2,p3,0,0,0,0,0);
            }
            else if(it->tbname=="myorder")
            {
                p1=m_myorder[it->id]->getCheckInDay();
                p2=m_myorder[it->id]->getCheckOutDay();
                p4=double(m_myorder[it->id]->getState());
                p5=m_myorder[it->id]->getRoomID();
                p6=m_myorder[it->id]->getCustomerID();
                p7=m_myorder[it->id]->getRoomNum();
                p8=m_myorder[it->id]->getCost();
                database.insert(it->id,p1,p2,"",p4,p5,p6,p7,p8);
            }
            else if(it->tbname=="hotel")
            {
                for(it1=operatingWord.begin();it1!=operatingWord.end();it1++)
                {
                    if(it1->id==it->id&&it1->tbname==it->tbname&&it1->operType==2)
                    {
                        flag = true;
                        break;
                    }
                }
                if(!flag)
                {
                    p1=m_hotel[it->id]->getHotelName();
                    p2=m_hotel[it->id]->getCity();
                    p3=m_hotel[it->id]->getDescription();
                    p6=m_hotel[it->id]->getFounderID();
                    p9=m_hotel[it->id]->getFilename();
                    database.insertForHotel(it->id,p1,p2,p3,p9,p6);
                }
            }
            else if(it->tbname=="room")
            {
                for(it1=operatingWord.begin();it1!=operatingWord.end();it1++)
                {
                    if(it1->id==it->id&&it1->tbname==it->tbname&&it1->operType==2)
                    {
                        flag = true;
                        break;
                    }
                }
                if(!flag)
                {
                    p1=m_room[it->id]->getCate();
                    p2=m_room[it->id]->getDescription();
                    p4=m_room[it->id]->getPrice();
                    p5=m_room[it->id]->getHotelID();
                    p6=m_room[it->id]->getNumber();
                    p7=m_room[it->id]->getReserve();
                    p8=m_room[it->id]->getRate();
                    p3=m_room[it->id]->getFilename();
                    database.insert(it->id,p1,p2,p3,p4,p5,p6,p7,p8);
                }
            }
            else if(it->tbname=="comment")
            {
                p5=m_comments[it->id]->getOrderID();
                p6=m_comments[it->id]->getHotelID();
                p7=m_comments[it->id]->getStars();
                database.insert(it->id,"","","",0,p5,p6,p7,0);
            }
            else if(it->tbname=="message")
            {
                p1=m_message[it->id]->getSenderName();
                p2=m_message[it->id]->getReceiverName();
                p3=m_message[it->id]->getContent();
                database.insertForMessage(it->id,p1,p2,m_message[it->id]->getSendDate(),p3);
            }
        }
    }
}

//用于检查登录是否有效，true为有效，false为无效
bool Platform::checkValid(QString phonenumber,QString password,int type)
{
    int flag = 0;
    bool find = false;
    QMap<int,Customer*>::iterator it1;
    QMap<int,MyUser*>::iterator it2;
    switch (type) {
    case 0:
        for(it1=m_customer.begin();it1!=m_customer.end();it1++)
        {
            if(it1.value()->getPhoneNumber()==phonenumber)
            {
                find = true;
                break;
            }
        }
        if(find && it1.value()->getPassword()==password)
        {
            flag = 1;
        }
        break;
    case 1:
        for(it2=m_admin.begin();it2!=m_admin.end();it2++)
        {
            if(it2.value()->getPhoneNumber()==phonenumber)
            {
                find = true;
                break;
            }
        }
        if(find && it2.value()->getPassword()==password)
        {
            flag = 1;
        }
        break;
    case 2:
        for(it2=m_hotelrepre.begin();it2!=m_hotelrepre.end();it2++)
        {
            if(it2.value()->getPhoneNumber()==phonenumber)
            {
                find = true;
                break;
            }
        }
        if(find && it2.value()->getPassword()==password)
        {
            flag = 1;
        }
        break;
    default:
        break;
    }
    if(flag==1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Platform::getMaxID(int type)     //根据type的不同来返回不同表格的maxid+1
{
    int maxid = 0;
    switch (type) {
    case 0:
        if(m_customer.count()!=0)
        {
            maxid=m_customer.lastKey()+1;
        }
        break;
    case 1:
        if(m_admin.count()!=0)
        {
            maxid=m_admin.lastKey()+1;
        }
        break;
    case 2:
        if(m_hotelrepre.count()!=0)
        {
            maxid=m_hotelrepre.lastKey()+1;
        }
        break;
    case 3:
        if(m_myorder.count()!=0)
        {
            maxid=m_myorder.lastKey()+1;
        }
        break;
    case 4:
        if(m_room.count()!=0)
        {
            maxid=m_room.lastKey()+1;
        }
        break;
    case 5:
        if(m_hotel.count()!=0)
        {
            maxid=m_hotel.lastKey()+1;
        }
        break;
    case 6:
        if(m_comments.count()!=0)
        {
            maxid=m_comments.lastKey()+1;
        }
        break;
    case 7:
        if(m_message.count()!=0)
        {
            maxid=m_message.lastKey()+1;
        }
        break;
    default:
        break;
    }
    return maxid;
}

bool Platform::registerSearch(QString value, int searchItem)    //searchItem为1表示查询手机号，searchItem为2表示查询用户名
{
    int flag = 0;
    QMap<int, Customer *>::iterator it1;
    QMap<int, MyUser *>::iterator it2;
    if(searchItem==1)
    {
        for(it1=m_customer.begin();it1!=m_customer.end();it1++)
        {
            if(it1.value()->getPhoneNumber()==value)
            {
                flag = 1;
                break;
            }
        }
        if(flag==1)
        {
            return true;
        }
        for(it2=m_admin.begin();it2!=m_admin.end();it2++)
        {
            if(it2.value()->getPhoneNumber()==value)
            {
                flag = 1;
                break;
            }
        }
        if(flag==1)
        {
            return true;
        }
        for(it2=m_hotelrepre.begin();it2!=m_hotelrepre.end();it2++)
        {
            if(it2.value()->getPhoneNumber()==value)
            {
                flag = 1;
                break;
            }
        }
    }
    else if(searchItem==2)
    {
        for(it1=m_customer.begin();it1!=m_customer.end();it1++)
        {
            if(it1.value()->getUsername()==value)
            {
                flag = 1;
                break;
            }
        }
        if(flag==1)
        {
            return true;
        }
        for(it2=m_admin.begin();it2!=m_admin.end();it2++)
        {
            if(it2.value()->getUsername()==value)
            {
                flag = 1;
                break;
            }
        }
        if(flag==1)
        {
            return true;
        }
        for(it2=m_hotelrepre.begin();it2!=m_hotelrepre.end();it2++)
        {
            if(it2.value()->getUsername()==value)
            {
                flag = 1;
                break;
            }
        }
    }
    if(flag==1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Platform::foundSearch(QString value,QString city)      //创建酒店时的查重
{
    int flag = 0;
    QMap<int, Hotel *>::iterator it;
    for(it=m_hotel.begin();it!=m_hotel.end();it++)
    {
        if(it.value()->getHotelName()==value)
        {
            if(it.value()->getCity()==city)
            {
                flag=1;
                break;
            }
        }
    }
    if(flag==1)
        return true;
    else
        return false;
}

void Platform::clear()        //清空动态生成的所有对象
{
    QMap<int, Customer *>::iterator it1;
    QMap<int, MyUser *>::iterator it2;
    QMap<int, Order *>::iterator it3;
    QMap<int, Hotel *>::iterator it4;
    QMap<int, Room *>::iterator it5;
    QMap<int, Comment *>::iterator it6;
    QMap<int, Message *>::iterator it7;
    for(it1=m_customer.begin();it1!=m_customer.end();it1++)
    {
        delete it1.value();
        it1.value()=0;
    }
    for(it2=m_admin.begin();it2!=m_admin.end();it2++)
    {
        delete it2.value();
        it2.value()=0;
    }
    for(it2=m_hotelrepre.begin();it2!=m_hotelrepre.end();it2++)
    {
        delete it2.value();
        it2.value()=0;
    }
    for(it3=m_myorder.begin();it3!=m_myorder.end();it3++)
    {
        delete it3.value();
        it3.value()=0;
    }
    for(it4=m_hotel.begin();it4!=m_hotel.end();it4++)
    {
        delete it4.value();
        it4.value()=0;
    }
    for(it5=m_room.begin();it5!=m_room.end();it5++)
    {
        delete it5.value();
        it5.value()=0;
    }
    for(it6=m_comments.begin();it6!=m_comments.end();it6++)
    {
        delete it6.value();
        it6.value()=0;
    }
    for(it7=m_message.begin();it7!=m_message.end();it7++)
    {
        delete it7.value();
        it7.value()=0;
    }
}

bool Platform::roomFoundSearch(int hotelid, QString roomcate)
{
    bool flag = false;
    QMap<int,Room *>::iterator it;
    for(it=m_room.begin();it!=m_room.end();it++)
    {
        if(it.value()->getHotelID()==hotelid)
        {
            if(it.value()->getCate()==roomcate)
            {
                flag = true;
            }
        }
    }
    return flag;
}

bool Platform::belongToHotels(int roomid,int founderid)
{
    int hotelid=m_room[roomid]->getHotelID();
    if(m_hotel[hotelid]->getFounderID()==founderid)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Platform::refund(QString username, double cost)
{
    EditFlag p;
    int id = -1;
    double original = 0;
    QMap<int,Customer *>::iterator it;
    for(it=m_customer.begin();it!=m_customer.end();it++)
    {
        if(it.value()->getUsername()==username)
        {
            id=it.key();
            break;
        }
    }
    if(id!=-1)
    {
        original=m_customer[id]->getBalance();
        m_customer[id]->setBalance(cost+original);
        p.id=id;
        p.tbname="customer";
        p.operType=1;
        addOperatingWord(p);
        return true;
    }
    else
    {
        return false;
    }
}

double Platform::getCommentGrade(int hotelid)
{
    double grade = 0;
    int number = 0;
    QMap<int, Comment *>::iterator it;
    for(it=m_comments.begin();it!=m_comments.end();it++)
    {
        if(it.value()->getHotelID()==hotelid)
        {
            number++;
            grade += double(it.value()->getStars());
        }
    }
    if(number!=0)
    {
        grade=grade/number;
    }
    return grade;
}

double Platform::getCost(QDate checkinday, QDate checkoutday, int roomnum, bool isdiscout, double rate, double price)
{
    if(isdiscout)
        price = rate*price;
    int days = checkoutday.toJulianDay()-checkinday.toJulianDay();
    double cost = 0;
    cost = price*roomnum*days;
    return cost;
}
