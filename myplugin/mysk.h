#ifndef MYTRS_H
#define MYTRS_H

#include <QObject>
#include "mydo.h"

class mygeneral;
class mysys;
class mysk:public QObject
{
    Q_OBJECT
public:
    explicit mysk(QObject *parent=0):QObject(parent){
        owner=NULL;
    }
    enum skType{Skill=0,ViewAsSkill=1,TriggerSkill=2};
    static QString type2str(int gettype){
        switch(gettype){
        case TriggerSkill:return tr("TriggerSkill");
        case ViewAsSkill:return tr("ViewAsSkill");
        default:qWarning()<<"type2str:"<<gettype;return QString();
        }
    }
    static int str2type(QString getstr){
        if(getstr==type2str(TriggerSkill)){return TriggerSkill;}
        if(getstr==type2str(ViewAsSkill)){return ViewAsSkill;}
        return -1;
    }
    static QString type2abb(int gettype){
        switch(gettype){
        case TriggerSkill:return "trs";
        case ViewAsSkill:return "vs";
        default:qWarning()<<"type2abb:"<<gettype;return QString();
        }
    }
    static int abb2type(QString getstr){
        if(getstr==type2abb(TriggerSkill)){return TriggerSkill;}
        if(getstr==type2abb(ViewAsSkill)){return ViewAsSkill;}
        qWarning()<<"abb2type"<<getstr;
        return Skill;
    }
    enum skProperty{Name=0,Translation=1,Owner=2,Description=3,Words=4};
    static QString property2str(int getproperty){
        switch(getproperty){
        case Name:return tr("Name");
        case Translation:return tr("Translation");
        case Owner:return tr("Owner");
        case Description:return tr("Description");
        case Words:return tr("Words");
        default:qWarning()<<"property2str:"<<getproperty;return QString();
        }
    }
    static int str2property(QString getstr){
        if(getstr==property2str(Name)){return Name;}
        if(getstr==property2str(Translation)){return Translation;}
        if(getstr==property2str(Owner)){return Owner;}
        if(getstr==property2str(Description)){return Description;}
        if(getstr==property2str(Words)){return Words;}
        qWarning()<<"str2property:"<<getstr;
        return 0;
    }
    static int str2property(QString getstr,int gettype);
    static QStringList propertystrlist(bool b4tab=false){
        QStringList strlist;
        strlist<<property2str(Name)<<property2str(Translation)<<property2str(Owner)
              <<property2str(Description)<<property2str(Words);
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
    virtual void propertymap_get(QMap<QString,QString> &strmap,QMap<QString,QStringList> &strlistmap);
    virtual void propertymap_set(QMap<QString,QString> &strmap);
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
    virtual int getType(){return Skill;}
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
    QStringList need4block(QString getstr);
    myobj *findObjByName(QString getname,QString getstr);
    myblock *findBlockByName(QString);
    myfunction *findFuncByObj(myobj *);
    virtual QString findRemarkByName_event(QString getname)=0;
    QString findRemarkByName_block(QString getname);
    virtual void rfrObj(){}
    virtual void iniObj(){}    
    virtual QStringList trans()=0;
    QStringList trans4avlobjlist(QString getstr="");
    //QStringList trans4design();
    void dotrans(QString getstr);
    virtual QStringList funtaglist(QString getstr){qWarning()<<"funtaglist:"<<getstr;return QStringList();}
    virtual QStringList eventstrlist()=0;
    mysys *getsys();
    void sig_update();
    void myshow(){foreach(myblock *ip,blocklist){ip->myshow();}}
};

class mytrs : public mysk
{
    Q_OBJECT
public:
    explicit mytrs(QObject *parent = 0):mysk(parent){
        subtype=NotFrequent;
    }
    enum trsProperty{Subtype=11};
    static QString property2str(int getproperty){
        switch(getproperty){
        case Subtype:return tr("Subtype");
        default:return mysk::property2str(getproperty);
        }
    }
    static int str2property(QString getstr){
        if(getstr==property2str(Subtype)){return Subtype;}
        return mysk::str2property(getstr);
    }
    static QStringList propertystrlist(bool b4tab=false){
        QStringList strlist=mysk::propertystrlist(b4tab);
        strlist.insert(strlist.indexOf(property2str(Description)),property2str(Subtype));
        return strlist;
    }
    void propertymap_get(QMap<QString,QString> &strmap,QMap<QString,QStringList> &strlistmap);
    void propertymap_set(QMap<QString, QString> &strmap);
    void propertystr_set(QString getstr);
    QString propertystr_get();
    enum trsType{NotFrequent=0,Frequent=1,Compulsory=2,Limited=3,Wake=4};
    static QString subtype2str(int gettype){
        switch(gettype){
        case NotFrequent:return "NotFrequent";
        case Frequent:return "Frequent";
        case Compulsory:return "Compulsory";
        case Limited:return "Limited";
        case Wake:return "Wake";
        default:return subtype2str(NotFrequent);
        }
    }
    static int str2subtype(QString getstr){
        if(getstr==subtype2str(NotFrequent)){return NotFrequent;}
        if(getstr==subtype2str(Frequent)){return Frequent;}
        if(getstr==subtype2str(Compulsory)){return Compulsory;}
        if(getstr==subtype2str(Limited)){return Limited;}
        if(getstr==subtype2str(Wake)){return Wake;}
        qWarning()<<"TRSstr2type:"<<getstr;
        return NotFrequent;
    }    
    static QStringList typestrlist(){
        QStringList strlist;
        strlist<<subtype2str(NotFrequent)<<subtype2str(Frequent)<<subtype2str(Compulsory)<<subtype2str(Limited)<<subtype2str(Wake);
        return strlist;
    }
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
public:
    explicit myvs(QObject *parent=0):mysk(parent){
        vsn=1;objname_viewas=myobj::getconstlist_tag("ob").first();
    }
    int getType(){return ViewAsSkill;}    
    enum vsProperty{CardsNum=11,CardViewAs=12};
    static QString property2str(int getproperty){
        switch(getproperty){
        case CardsNum:return tr("CardsNum");
        case CardViewAs:return tr("CardViewAs");
        default:return mysk::property2str(getproperty);
        }
    }
    static int str2property(QString getstr){
        if(getstr==property2str(CardsNum)){return CardsNum;}
        if(getstr==property2str(CardViewAs)){return CardViewAs;}
        return mysk::str2property(getstr);
    }
    static QStringList propertystrlist(bool b4tab=false){
        QStringList strlist=mysk::propertystrlist(b4tab);
        strlist.insert(strlist.indexOf(property2str(Description)),property2str(CardsNum));
        strlist.insert(strlist.indexOf(property2str(Description)),property2str(CardViewAs));
        return strlist;
    }
    void propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap);
    void propertymap_set(QMap<QString, QString> &strmap);
    enum vsbType{ViewFilter=0,EnabledAtPlay=1,EnabledAtResponse=2};
    static QString vsbtype2str(int gettype){
        switch(gettype){
        case ViewFilter:return "viewfilter";
        case EnabledAtPlay:return "enabledatplay";
        case EnabledAtResponse:return "enabledatresponse";
        default:;
        }
        qWarning()<<"vsbtype2str:"<<gettype;
        return "";
    }
    static QStringList getvsbstrlist(){
        QStringList strlist;
        strlist<<vsbtype2str(ViewFilter)<<vsbtype2str(EnabledAtPlay)<<vsbtype2str(EnabledAtResponse);
        return strlist;
    }
    QStringList funtaglist(QString getstr){
        QStringList strlist;
        if(getstr==vsbtype2str(ViewFilter)){strlist<<"selected$";}
        if(getstr==vsbtype2str(EnabledAtResponse)){strlist<<"pattern$";}
        return strlist;
    }
    QStringList eventstrlist(){return getvsbstrlist();}

    int vsn;
    QString objname_viewas;    
    void iniObj();    
    QString findRemarkByName_event(QString getname);    
    QStringList trans();
};

#endif // MYTRS_H
