#ifndef GETDETAILS_H
#define GETDETAILS_H

#include <QWidget>

class Platform;

namespace Ui {
class getDetails;
}

class getDetails : public QWidget
{
    Q_OBJECT

public:
    explicit getDetails(Platform *p,QWidget *parent = 0);
    ~getDetails();

    void setHotelID(int hotelid)
    {
        m_hotelid=hotelid;
    }
    void setRoomID(int roomid)
    {
        m_roomid=roomid;
    }
    void setup();
private:
    Ui::getDetails *ui;
    Platform *m_platform;

    int m_hotelid;
    int m_roomid;
    QString m_roomFilename;
    QString m_hotelFilename;
};

#endif // GETDETAILS_H
