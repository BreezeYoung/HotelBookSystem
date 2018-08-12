#ifndef ADMINWIDGET_H
#define ADMINWIDGET_H

#include <QWidget>

class Platform;
class MainWindow;

namespace Ui {
class AdminWidget;
}

class AdminWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWidget(Platform *p,MainWindow *parent = 0);
    ~AdminWidget();

    void setPhoneNumber(QString phonenumber)
    {
        m_phonenumber = phonenumber;
    }
    void setup();
    void setupHotelPage();
    void setupInfoPage();
    void setupUserPage();

private slots:
    void on_editInfoButton_clicked();

    void on_saveInfoButton_clicked();

    void buttonRespond(int row,int column);

    void on_passButton_clicked();

    void on_reverifyButton_clicked();

    void on_personInfoButton_clicked();

    void on_pushButton_clicked();

    void on_checkUserButton_clicked();

private:
    Ui::AdminWidget *ui;
    Platform *m_platform;

    int m_id;         //管理员ID
    QString m_username;
    QString m_password;
    QString m_phonenumber;

};

#endif // ADMINWIDGET_H
