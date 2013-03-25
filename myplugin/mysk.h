#ifndef MYTRS_H
#define MYTRS_H

#include <QObject>
#include "mydo.h"

class mygeneral;
class mysys;
class mysk:public QObject
{
    Q_OBJECT
    Q_ENUMS(skType)
    Q_ENUMS(skAbb)
    Q_ENUMS(skProperty)
public:
    explicit mysk(QObject *parent=0):QObject(parent){
        owner=NULL;
    }
    enum skType{ViewAsSkill=1,TriggerSkill=2,DistanceSkill=3,FilterSkill=4,TargetModSkill=5};
    enum skAbb{vs=ViewAsSkill,trs=TriggerSkill,dts=DistanceSkill,fts=FilterSkill,tms=TargetModSkill};
    static QString type2str(int gettype){return myobj::enumstr(staticMetaObject,"skType",gettype);}
    static int str2type(QString getstr){return myobj::enumint(staticMetaObject,"skType",getstr);}
    static QStringList typestrlist(){return myobj::enumstrlist(staticMetaObject,"skType");}
    static QString type2abb(int gettype){return myobj::enumstr(staticMetaObject,"skAbb",gettype);}
    static int abb2type(QString getstr){return myobj::enumint(staticMetaObject,"skAbb",getstr);}
    enum skProperty{Name=0,Translation=1,Owner=2,Description=3,Words=4};
    static QString property2str(int getproperty){return myobj::enumstr(staticMetaObject,"skProperty",getproperty);}
    static int str2property(QString getstr){return myobj::enumint(staticMetaObject,"skProperty",getstr);}
    static int str2property(QString getstr,int gettype);
    static QStringList propertystrlist(bool b4tab=false){
        QStringList strlist=myobj::enumstrlist(staticMetaObject,"skProperty");
        if(b4tab){strlist<<property2str(Words);}
        return strlist;
    }
    static QString property2prefix(int getproperty){
        switch(getproperty){
        case Description:return ":";
        case Words:return "$";
        default:qWarning()<<"property2prefix"<<getproperty;return QString();
        }
    }
    virtual void propertymap_get(QMap<QString,QString> &strmap,QMap<QString,QStringList> &strlistmap,bool b4remark);
    virtual void propertymap_set(QMap<QString,QString> &strmap,bool b4remark);
    virtual QString propertystr_get();
    QStringList propertystr_get(bool front);
    virtual void propertystr_set(QString getstr);
    void propertystr_set(QStringList getstrlist,bool front);
    bool propertystr_dvd(QString getstr,QStringList &frlist,QStringList &midlist,QStringList &bklist);
    QString name;
    mygeneral *owner;
    QString translation;
    QString description;
    QStringList wordslist;

    QList<myblock *> blocklist;
    QList<myobj *> avlobjlist;
    //QList<myobj *> usdobjlist;

    static int globalint;
    virtual int getType()=0;
    void setOwner(mygeneral *getp);
    void setDefaultName();
    void setName(QString getname);
    void getavlobjlist(int gettype,QList<myobj *> &list,QString getstr,bool b4nil);
    //void getusdobjlist(int gettype,QList<myobj *> &list,QString getstr="");
    //void addusdobjlist(QList<myobj *> &list);
    //void addusdobjlist(myobj *getp);
    void addFunction(QString geteventstr,QString getblockstr,
                             QString getfunstr,QList<myobj *> &getobjlist,
                             QStringList &getrtrmlist,QStringList &getblrmlist,bool b4redo=false);
    void undo();
    void redo();
    bool removeObj(myobj *getp);    
    myobj *findObjByName(QString getname,QString getstr);
    myblock *findBlockByName(QString);
    myfunction *findFuncByObj(myobj *);
    virtual QString findRemarkByName_event(QString getname)=0;
    QString findRemarkByName_block(QString getname);
    //virtual void rfrObj(){}
    virtual void iniObj()=0;
    virtual myblock *iniBlock(QString getstr);
    virtual QStringList trans()=0;
    QStringList trans4avlobjlist(QString getstr="");
    //QStringList trans4design();
    void dotrans(QString getstr);
    virtual QStringList funtaglist(QString getstr){qWarning()<<"funtaglist:"<<getstr;return QStringList();}
    virtual QStringList eventstrlist()=0;
    virtual QStringList blockstrlist(QString getstr);
    mysys *getsys();
    void sig_update();
    void myshow(){foreach(myblock *ip,blocklist){ip->myshow();}}
};

