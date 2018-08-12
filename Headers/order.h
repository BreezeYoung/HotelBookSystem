#ifndef ORDER_H
#define ORDER_H

#include<QString>

typedef int stateType;   //0表示未支付，1表示已支付，2表示待退款，3表示已退款，4表示已完成（即退房），5表示取消订单

class Order
{
public:
    Order(int _id=0,int _roomID=0,int _customerID=0,int _roomNum=0,QString _checkInDay="",QString _checkOutDay="",double _cost=0,stateType _state=0)
        :id(_id),roomID(_roomID),customerID(_customerID),roomNum(_roomNum),checkInDay(_checkInDay),checkOutDay(_checkOutDay),cost(_cost),state(_state)
    {}
    ~Order(){}

    //获取成员属性的接口
    int getID() const {
        return id;
    }
    int getRoomID()const
    {
        return roomID;
    }
    int getCustomerID() const
    {
        return customerID;
    }
    int getRoomNum() const
    {
        return roomNum;
    }
    QString getCheckInDay() const{
        return checkInDay;
    }
    int getCheckInDayInt() const{
        return checkInDay.toInt();
    }
    QString getCheckOutDay() const{
        return checkOutDay;
    }
    int getCheckOutDayInt() const{
        return checkOutDay.toInt();
    }
    double getCost() const {
        return cost;
    }
    stateType getState() const {
        return state;
    }

    //修改成员属性的接口
    void setState(stateType newState)
    {
        state=newState;
    }

    QString toStringBystate()
    {
        QString result="";
        switch (state) {
        case 0:
            result=QString("未支付");
            break;
        case 1:
            result=QString("已支付");
            break;
        case 2:
            result=QString("待退款");
            break;
        case 3:
            result=QString("已退款");
            break;
        case 4:
            result=QString("已完成");
            break;
        case 5:
            result=QString("取消订单");
            break;
        default:
            break;
        }
        return result;
    }
private:
    int id;                 //订单编号
    int roomID;             //客房编号
    int customerID;         //顾客编号
    int roomNum;            //预订房间数量
    QString checkInDay;     //开始入住的日期
    QString checkOutDay;    //退房日期
    double cost;            //需支付的价钱
    stateType state;        //订单状态
};

#endif // ORDER_H
