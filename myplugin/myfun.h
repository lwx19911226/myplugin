#ifndef MYFUN_H
#define MYFUN_H

#include <QObject>
#include "myobj.h"
class myblock;
class myfun : public QObject
{
    Q_OBJECT
    Q_ENUMS(iniFormat)
public:
    explicit myfun(QObject *parent = 0);
    enum iniFormat{Name=0,In_Type=1,Out_Type=2,Trans=3,Remark=4,Tag=5};
    //QString name;
    //QList<myobj *> objlist;
    //QList<myobj *> rtobjlist;
    //QList<myblock *> blocklist;
    //virtual void getrt(QList<myobj *> &list){list.append(rtobjlist);}
    //virtual void setrt(QList<QString> &rtnamelist,QList<QString> &rtrmlist,QString geteventstr);
    //virtual void setBlock(QList<QString> &blrmlist);
    //virtual void setrtrm(QList<QString> &rtrmlist);
    //virtual void setrtevent(QString getstr);
    //virtual bool rtNull(){return rtobjlist.isEmpty();}
    //virtual QStringList trans();
    static QStringList myfunlist;
    static QStringList myfuntaglist;
    static int globalint;
    static void myini();
    static QStringList need(QString);
    //static QString need(QString,int);
    static QStringList get(QString);
    static int getBlock_cnt(QString);
    static bool notnil(QString,int);
    static QString getTrans(QString);
    static QStringList getfunstrlist(QStringList taglist);
    static QStringList getfuntagrmlist(bool visibility=true);
    static QString remark2tag(QString);
    static bool matchTaglist(QString getname,QStringList gettaglist);
    static QString findRemarkByName(QString getname);
signals:
    
public slots:
    
};

#endif // MYFUN_H
