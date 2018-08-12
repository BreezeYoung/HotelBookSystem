#ifndef ROOMDETAILS_H
#define ROOMDETAILS_H

#include <QWidget>
#include<QString>

class Platform;

namespace Ui {
class RoomDetails;
}

class RoomDetails : public QWidget
{
    Q_OBJECT

public:
    explicit RoomDetails(Platform *p,QWidget *parent = 0);
    ~RoomDetails();

    void setHotelID(const int &hotelid)
    {
        m_hotelID=hotelid;
    }
    void setRoomID(const int &roomid)
    {
        m_roomID=roomid;
    }
    void setupForCreate();
    void setupForManageInfo();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_editButton_clicked();

    void on_saveButton_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_ButtonUpdatePhoto_pressed();

    void on_ButtonUpdatePhoto_2_pressed();

private:
    Ui::RoomDetails *ui;
    Platform *m_platform;

    bool isDiscount;
    int m_roomID;
    int m_hotelID;
    int m_number;
    double m_price;
    double m_rate;
    QString m_cate;
    QString m_descri;
    QString m_hotelName;
    QString m_filename;
};

#endif // ROOMDETAILS_H
