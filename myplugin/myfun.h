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
    static QString extract_str(QString getstr,int gettype);
    static QStringList name2strlist(QString getname);
    static QString name2str(QString getname,int getqsv);
    static QStringList intypestrlist(QString getname,int getqsv);
    //static QString need(QString,int);
    static QStringList outtypestrlist(QString getname,int getqsv);
    static QStringList inouttypestrlist(QString getname,int getqsv);
    static int name2blockcnt(QString getname,int getqsv);
    static bool notnil(QString getname,int getqsv,int geti);
    static QString name2trans(QString getname,int getqsv);
    static QStringList getfunstrlist(int getqsv,QStringList taglist);
    static QStringList getfuntagrmlist(bool visibility=true);
    static QString remark2tag(QString);
    static bool matchtaglist(QString getname,int getqsv, QStringList gettaglist);
    static bool matchtaglist_str(QString getstr,QStringList gettaglist);
    static bool matchqsv_str(QString getstr,int getqsv);
    static QList<int> getqsvlist_str(QString getstr);
    static QString name2remark(QString getname, int getqsv);

signals:
    
public slots:
    
};

#endif // MYFUN_H
