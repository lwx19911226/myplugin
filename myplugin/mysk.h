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
    Q_PROPERTY(QString NameProperty READ getName WRITE setName)
    Q_PROPERTY(QString TranslationProperty READ getTranslation WRITE setTranslation)
    Q_PROPERTY(QString OwnerProperty READ getOwnerProperty WRITE setOwnerProperty)
    Q_PROPERTY(QString DescriptionProperty READ getDescription WRITE setDescription)
    Q_PROPERTY(QString WordsProperty READ getWordsProperty WRITE setWordsProperty)
public:
    explicit mysk(QObject *parent=0):QObject(parent){
        owner=NULL;
    }
    mysk(const mysk &getcpy):QObject(getcpy.parent()){qWarning()<<"130325"<<getcpy.name;}
    enum skType{TriggerSkill=1,ViewAsSkill=2,DistanceSkill=3,FilterSkill=4,TargetModSkill=5};
    enum skAbb{vs=ViewAsSkill,trs=TriggerSkill,dts=DistanceSkill,fts=FilterSkill,tms=TargetModSkill};
    //enum skClass{myvs=ViewAsSkill,mytrs=TriggerSkill,mydts=DistanceSkill,myfts=FilterSkill,mytms=TargetModSkill};
    static QString type2str(int gettype){return myobj::enumstr(&staticMetaObject,"skType",gettype);}
    static int str2type(QString getstr){return myobj::enumint(&staticMetaObject,"skType",getstr);}
    static QStringList typestrlist(){return myobj::enumstrlist(&staticMetaObject,"skType");}
    static QString type2abb(int gettype){return myobj::enumstr(&staticMetaObject,"skAbb",gettype);}
    static int abb2type(QString getstr){return myobj::enumint(&staticMetaObject,"skAbb",getstr);}
    static QString type2class(int gettype){return "my"+type2abb(gettype);}
    static QStringList typeclasslist(){return mycode::mymdf(myobj::enumstrlist(&staticMetaObject,"skAbb"),"my");}
    static void typelist(QList<int> &list){myobj::enumintlist(&staticMetaObject,"skType",list);}
    enum skProperty{Name=0,Translation=1,Owner=2,Description=3,Words=4};
    static QString enumname_property(int gettype){return type2abb(gettype)+"Property";}
    static QString enumname_babb(int gettype){return type2abb(gettype)+"bAbb";}
    static QString enumname_btype(int gettype){return type2abb(gettype)+"bType";}
    virtual QString property2str(int getproperty){
        if(myobj::enumcontains(metaObject(),enumname_property(getType()).toUtf8(),getproperty)){
            return myobj::enumstr(metaObject(),enumname_property(getType()).toUtf8(),getproperty);
        }
        else{return myobj::enumstr(metaObject(),"skProperty",getproperty);}
    }
    virtual int str2property(QString getstr){
        if(myobj::enumcontains(metaObject(),enumname_property(getType()).toUtf8(),getstr)){
            return myobj::enumint(metaObject(),enumname_property(getType()).toUtf8(),getstr);
        }
        else{return myobj::enumint(metaObject(),"skProperty",getstr);}
    }
    virtual QStringList propertystrlist(bool b4tab=false){
        QStringList strlist=myobj::enumstrlist(metaObject(),"skProperty");
        if(b4tab){strlist<<property2str(Words);}
        foreach(QString stri,myobj::enumstrlist(metaObject(),enumname_property(getType()).toUtf8())){
            strlist.insert(strlist.indexOf(property2str(Description)),stri);
        }
        return strlist;
    }
    static QString property2prefix(int getproperty){
        switch(getproperty){
        case Description:return ":";
        case Words:return "$";
        default:return QString();
        }
    }
    virtual void propertymap_get(QMap<QString,QString> &strmap,bool b4remark);
    virtual void propertymap_set(QMap<QString,QString> &strmap,bool b4remark);
    virtual QString propertystr_get();
    //QStringList propertystr_get(bool front);
    virtual void propertystr_set(QString getstr);
    //void propertystr_set(QStringList getstrlist,bool front);
    bool propertystr_dvd(QString getstr,QString &getabb,QStringList &frlist,QStringList &midlist,QStringList &bklist);
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
    QString getName(){return name;}
    QString getTranslation(){return translation;}
    void setTranslation(QString getstr){translation=getstr;}
    QString getOwnerProperty();
    void setOwnerProperty(QString getstr);
    QString getDescription(){return description;}
    void setDescription(QString getstr){description=getstr;}
    QString getWordsProperty(){return wordslist.join(",,");}
    void setWordsProperty(QString getstr){wordslist=getstr.split(",,");}
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
    Q_PROPERTY(QString SubtypeProperty READ getSubtypeProperty WRITE setSubtypeProperty)
