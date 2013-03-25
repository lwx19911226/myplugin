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
    Q_ENUMS(skClass)
    Q_ENUMS(skProperty)
public:
    explicit mysk(QObject *parent=0):QObject(parent){
        owner=NULL;
    }
    mysk(const mysk &getcpy):QObject(getcpy.parent()){qWarning()<<"130325"<<getcpy.name;}
    enum skType{TriggerSkill=1,ViewAsSkill=2,DistanceSkill=3,FilterSkill=4,TargetModSkill=5};
    enum skAbb{vs=ViewAsSkill,trs=TriggerSkill,dts=DistanceSkill,fts=FilterSkill,tms=TargetModSkill};
    enum skClass{myvs=ViewAsSkill,mytrs=TriggerSkill,mydts=DistanceSkill,myfts=FilterSkill,mytms=TargetModSkill};
    static QString type2str(int gettype){return myobj::enumstr(&staticMetaObject,"skType",gettype);}
    static int str2type(QString getstr){return myobj::enumint(&staticMetaObject,"skType",getstr);}
    static QStringList typestrlist(){return myobj::enumstrlist(&staticMetaObject,"skType");}
    static QString type2abb(int gettype){return myobj::enumstr(&staticMetaObject,"skAbb",gettype);}
    static int abb2type(QString getstr){return myobj::enumint(&staticMetaObject,"skAbb",getstr);}
    static QString type2class(int gettype){return myobj::enumstr(&staticMetaObject,"skClass",gettype);}
    static QStringList typeclasslist(){return myobj::enumstrlist(&staticMetaObject,"skClass");}
    static void typelist(QList<int> &list){myobj::enumintlist(&staticMetaObject,"skType",list);}
    enum skProperty{Name=0,Translation=1,Owner=2,Description=3,Words=4};
    virtual QString property2str(int getproperty){return myobj::enumstr(metaObject(),"skProperty",getproperty);}
    virtual int str2property(QString getstr){return myobj::enumint(metaObject(),"skProperty",getstr);}
    virtual QStringList propertystrlist(bool b4tab=false){
        QStringList strlist=myobj::enumstrlist(metaObject(),"skProperty");
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
    virtual void iniObj();
    virtual myblock *iniBlock(QString getstr);
    virtual QStringList trans()=0;
    QStringList trans4avlobjlist(QString getstr="");
    //QStringList trans4design();
    void dotrans(QString getstr);
    virtual QStringList funtaglist(QString getstr){qWarning()<<"funtaglist:"<<getstr;return QStringList();}
    virtual QStringList eventstrlist();
    QString bstr2abb(QString getstr);
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
    QString property2str(int getproperty){
        if(myobj::enumcontains(metaObject(),"trsProperty",getproperty)){
            return myobj::enumstr(metaObject(),"trsProperty",getproperty);
        }
        else{return mysk::property2str(getproperty);}
    }
    int str2property(QString getstr){
        if(myobj::enumcontains(metaObject(),"trsProperty",getstr)){
            return myobj::enumint(metaObject(),"trsProperty",getstr);
        }
        else{return mysk::str2property(getstr);}
    }
    QStringList propertystrlist(bool b4tab=false){
        QStringList strlist=mysk::propertystrlist(b4tab);
        foreach(QString stri,myobj::enumstrlist(metaObject(),"trsProperty")){
            strlist.insert(strlist.indexOf(property2str(Description)),stri);
        }
        return strlist;
    }
    void propertymap_get(QMap<QString,QString> &strmap,QMap<QString,QStringList> &strlistmap,bool b4remark);
    void propertymap_set(QMap<QString, QString> &strmap, bool b4remark);
    void propertystr_set(QString getstr);
    QString propertystr_get();
    enum trsType{NotFrequent=0,Frequent=1,Compulsory=2,Limited=3,Wake=4};
    static QString subtype2str(int gettype){return myobj::enumstr(&staticMetaObject,"trsType",gettype);}
    static int str2subtype(QString getstr){return myobj::enumint(&staticMetaObject,"trsType",getstr,NotFrequent);}
    static QStringList subtypestrlist(){return myobj::enumstrlist(&staticMetaObject,"trsType");}
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
Q_DECLARE_METATYPE(mytrs)

class myvs:public mysk
{
    Q_OBJECT
    Q_ENUMS(vsProperty)
    Q_ENUMS(vsbType)
    Q_ENUMS(vsbAbb)
public:
    explicit myvs(QObject *parent=0):mysk(parent){
        vsn=1;objname_viewas=myobj::getconstlist_tag("ob").first();
    }
    int getType(){return ViewAsSkill;}    
    enum vsProperty{CardsNum=11,CardViewAs=12};
    QString property2str(int getproperty){
        if(myobj::enumcontains(metaObject(),"vsProperty",getproperty)){
            return myobj::enumstr(metaObject(),"vsProperty",getproperty);
        }
        else{return mysk::property2str(getproperty);}
    }
    int str2property(QString getstr){
        if(myobj::enumcontains(metaObject(),"vsProperty",getstr)){
            return myobj::enumint(metaObject(),"vsProperty",getstr);
        }
        else{return mysk::str2property(getstr);}
    }
    QStringList propertystrlist(bool b4tab=false){
        QStringList strlist=mysk::propertystrlist(b4tab);
        foreach(QString stri,myobj::enumstrlist(metaObject(),"vsProperty")){
            strlist.insert(strlist.indexOf(property2str(Description)),stri);
        }
        return strlist;
    }
    void propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap,bool b4remark);
    void propertymap_set(QMap<QString, QString> &strmap, bool b4remark);
    QString propertystr_get();
    void propertystr_set(QString getstr);
    enum vsbType{ViewFilter=0,EnabledAtPlay=1,EnabledAtResponse=2};
    enum vsbAbb{vs_vf=ViewFilter,vs_ep=EnabledAtPlay,vs_er=EnabledAtResponse};
    //static QString vsbtype2str(int gettype){return myobj::enumstr(staticMetaObject,"vsbType",gettype);}
    //static int str2vsbtype(QString getstr){return myobj::enumint(staticMetaObject,"vsbType",getstr);}
    //static bool vsbtypecontains(QString getstr){return myobj::enumcontains(staticMetaObject,"vsbType",getstr);}
    //static QString vsbtype2abb(int gettype){return myobj::enumstr(staticMetaObject,"vsbAbb",gettype);}
    QStringList funtaglist(QString getstr){
        QStringList strlist;
        if(getstr==myobj::enumstr(metaObject(),"vsbType",ViewFilter)){strlist<<"selected$";}
        if(getstr==myobj::enumstr(metaObject(),"vsbType",EnabledAtResponse)){strlist<<"pattern$";}
        return strlist;
    }
    //QStringList eventstrlist(){return myobj::enumstrlist(staticMetaObject,"vsbType");}

    int vsn;
    QString objname_viewas;    
    //QString bstr2abb(QString getstr);
    myblock *iniBlock(QString getstr);
    QString findRemarkByName_event(QString getname);    
    QStringList trans();
};
Q_DECLARE_METATYPE(myvs)

