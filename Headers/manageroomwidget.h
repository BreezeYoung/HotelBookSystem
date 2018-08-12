#ifndef MANAGEROOMWIDGET_H
#define MANAGEROOMWIDGET_H

#include <QWidget>
#include <QString>
#include "roomdetails.h"

class Platform;

namespace Ui {
class ManageRoomWidget;
}

class ManageRoomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ManageRoomWidget(Platform *p, QWidget *parent = 0);
    ~ManageRoomWidget();

    void setHotelID(const int &hotelid)
    {
        m_hotelID=hotelid;
    }
    void setup();
    void setupTablePage();

private slots:
    void on_updateButton_clicked();

    void on_deleteRoomButton_clicked();

    void on_createRoomButton_clicked();

    void on_manageInfoButton_clicked();

private:
    Ui::ManageRoomWidget *ui;
    Platform *m_platform;
    RoomDetails *details_ui;

    int m_roomID;
    int m_hotelID;
    int m_number;
    int m_reserve;
    double m_price;
    double m_rate;
    QString m_hotelName;
};

#endif // MANAGEROOMWIDGET_H
