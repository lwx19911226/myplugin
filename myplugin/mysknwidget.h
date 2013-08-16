#ifndef MYSKNWIDGET_H
#define MYSKNWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include "mainwindow.h"

class mysknwidget : public QWidget
{
    Q_OBJECT
public:
    explicit mysknwidget(MainWindow *getmain,QWidget *parent = 0);
    
    QTableWidget *p_tablewidget;
    MainWindow *pmain;
signals:
    
public slots:
    void mycopy();
};

#endif // MYSKNWIDGET_H