class mydts:public mysk
{
    Q_OBJECT
    Q_ENUMS(dtsbType)
    Q_ENUMS(dtsbAbb)
public:
    explicit mydts(QObject *parent=0):mysk(parent){}
    int getType(){return DistanceSkill;}
    enum dtsbType{CorrectFunc=0};
    enum dtsbAbb{dts_cf=CorrectFunc};
    QString findRemarkByName_event(QString getname){return getname;}
    QStringList trans();
};
Q_DECLARE_METATYPE(mydts)

class myfts:public mysk
{
    Q_OBJECT
    Q_ENUMS(ftsbType)
    Q_ENUMS(ftsbAbb)
public:
    explicit myfts(QObject *parent=0):mysk(parent){}
    int getType(){return FilterSkill;}
    enum ftsbType{ViewFilter=1};
    enum ftsbAbb{fts_vf=ViewFilter};
    QString objname_viewas;
    QString findRemarkByName_event(QString getname){return getname;}
    QStringList trans();
};
Q_DECLARE_METATYPE(myfts)

class mytms:public mysk
{
    Q_OBJECT
    Q_ENUMS(tmsbType)
    Q_ENUMS(tmsbAbb)
public:
    explicit mytms(QObject *parent=0):mysk(parent){}
    int getType(){return TargetModSkill;}
    enum tmsbType{};
    enum tmsbAbb{};
    void iniObj(){}
    QString findRemarkByName_event(QString getname){return getname;}
    QStringList trans(){return QStringList();}
};
Q_DECLARE_METATYPE(mytms)

#endif // MYTRS_H
