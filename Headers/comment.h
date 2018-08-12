#ifndef COMMENT_H
#define COMMENT_H
#include<QString>

class Comment
{
public:
    Comment(int id=0,int orderid=0,int hotelid=0,int stars=0)
        :m_id(id),m_orderid(orderid),m_hotelid(hotelid),m_stars(stars)
    {}
    ~Comment(){}

    //获得成员属性的接口
    int getID()const
    {
        return m_id;
    }
    int getOrderID() const
    {
        return m_orderid;
    }
    int getHotelID() const
    {
        return m_hotelid;
    }
    int getStars() const
    {
        return m_stars;
    }

    //修改成员属性的接口
    void setStars(int stars)
    {
        m_stars=stars;
    }

private:
    int m_id;               //评价编号
    int m_orderid;          //订单编号
    int m_hotelid;          //酒店编号
    int m_stars;            //评价星数
};

#endif // COMMENT_H
