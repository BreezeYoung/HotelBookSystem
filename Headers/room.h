#ifndef ROOM_H
#define ROOM_H
#include<QString>

class Room
{
public:
    Room(int id=0,int hotelid=0,int number=0,int reserve=0,double price=0,double rate=0,bool isDiscount=false,QString cat="",QString description="",QString filename="")
        :m_id(id),m_hotelid(hotelid),m_number(number),m_reserve(reserve),m_price(price),m_rate(rate),m_isDiscount(isDiscount),m_category(cat),m_description(description),m_filename(filename)
    {}

    //获得成员属性的接口
    int getID() const
    {
        return m_id;
    }
    int getHotelID() const
    {
        return m_hotelid;
    }
    int getNumber() const
    {
        return m_number;
    }
    int getReserve() const
    {
        return m_reserve;
    }
    double getPrice() const
    {
        return m_price;
    }
    double getRate() const
    {
        return m_rate;
    }
    bool getIsDiscount() const
    {
        return m_isDiscount;
    }
    int getIsDiscountInt() const
    {
        if(m_isDiscount)
            return 1;
        else
            return 0;
    }
    QString getCate() const
    {
        return m_category;
    }
    QString getDescription() const
    {
        return m_description;
    }
    QString getFilename() const
    {
        return m_filename;
    }

    //修改成员属性的接口
    void setNumber(int number)
    {
        m_number=number;
    }
    void setReserve(int reserve)
    {
        m_reserve=reserve;
    }
    void setPrice(double price)
    {
        m_price=price;
    }
    void setRate(double rate)
    {
        m_rate=rate;
    }
    void setIsDiscount(bool isdiscount)
    {
        m_isDiscount=isdiscount;
    }
    void setCate(QString category)
    {
        m_category=category;
    }
    void setDescription(QString description)
    {
        m_description=description;
    }

    void declineReserve(int roomnum)
    {
        m_reserve -= roomnum;
    }
    void addReserve(int roomnum)
    {
        m_reserve += roomnum;
    }
    void setFilename(QString filename)
    {
        m_filename=filename;
    }

private:
    int m_id;                //客房编号
    int m_hotelid;           //所属酒店编号
    int m_number;            //客房数量
    int m_reserve;           //被预订的数量
    double m_price;          //价格
    double m_rate;           //优惠折数
    bool m_isDiscount;       //是否优惠
    QString m_category;      //客房类型，相当于客房名字
    QString m_description;   //客房描述
    QString m_filename;      //图片地址
};

#endif // ROOM_H
