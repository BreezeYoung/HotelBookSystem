#ifndef FOUNDHOTELWIDGET_H
#define FOUNDHOTELWIDGET_H

#include <QWidget>
#include "title_bar.h"

class Platform;
class HotelManagerWidget;

namespace Ui {
class foundHotelWidget;
}

class foundHotelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit foundHotelWidget(Platform *p,HotelManagerWidget *parent = 0);
    ~foundHotelWidget();

    void setup();
    void setFounderID(const int &founderid)
    {
        m_founderID=founderid;
    }
private slots:
    void on_createButton_clicked();

    void on_ButtonUpdatePhoto_pressed();

private:
    Ui::foundHotelWidget *ui;

    int m_founderID;
    Platform *m_platform;
    QString m_hotelName;
    QString m_city;
    QString m_description;
    QString m_filename;
};

#endif // FOUNDHOTELWIDGET_H
