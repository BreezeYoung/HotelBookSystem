#ifndef HOTELDETAILS_H
#define HOTELDETAILS_H

#include <QWidget>
#include <QString>
#include"title_bar.h"

class Platform;

namespace Ui {
class HotelDetails;
}

class HotelDetails : public QWidget
{
    Q_OBJECT

public:
    explicit HotelDetails(Platform *p,QWidget *parent = 0);
    ~HotelDetails();

    //设置成员属性的接口
    void setHotelName(QString _hotelName)
    {
        m_hotelName=_hotelName;
    }
    void setDescription(QString _description)
    {
        m_description=_description;
    }
    void setCity(QString _city)
    {
        m_city=_city;
    }
    void setHotelID(int _hotelid)
    {
        m_hotelID=_hotelid;
    }

    void setup();
private slots:
    void on_editButton_pressed();

    void on_saveButton_pressed();

private:
    Ui::HotelDetails *ui;
    Platform *m_platform;

    QString m_hotelName;
    QString m_city;
    QString m_description;
    QString m_roomType;
    QString m_filename;
    int m_roomNum;
    int m_hotelID;
};

#endif // HOTELDETAILS_H
