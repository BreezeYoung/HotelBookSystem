#ifndef USER_H
#define USER_H

#include<QString>
//用下标来表示用户类型
static const QString Identity[3]={"customer","admin","hotelrepre"};

/*
 *用户类
 *包含用户的基本信息
 *用于平台管理员和酒店代表的信息管理
 */
class MyUser
{
public:
    MyUser(int id=0,QString username="",QString phoneNumber="",QString password="")
        :_username(username),_phoneNumber(phoneNumber),_password(password),_id(id)
    {}
    virtual ~MyUser(){}

    //获得成员属性的接口
    int getID() const{
        return _id;
    }
    QString getUsername() const{
        return _username;
    }
    QString getPhoneNumber() const{
        return _phoneNumber;
    }
    QString getPassword() const{
        return _password;
    }

    //修改成员属性的接口
    void setID(const int& i){
        _id=i;
    }
    void setUsername(const QString& username){
        _username=username;
    }
    void setPhoneNumber(const QString& phoneNumber){
        _phoneNumber=phoneNumber;
    }
    void setPassword(const QString& password){
        _password=password;
    }
    bool operator ==(MyUser &user);

protected:
    int _id;                 //在数据库中的主键
    QString _username;       //用户在应用中的昵称
    QString _phoneNumber;    //以手机号作为用户登录名
    QString _password;       //密码
};


/*
 * 顾客类
 * 继承用户类
 * 新增_orderNum表示顾客所拥有的订单
 * 新增m_balance表示顾客所拥有的余额
 */
class Customer:public MyUser
{
public:
    Customer(int id=0,QString username="",QString phoneNumber="",QString password="",int ordernum=0,double balance=0)
        :MyUser(id,username,phoneNumber,password),_ordernum(ordernum),m_balance(balance)
    {}
    Customer(const MyUser& user):MyUser(user)
    {
        _ordernum=0;
        m_balance=0;
    }
    //获得对象属性的接口
    int getOrderNum() const
    {
        return _ordernum;
    }
    double getBalance() const
    {
        return m_balance;
    }

    //修改成员属性的接口
    int setOrderNum(int ordernum)
    {
        _ordernum=ordernum;
    }
    void setBalance(double balance)
    {
        m_balance=balance;
    }
private:
    int _ordernum;
    double m_balance;
};

#endif  //USER_H
