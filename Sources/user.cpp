#include "user.h"

/*
//Users Users::UsersData[3]={Users(),Users(),Users()};

User * User::getPointer(const User &user){
    User *pointer;
    switch (user.getType()) {
    case 0:
        pointer=new Customer(user);
        break;
    default:
        pointer=new User(user);
        break;
    }
    return pointer;
}

void User::insert()   //单APP模式下保存至静态变量
{
    //Users::UsersData[_type].add(new User(*this));
}

bool User::operator ==(User& user){
    if(user.getId()==_id&&user.getPhoneNumber()==_phoneNumber&&user.getPassword()==_password&&user.getType()==_type)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Customer::saveOrderId(){     //单APP模式下保存至静态变量
    _id=Users::UsersData[0].maxId()+1;
    Users::UsersData[0].add(new Customer(*this));
}

void Customer::insert(){      //单APP模式下保存至静态变量
    _id=Users::UsersData[0].maxId()+1;
    Users::UsersData[0].add(new Customer(*this));
}
*/
/*
 * Users的构造函数
 * 提取用户信息
 */
/*
Users::Users(int type):m_type(type)
{
    *this=Users::UsersData[m_type];   //从静态变量中提取
}
*/
/*
 * 复制构造函数
 * 完成深复制
 */
/*
Users::Users(const Users &users){
    clear();
    for(int i=0;i<users.count();++i)
    {
        add(users[i]);
    }
}

Users::~Users(){
    clear();
}

int Users::maxId(){
    int max=0;
    for(int i=0;i<count();i++)
    {

    }
}

//void Users::save(){
//
//}

void Users::clear(){
    for(int i=0;i<_users.count();i++)
    {
        auto p=_users[i];
        delete p;
        _users[i]=0;
    }
    _users.clear();
}

void Users::add(const User *user){   //添加一个用户的信息
    User * u;
    switch (user->getType()) {
    case 0:
        u=new Customer(*(Customer *)user);
        break;
    default:
        u=new User(*user);
        break;
    }
    _users.append(u);
}

void Users::add(const Users &users){   //将users的用户全部添加到_users中
    for(int i=0;i<users.count();i++)
    {
        add(users[i]);
    }
}

void Users::remove(User *user){   //移除用户
    for(int i=0;i<count();i++)
    {
        if(*(_users[i])==*user)
        {
            delete _users[i];
            _users[i]=0;
            _users.removeAt(i);
            return;
        }
    }
}

//用于编辑用户信息
void Users::edit(int index, QString phonenumber, QString password)
{
    _users[index]->setPhoneNumber(phonenumber);
    _users[index]->setPassword(password);
}

bool Users::contains(const QString &phonenumber){
    for(int i=0;i<count();i++)
    {
        if(_users[i]->getPhoneNumber()==phonenumber)
            return true;
    }
    return false;
}

Users Users::search(const QString &key){
    Users searchResult;
    for(int i=0;i<count();i++)
    {
        if(_users[i]->getPhoneNumber().contains(key)||_users[i]->getPassword().contains(key))
            searchResult.add(_users[i]);
    }
    return searchResult;
}

int Users::indexOf(User *user){    //查找编号，失败返回-1
    for(int i=0;i<count();i++)
    {
        if(*(_users[i])==*user)
        {
            return i;
        }
    }
    return -1;
}

User * Users::userValue(const QString &phonenumber){   //找到手机号为phonenumber的用户，失败返回空指针
    for(int i=0;i<count();i++)
    {
        if(_users[i]->getPhoneNumber()==phonenumber)
            return _users[i];
    }
    return 0;
}

Users& Users::operator =(const Users & users){
    if(this==&users)
        return *this;
    else
    {
        clear();
        for(int i=0;i<users.count();i++)
        {
            this->add(users[i]);
        }
        m_type=users.getType();
        return *this;
    }
}
*/
/*
QVector<QStringList> Users::transferTableContents(){
    QStringList number;
    QStringList type;
    QStringList phonenumber;
    QStringList password;
    for(int i=0;i<count();i++)
    {
        number.append();
    }
}
*/
