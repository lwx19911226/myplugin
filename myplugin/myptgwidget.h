#ifndef MYPTGWIDGET_H
#define MYPTGWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "mainwindow.h"


class myptgwidget : public QWidget
{
    Q_OBJECT
public:
    explicit myptgwidget(MainWindow *getmain,QWidget *parent = 0);
    MainWindow *pmain;
    QTableWidget *p_tablewidget;
    QLineEdit *p_lineedit;
    QCheckBox *p_checkbox_judge,*p_checkbox_or;
    QStringList ptstrlist;

signals:
    
public slots:
    void mygenerate();
    void mycopy();
    void mydel();
    void myjudge(bool checked);
};

#endif // MYPTGWIDGET_H