public:
    explicit mytrs(QObject *parent = 0):mysk(parent){
        subtype=NotFrequent;
    }
    enum trsProperty{Subtype=11};    
    //void propertymap_get(QMap<QString,QString> &strmap,QMap<QString,QStringList> &strlistmap,bool b4remark);
    //void propertymap_set(QMap<QString, QString> &strmap, bool b4remark);
    //void propertystr_set(QString getstr);
    //QString propertystr_get();
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
    QString getSubtypeProperty();
    void setSubtypeProperty(QString getstr);
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
    Q_PROPERTY(QString CardsNumProperty READ getCardsNumProperty WRITE setCardsNumProperty)
    Q_PROPERTY(QString CardViewAsProperty READ getCardViewAsProperty WRITE setCardViewAsProperty)
    Q_PROPERTY(QString CardViewAsPropertyRemark READ getCardViewAsPropertyRemark WRITE setCardViewAsPropertyRemark)
public:
    explicit myvs(QObject *parent=0):mysk(parent){
        vsn=0;objname_viewas=myobj::getconstlist_tag("ob").first();
    }
    int getType(){return ViewAsSkill;}    
    enum vsProperty{CardsNum=11,CardViewAs=12};    
    //void propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap,bool b4remark);
    //void propertymap_set(QMap<QString, QString> &strmap, bool b4remark);
    //QString propertystr_get();
    //void propertystr_set(QString getstr);
    enum vsbType{ViewFilter=0,EnabledAtPlay=1,EnabledAtResponse=2};
    enum vsbAbb{vs_vf=ViewFilter,vs_ep=EnabledAtPlay,vs_er=EnabledAtResponse};
    QStringList funtaglist(QString getstr){
        QStringList strlist;
        if(getstr==myobj::enumstr(metaObject(),"vsbType",ViewFilter)){strlist<<"selected$";}
        if(getstr==myobj::enumstr(metaObject(),"vsbType",EnabledAtResponse)){strlist<<"pattern$";}
        return strlist;
    }

    int vsn;
    QString objname_viewas;
    QString getCardsNumProperty();
    void setCardsNumProperty(QString getstr);
    QString getCardViewAsProperty();
    void setCardViewAsProperty(QString getstr);
    QString getCardViewAsPropertyRemark();
    void setCardViewAsPropertyRemark(QString getstr);
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
    QStringList funtaglist(QString getstr){
        if(getstr==myobj::enumstr(metaObject(),"dtsbType",CorrectFunc)){return QStringList();}
        return QStringList();
    }
    QStringList trans();
};
Q_DECLARE_METATYPE(mydts)

class myfts:public mysk
{
    Q_OBJECT
    Q_ENUMS(ftsbType)
    Q_ENUMS(ftsbAbb)
    Q_ENUMS(ftsProperty)
    Q_PROPERTY(QString CardViewAsProperty READ getCardViewAsProperty WRITE setCardViewAsProperty)
    Q_PROPERTY(QString CardViewAsPropertyRemark READ getCardViewAsPropertyRemark WRITE setCardViewAsPropertyRemark)
public:
    explicit myfts(QObject *parent=0):mysk(parent){
        objname_viewas=myobj::getconstlist_tag("ob").first();
    }
    int getType(){return FilterSkill;}
    enum ftsbType{ViewFilter=1};
    enum ftsbAbb{fts_vf=ViewFilter};
    enum ftsProperty{CardViewAs=11};
    //void propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap,bool b4remark);
    //void propertymap_set(QMap<QString, QString> &strmap, bool b4remark);
    //QString propertystr_get();
    //void propertystr_set(QString getstr);
    QString objname_viewas;
    QString getCardViewAsProperty();
    void setCardViewAsProperty(QString getstr);
    QString getCardViewAsPropertyRemark();
    void setCardViewAsPropertyRemark(QString getstr);
    QString findRemarkByName_event(QString getname){return getname;}
    QStringList funtaglist(QString getstr){
        QStringList strlist;
        if(getstr==myobj::enumstr(metaObject(),"ftsbType",FilterSkill)){strlist<<"room$";}
        return strlist;
    }
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
