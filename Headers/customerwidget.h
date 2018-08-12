#ifndef CUSTOMERWIDGET_H
#define CUSTOMERWIDGET_H
#include <QWidget>
#include "ui_customerwidget.h"
#include "title_bar.h"
#include "commonhelper.h"
#include "getdetails.h"

class MessageDetails;
class Platform;
class MainWindow;

namespace Ui {
class CustomerWidget;
}

class CustomerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerWidget(Platform *platform,MainWindow *parent = 0);
    ~CustomerWidget();

    void setup();
    void setUsername(const QString& username)
    {
        m_username=username;
    }
    void setPhonenumber(const QString& phonenumber)
    {
        m_phonenumber=phonenumber;
    }
    void setupMsgPage();
    void setupOrderPage();
    void setupRoomPage();

private slots:


    void on_personInfoButton_clicked();

    void on_homePageButton_clicked();

    void on_editInfoButton_pressed();

    void on_saveInfoButton_pressed();

    void on_gotoBookButton_clicked();

    void buttonRespond(int row,int column);

    void on_updateButton_clicked();

    void replyOrCheck(int row,int column);

    void autoSearchForCity(const QString &text);

    void autoSearchForHname(const QString &text);

    void autoSearchForRcate(const QString &text);

    void checkOutValid(const QDate &date);

    void on_bookButton_clicked();

    void roomNumBoxChangeValue(int row,int column);      //根据不同的点击单元更改不同的房间数量的选择的极限值

    void clickToDialog(int row,int column);

    void on_payButton_clicked();

    void on_refundButton_clicked();

    void on_finishButton_clicked();

    void on_cancelButton_clicked();

    void on_commentButton_clicked();

    void orderByClement(int logicalIndex);          //根据点击表头的对应列的内容进行排序，若为汉字则按拼音排序，若为数字则按数字大小排序

    void clickToGetDetails(int row,int column);

private:
    Ui::CustomerWidget *ui;
    MessageDetails *msgDetails_ui;
    getDetails *details_ui;

    //排序的标志
    int orderBy;            //根据大小排序，2表示从大到小，1表示从小到大，0表示初始排序

    Platform * m_platform;
    QString m_username;
    QString m_phonenumber;
    QString m_password;
    int m_id;
    int m_ordernum;
    double m_balance;
};

#endif // CUSTOMERWIDGET_H
