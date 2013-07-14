#ifndef MYEVENT_H
#define MYEVENT_H

#include <QObject>
#include "myobj.h"

class myevent : public QObject
{
    Q_OBJECT
    Q_ENUMS(iniFormat)
public:
    explicit myevent(QObject *parent = 0);
    enum iniFormat{Name=0,Data_Type=1,Data_Name=2,Data_Remark=3,Data_Trans=4,Remark=5,Extra=6};
    static QString trans(QString geteventstr){return "sgs."+geteventstr;}
    static QStringList myeventlist;
    static void myini();
    static QStringList geteventstrlist(int getqsv);
    static void getavlobjlist(QString,QList<myobj *> &list,QObject *getpf);
    static QStringList trans4eventdata(QString getstr,bool front=true);
    static QString findRemarkByName(QString getname);
    static bool isEvent(QString getname);

signals:
    
public slots:
    
};

#endif // MYEVENT_H
