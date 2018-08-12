#ifndef HOTELMANAGERWIDGET_H
#define HOTELMANAGERWIDGET_H

#include <QWidget>
#include<QString>
#include "title_bar.h"
#include "commonhelper.h"
#include "foundhotelwidget.h"
#include "manageroomwidget.h"
#include "messagedetails.h"

class Platform;
class MainWindow;
class HotelDetails;

namespace Ui {
class HotelManagerWidget;
}

class HotelManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HotelManagerWidget(Platform *platform,MainWindow *parent = 0);
    ~HotelManagerWidget();

    void setup();
    void setupMsgPage();
    void setupHotelPage();
    void setupOrderPage();
    void setPhonenumber(const QString& phonenumber)
    {
        m_phonenumber=phonenumber;
    }
public slots:


private slots:
    void on_myMessageButton_clicked();

    void on_hotelManageButton_clicked();

    void on_orderManageButton_clicked();

    void on_editInfoButton_pressed();

    void on_pushButton_clicked();

    void on_saveInfoButton_pressed();

    void on_newHotelButton_clicked();

    void on_getInfoButton_clicked();

    void on_deleteHotelButton_clicked();

    void on_updateButton_clicked();

    void on_manageRoomButton_clicked();

    void replyOrCheck(int row,int column);

    void on_urgeButton_clicked();

    void on_refundButton_clicked();

    void buttonRespond(int row,int column);

private:
    Ui::HotelManagerWidget *ui;
    foundHotelWidget *found_ui;
    HotelDetails *details_ui;
    ManageRoomWidget *manageRoom_ui;
    MessageDetails *msgDetails_ui;

    Platform *m_platform;
    QString m_username;
    QString m_phonenumber;
    QString m_password;
    int m_id;
};

#endif // HOTELMANAGERWIDGET_H
