#ifndef MYOBJ_H
#define MYOBJ_H

#include <QObject>
#include <QMetaType>
#include <QMetaEnum>
#include <QString>
#include <QList>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QTextCodec>
#include <QMessageBox>
#include <QFileDialog>
#include <QtGlobal>
#include <QDebug>


class myobj : public QObject
{
    Q_OBJECT
    Q_ENUMS(objType)
    Q_ENUMS(iniFormat)
public:
    explicit myobj(QObject *parent = 0):QObject(parent){
        blockstr="";noDeclaration=false;isVerified=false;isGlobal=false;
        //isDynamic=true;
    }
    enum objType{all=0,myplayer=1,mysplayer=2,mycard=3,mybool=4,mylist=5,myplayerlist=6,mysplayerlist=7,mycardlist=8,mynumlist=9,mynum=10,mystr=11,mystrc=12};
    enum iniFormat{Name=0,Type=1,Remark=2,Extra=3};
    static QString enumstr(const QMetaObject *mob,QByteArray getname,int getint){
        return tr(mob->enumerator(mob->indexOfEnumerator(getname)).valueToKey(getint));
    }
    static int enumint(const QMetaObject *mob,QByteArray getname,QString getstr,int defaultint=-1){
        QMetaEnum myenum=mob->enumerator(mob->indexOfEnumerator(getname));
        for(int i=0;i<myenum.keyCount();i++){
            if(getstr==tr(myenum.key(i))){return myenum.value(i);}
        }
        qWarning()<<"enumint"<<getname<<getstr;
        return defaultint;
    }
    static QStringList enumstrlist(const QMetaObject *mob,QByteArray getname){
        QStringList strlist;
        QMetaEnum myenum=mob->enumerator(mob->indexOfEnumerator(getname));
        for(int i=0;i<myenum.keyCount();i++){
            strlist<<enumstr(mob,getname,myenum.value(i));
        }
        return strlist;
    }
    static void enumintlist(const QMetaObject *mob,QByteArray getname,QList<int> &list){
        QMetaEnum myenum=mob->enumerator(mob->indexOfEnumerator(getname));
        for(int i=0;i<myenum.keyCount();i++){
            list<<myenum.value(i);
        }
    }
    static bool enumcontains(const QMetaObject *mob,QByteArray getname,QString getstr){
        return enumstrlist(mob,getname).contains(getstr);
    }
    static bool enumcontains(const QMetaObject *mob,QByteArray getname,int getint){
        return enumcontains(mob,getname,enumstr(mob,getname,getint));
    }
    static int enumcnt(const QMetaObject *mob,QByteArray getname){
        return mob->enumerator(mob->indexOfEnumerator(getname)).keyCount();
    }

    QString name;
    int type;
    QString remark;
    QString blockstr;
    bool noDeclaration;
    //bool isDynamic;//generated by function
    bool isVerified;
    bool isGlobal;
    static QStringList myconstlist;
    static QStringList myconsttaglist;

    virtual bool matchType(int gettype){        
        return isSubtype(gettype,type) or (name=="nil");
    }
    virtual bool matchBlock(QString getstr){
        return (blockstr=="") or (blockstr==getstr);
    }    
    static QString type2str(int gettype){return enumstr(&staticMetaObject,"objType",gettype);}
    static int str2type(QString getstr){
        /*
        if(getstr.startsWith(type2str(Mylist))){return Mylist;}
        if(getstr.startsWith(type2str(Mysplayerlist))){return Mysplayerlist;}
        if(getstr.startsWith(type2str(Mycardlist))){return Mycardlist;}
        if(getstr.startsWith(type2str(Mynumlist))){return Mynumlist;}
        if(getstr.startsWith(type2str(Myplayer))){return Myplayer;}
        if(getstr.startsWith(type2str(Mysplayer))){return Mysplayer;}
        if(getstr.startsWith(type2str(Mycard))){return Mycard;}
        if(getstr.startsWith(type2str(Mynum))){return Mynum;}
        if(getstr.startsWith(type2str(Mystrc))){return Mystrc;}
        if(getstr.startsWith(type2str(Mystr))){return Mystr;}
        if(getstr.startsWith(type2str(Mybool))){return Mybool;}
        if(getstr.startsWith(type2str(All))){return All;}
        qWarning()<<"OBJstr2type:"<<getstr;
        return All;
        */
        QMetaEnum myenum=staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("objType"));
        QString str="";
        int ii=-1;
        for(int i=0;i<myenum.keyCount();i++){
            QString tstr(myenum.key(i));
            if(getstr.startsWith(tstr)&&(tstr.length()>str.length())){str=tstr;ii=i;}
        }
        return myenum.value(ii);
    }
    static bool b4input(int &gettype){
        if(gettype==mystr){gettype=mystrc;}
        return isSubtype(mynum,gettype)||isSubtype(mystr,gettype);
    }
    /*
    static bool b4vrstr(int gettype){
        if(isSubtype(Mynum,gettype)){return true;}
        if(isSubtype(Mystr,gettype)){return true;}
        if(isSubtype(Mylist,gettype)){return true;}
        return false;
    }
    */
    static bool isSubtype(int gettype0,int gettype){
        if(gettype0==all){return true;}
        if(gettype0==mylist){
            if(gettype==mysplayerlist){return true;}
            if(gettype==mycardlist){return true;}
            if(gettype==mynumlist){return true;}
        }
        if(gettype0==myplayerlist){
            if(gettype==mysplayerlist){return true;}
        }
        if(gettype0==myplayer){
            if(gettype==mysplayer){return true;}
        }
        if(gettype0==mystr){
            if(gettype==mystrc){return true;}
        }
        return (gettype0==gettype);
    }
    static QString gettypestr(QString getstr){
        QString typestr=type2str(str2type(getstr));
        if(getstr.startsWith(typestr)){return typestr;}
        qWarning()<<"typestr"<<getstr;
        return QString();
    }
    static QString gettypesuffix(QString getstr){
        QString typestr=gettypestr(getstr);
        if(getstr.startsWith(typestr)){return getstr.mid(typestr.length());}
        qWarning()<<"typesuffix"<<getstr;
        return QString();
    }
    static QString suitstr(QString getstr){
        if(getstr.contains(QRegExp("Spade"))){return "spade";}
        if(getstr.contains(QRegExp("Club"))){return "club";}
        if(getstr.contains(QRegExp("Heart"))){return "heart";}
        if(getstr.contains(QRegExp("Diamond"))){return "diamond";}
        return getstr;
    }
    static QString num2str(int getnum){
        if(getnum<1||getnum>13){return "-";}
        QByteArray array="-A23456789-JQK";
        if(getnum==10){return "10";}
        else{
            return QString(array.at(getnum));
        }
    }
    static QString objname_nullification(){return "nullification";}

    QString trans(){
        if(type==mystrc){return QString("\"%1\"").arg(name);}
        return name;
    }
    static void myini();
    static bool isConst(QString getstr);
    static QStringList transConst(QString getstr);
    static void newConst(QList<myobj *> &list,QString getbl,QObject *getpf,bool only=false);
    static QStringList getconstlist_tag(QString);
    static QStringList getconstrmlist_tag(QString);
    static QStringList getconsttaglist();
    static QString remark2name(QString);
    static QString name2remark(QString);
    static QString remark2tag(QString);
    static bool matchTag(QString getname,QString gettag);
signals:
    
public slots:
    
};

#endif // MYOBJ_H
