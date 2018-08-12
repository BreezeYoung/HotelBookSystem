#ifndef HOTEL_H
#define HOTEL_H
#include<QString>


class Hotel
{
public:
    Hotel(int _id=0,int _founderid=0,QString _hotelName="",QString _city="",QString _description="",bool _state=false,QString _filename="")
        :id(_id),founderID(_founderid),hotelName(_hotelName),city(_city),description(_description),isVerify(_state),m_filename(_filename)
    {}

    //获得成员属性的接口
    int getID() const{
        return id;
    }
    int getFounderID() const{
        return founderID;
    }
    QString getHotelName() const{
        return hotelName;
    }
    QString getCity() const{
        return city;
    }
    QString getDescription() const{
        return description;
    }
    bool getIsVerify() const{
        return isVerify;
    }
    int getIsVerifyInt() const
    {
        if(isVerify)
            return 1;
        else
            return 0;
    }
    QString getFilename() const
    {
        return m_filename;
    }

    //修改成员属性的接口
    void setHotelName(const QString &_hotelName)
    {
        hotelName=_hotelName;
    }
    void setIsverify(bool verify)
    {
        isVerify=verify;
    }
    void setDescription(const QString &_description)
    {
        description=_description;
    }
    void setFilename(QString filename)
    {
        m_filename=filename;
    }
private:
    int id;                    //酒店编号
    int founderID;             //创建者的ID，也即酒店代表的ID
    QString hotelName;         //酒店名称
    QString city;              //酒店所在城市
    QString description;       //酒店描述（只限100字以内描述）
    bool isVerify;             //是否通过审核
    QString m_filename;        //图片地址
};

#endif // HOTEL_H
