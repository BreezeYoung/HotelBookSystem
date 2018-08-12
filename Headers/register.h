#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include "title_bar.h"
#include "platform.h"

namespace Ui {
class Register;
}
class Platform;

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(Platform * platform, QWidget *parent = 0);
    ~Register();
    void setup();

private slots:

    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Register *ui;
    QString _username;
    QString _phonenumber;
    QString _password;
    QString _rePassword;
    int _type;

    Platform * m_platform;
};

#endif // REGISTER_H