class mytrs : public mysk
{
    Q_OBJECT
    Q_ENUMS(trsType)
    Q_ENUMS(trsProperty)
public:
    explicit mytrs(QObject *parent = 0):mysk(parent){
        subtype=NotFrequent;
    }
    enum trsProperty{Subtype=11};
    static QString property2str(int getproperty){
        if(myobj::enumcontains(staticMetaObject,"trsProperty",getproperty)){
            return myobj::enumstr(staticMetaObject,"trsProperty",getproperty);
        }
        else{return mysk::property2str(getproperty);}
    }
    static int str2property(QString getstr){
        if(myobj::enumcontains(staticMetaObject,"trsProperty",getstr)){
            return myobj::enumint(staticMetaObject,"trsProperty",getstr);
        }
        else{return mysk::str2property(getstr);}
    }
    static QStringList propertystrlist(bool b4tab=false){
        QStringList strlist=mysk::propertystrlist(b4tab);
        foreach(QString stri,myobj::enumstrlist(staticMetaObject,"trsProperty")){
            strlist.insert(strlist.indexOf(property2str(Description)),stri);
        }
        return strlist;
    }
    void propertymap_get(QMap<QString,QString> &strmap,QMap<QString,QStringList> &strlistmap,bool b4remark);
    void propertymap_set(QMap<QString, QString> &strmap, bool b4remark);
    void propertystr_set(QString getstr);
    QString propertystr_get();
    enum trsType{NotFrequent=0,Frequent=1,Compulsory=2,Limited=3,Wake=4};
    static QString subtype2str(int gettype){return myobj::enumstr(staticMetaObject,"trsType",gettype);}
    static int str2subtype(QString getstr){return myobj::enumint(staticMetaObject,"trsType",getstr,NotFrequent);}
    static QStringList subtypestrlist(){return myobj::enumstrlist(staticMetaObject,"trsType");}
    static QString type2trans(int gettype){return "sgs.Skill_"+subtype2str(gettype);}
    QStringList funtaglist(QString getstr){
        if(!myevent::isEvent(getstr)){qWarning()<<"funtaglist:"<<getstr;}
        QStringList strlist;
        strlist<<"room$";
        return strlist;
    }
    QStringList eventstrlist(){
        return myevent::geteventstrlist();
    }

    int subtype;
    int getType(){return TriggerSkill;}
    void iniObj();
    QStringList trans();
    QString findRemarkByName_event(QString getname);
};

class myvs:public mysk
{
    Q_OBJECT
    Q_ENUMS(vsProperty)
    Q_ENUMS(vsbType)
public:
    explicit myvs(QObject *parent=0):mysk(parent){
        vsn=1;objname_viewas=myobj::getconstlist_tag("ob").first();
    }
    int getType(){return ViewAsSkill;}    
    enum vsProperty{CardsNum=11,CardViewAs=12};
    static QString property2str(int getproperty){
        if(myobj::enumcontains(staticMetaObject,"vsProperty",getproperty)){
            return myobj::enumstr(staticMetaObject,"vsProperty",getproperty);
        }
        else{return mysk::property2str(getproperty);}
    }
    static int str2property(QString getstr){
        if(myobj::enumcontains(staticMetaObject,"vsProperty",getstr)){
            return myobj::enumint(staticMetaObject,"vsProperty",getstr);
        }
        else{return mysk::str2property(getstr);}
    }
    static QStringList propertystrlist(bool b4tab=false){
        QStringList strlist=mysk::propertystrlist(b4tab);
        foreach(QString stri,myobj::enumstrlist(staticMetaObject,"vsProperty")){
            strlist.insert(strlist.indexOf(property2str(Description)),stri);
        }
        return strlist;
    }
    void propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap,bool b4remark);
    void propertymap_set(QMap<QString, QString> &strmap, bool b4remark);
    QString propertystr_get();
    void propertystr_set(QString getstr);
    enum vsbType{ViewFilter=0,EnabledAtPlay=1,EnabledAtResponse=2};
    static QString vsbtype2str(int gettype){return myobj::enumstr(staticMetaObject,"vsbType",gettype);}
    //static QStringList getvsbstrlist(){return myobj::enumstrlist(staticMetaObject,"vsbType");}
    QStringList funtaglist(QString getstr){
        QStringList strlist;
        if(getstr==vsbtype2str(ViewFilter)){strlist<<"selected$";}
        if(getstr==vsbtype2str(EnabledAtResponse)){strlist<<"pattern$";}
        return strlist;
    }
    QStringList eventstrlist(){return myobj::enumstrlist(staticMetaObject,"vsbType");}

    int vsn;
    QString objname_viewas;    
    void iniObj();
    myblock *iniBlock(QString getstr);
    QString findRemarkByName_event(QString getname);    
    QStringList trans();
};

#endif // MYTRS_H
