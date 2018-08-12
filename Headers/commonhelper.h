#ifndef COMMONHELPER_H
#define COMMONHELPER_H
#include<QFile>
#include<QApplication>
#include<QHBoxLayout>
#include<QMessageBox>
#include"stable.h"
#include"title_bar.h"
class CommonHelper{
public:
    static void setStyle(const QString &style,QApplication &a){
        QFile file(style);
        if( file.open(QFile::ReadOnly))
        {
            QString styleSheet = QLatin1String(file.readAll());
            a.setStyleSheet(styleSheet);
            file.close();
        }
        else
        {
            QMessageBox::warning(NULL, "warning", "Open failed", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        }
    }
    static void setTitleBar(QWidget *a)
    {
        a->setWindowFlags(Qt::FramelessWindowHint | a->windowFlags());
        TitleBar *pTitleBar=new TitleBar(a);
        a->installEventFilter(pTitleBar);
        a->setWindowTitle("酒店预订服务与管理系统");
        a->setWindowIcon(QIcon(":/app1.png"));

        QVBoxLayout *pLayout = new QVBoxLayout();
        pLayout->addWidget(pTitleBar);
        pLayout->addStretch();
        pLayout->setSpacing(0);
        pLayout->setContentsMargins(0, 0, 0, 0);
        a->setLayout(pLayout);
    }
};
#endif // COMMONHELPER_H
