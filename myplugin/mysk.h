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
    enum skType{TriggerSkill=1,ViewAsSkill=2,DistanceSkill=3,FilterSkill=4,ProhibitSkill=5,MaxCardsSkill=6,TargetModSkill=7,ExistingSkill=8};
    enum skAbb{trs=TriggerSkill,vs=ViewAsSkill,dts=DistanceSkill,fts=FilterSkill,prs=ProhibitSkill,mcs=MaxCardsSkill,tms=TargetModSkill,exs=ExistingSkill};
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
        const QMetaObject *mob=metaObject();
        if(myobj::enumcontains(mob,enumname_property(getType()).toUtf8(),getproperty)){
            return myobj::enumstr(mob,enumname_property(getType()).toUtf8(),getproperty);
        }
        else{return myobj::enumstr(mob,"skProperty",getproperty);}
    }
    virtual int str2property(QString getstr){
        const QMetaObject *mob=metaObject();
        if(myobj::enumcontains(mob,enumname_property(getType()).toUtf8(),getstr)){
            return myobj::enumint(mob,enumname_property(getType()).toUtf8(),getstr);
        }
        else{return myobj::enumint(mob,"skProperty",getstr);}
    }
    virtual QStringList propertystrlist(bool b4tab=false){
        const QMetaObject *mob=metaObject();
        QStringList strlist=myobj::enumstrlist(mob,"skProperty");
        if(b4tab){strlist<<property2str(Words);}
        foreach(QString stri,myobj::enumstrlist(mob,enumname_property(getType()).toUtf8())){
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
    virtual QString findRemarkByName_event(QString getname){return getname;}
    QString findRemarkByName_block(QString getname);
    QString findRemarkByName_obj(QString getname,QString getstr,bool b4func);
    QString findRemarkByName_obj(myobj *tgtobj,myfunction *pfunc, bool b4func);
    //virtual void rfrObj(){}
    virtual void iniObj();
    virtual myblock *iniBlock(QString getstr);
    virtual QStringList trans()=0;
    virtual QStringList trans4avlobjlist(QString getstr);
    QStringList trans4avlobjlist(QString getstr,QString abbstr);
    //QStringList trans4design();
    void dotrans(QString getstr);
    virtual QStringList funtaglist(QString getstr){qWarning()<<"funtaglist:"<<getstr;return QStringList();}
    virtual QStringList eventstrlist();
    virtual QStringList blockstrlist(QString getstr);
    mysys *getsys();
    void sig_update();
    static QString default4skname(){return "local selfskname=self:objectName()";}
    void myshow(){foreach(myblock *ip,blocklist){ip->myshow();}}
};

class mytrs : public mysk
{
    Q_OBJECT
    Q_ENUMS(trsType)
    Q_ENUMS(trsProperty)
    Q_ENUMS(cantriggerType)
    Q_PROPERTY(QString SubtypeProperty READ getSubtypeProperty WRITE setSubtypeProperty)
    Q_PROPERTY(QString CanTriggerProperty READ getCanTriggerProperty WRITE setCanTriggerProperty)
    Q_PROPERTY(QString CanTriggerPropertyRemark READ getCanTriggerPropertyRemark WRITE setCanTriggerPropertyRemark)
public:
    explicit mytrs(QObject *parent = 0):mysk(parent){
        subtype=NotFrequent;cantrigger=SkillOwnerAlive;
    }
    enum trsProperty{Subtype=11,CanTrigger=12};
    //void propertymap_get(QMap<QString,QString> &strmap,QMap<QString,QStringList> &strlistmap,bool b4remark);
    //void propertymap_set(QMap<QString, QString> &strmap, bool b4remark);
    //void propertystr_set(QString getstr);
    //QString propertystr_get();
    enum trsType{NotFrequent=0,Frequent=1,Compulsory=2,Limited=3,Wake=4};
    enum cantriggerType{SkillOwnerAlive=0,TriggerAlive=1,Always=2};
    static QString subtype2str(int gettype){return myobj::enumstr(&staticMetaObject,"trsType",gettype);}
    static int str2subtype(QString getstr){return myobj::enumint(&staticMetaObject,"trsType",getstr,NotFrequent);}
    static QStringList subtypestrlist(){return myobj::enumstrlist(&staticMetaObject,"trsType");}
    static QString type2trans(int gettype){return "sgs.Skill_"+subtype2str(gettype);}
    static QString cantrigger2str(int gettype){return myobj::enumstr(&staticMetaObject,"cantriggerType",gettype);}
    static int str2cantrigger(QString getstr){return myobj::enumint(&staticMetaObject,"cantriggerType",getstr,SkillOwnerAlive);}
    static QStringList cantriggerstrlist(){return myobj::enumstrlist(&staticMetaObject,"cantriggerType");}
    QStringList funtaglist(QString getstr){
        QStringList strlist;
        strlist<<"room$"<<"rtb$";
        //if(getstr=="CanTrigger"){return strlist;}
        if(!myevent::isEvent(getstr)){qWarning()<<"funtaglist:"<<getstr;}        
        strlist<<getstr+"$";
        return strlist;
    }
    QStringList eventstrlist();

    int subtype;
    int cantrigger;
    int getType(){return TriggerSkill;}
    QString getSubtypeProperty();
    void setSubtypeProperty(QString getstr);
    QString getCanTriggerProperty();
    void setCanTriggerProperty(QString getstr);
    QString getCanTriggerPropertyRemark();
    void setCanTriggerPropertyRemark(QString getstr);
    void iniObj();
    QStringList trans();
    QStringList trans4avlobjlist(QString getstr);
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
    Q_PROPERTY(QString TargetPlayersNumProperty READ getTargetPlayersNumProperty WRITE setTargetPlayersNumProperty)
public:
    explicit myvs(QObject *parent=0):mysk(parent){
        vsn=0;objname_viewas=myobj::getconstlist_tag("ob").first();tgtn=1;
    }
    int getType(){return ViewAsSkill;}    
    enum vsProperty{CardsNum=11,CardViewAs=12,TargetPlayersNum=13};
    //void propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap,bool b4remark);
    //void propertymap_set(QMap<QString, QString> &strmap, bool b4remark);
    //QString propertystr_get();
    //void propertystr_set(QString getstr);
    enum vsbType{ViewFilter=0,EnabledAtPlay=1,EnabledAtResponse=2,PlayerFilter=3,SkillCardUse=4};
    enum vsbAbb{vs_vf=ViewFilter,vs_ep=EnabledAtPlay,vs_er=EnabledAtResponse,vs_pf=PlayerFilter,vs_scu=SkillCardUse};
    QStringList funtaglist(QString getstr){
        const QMetaObject *mob=metaObject();
        QStringList strlist;
        if(getstr==myobj::enumstr(mob,"vsbType",ViewFilter)){strlist<<"selected$"<<"rtb$";}
        if(getstr==myobj::enumstr(mob,"vsbType",EnabledAtPlay)){strlist<<"rtb$";}
        if(getstr==myobj::enumstr(mob,"vsbType",EnabledAtResponse)){strlist<<"pattern$"<<"rtb$";}
        if(getstr==myobj::enumstr(mob,"vsbType",PlayerFilter)){strlist<<"rtb$"<<"targets$";}
        if(getstr==myobj::enumstr(mob,"vsbType",SkillCardUse)){strlist<<"rtb$"<<"targets$"<<"room$";}
        return strlist;
    }

    int vsn;
    QString objname_viewas;
    int tgtn;
    QString getCardsNumProperty();
    void setCardsNumProperty(QString getstr);
    QString getTargetPlayersNumProperty();
    void setTargetPlayersNumProperty(QString getstr);
    QString getCardViewAsProperty();
    void setCardViewAsProperty(QString getstr);
    QString getCardViewAsPropertyRemark();
    void setCardViewAsPropertyRemark(QString getstr);
    myblock *iniBlock(QString getstr);
    QString findRemarkByName_event(QString getname){
        const QMetaObject *mob=metaObject();
        if(getname==myobj::enumstr(mob,"vsbType",ViewFilter)){
            return tr("We use this function to check which card can be selected. ")
                    +tr("Return true means the card to select is selectable. ")
                    +tr("For example, we can only choose a red card when we use Wusheng; ")
                    +tr("we can only choose a spade handcard when we use Luanji and select a spade handcard before.");
        }
        if(getname==myobj::enumstr(mob,"vsbType",EnabledAtPlay)){
            return tr("We use this function to check if the skill can be used in Phase Play. ")
                    +tr("Return true means the skill is usable. ")
                    +tr("For example, before we use Fanjian, we can use it in Phase Play. ");
        }
        if(getname==myobj::enumstr(mob,"vsbType",EnabledAtResponse)){
            return tr("We use this function to check if the skill can be used to respond. ")
                    +tr("Return true means the skill is usable. ")
                    +tr("For example, we can use Wusheng when we want to respond a slash. ");
        }
        if(getname==myobj::enumstr(mob,"vsbType",PlayerFilter)){
            return tr("We use this function to check which player can be selected by the skillcard. ")
                    +tr("Return true means the player to select is selectable. ")
                    +tr("Warning: it is only valid when you view it as a skillcard. ")
                    +tr("For example, we can only choose a wounded Male when we use Jieyin; ")
                    +tr("we cannot choose two players with the handcard disparity over our total cards. ");
        }
        if(getname==myobj::enumstr(mob,"vsbType",SkillCardUse)){
            return tr("We use this function to define the behavior of the skillcard. ");
        }
        return getname;
    }
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
    QString findRemarkByName_event(QString getname){
        if(getname==myobj::enumstr(metaObject(),"dtsbType",CorrectFunc)){
            return tr("We use this function to calculate the additional value of distance. ")
                    +tr("Return value is the additional value beyond the geographical distance. ")
                    +tr("For example, with Mashu, when we calculate the distance from us to others, the additional value is -1. ");
        }
        return getname;
    }
    QStringList funtaglist(QString getstr){
        QStringList strlist;
        if(getstr==myobj::enumstr(metaObject(),"dtsbType",CorrectFunc)){strlist<<"rtn$";}
        return strlist;
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
    QString findRemarkByName_event(QString getname){
        if(getname==myobj::enumstr(metaObject(),"ftsbType",ViewFilter)){
            return tr("We use this function to check which card should be filtered. ")
                    +tr("Return true means the card should be filtered. ")
                    +tr("For example, with Hongyan, all spade cards in our places should be filtered as heart cards. ");
        }
        return getname;
    }
    QStringList funtaglist(QString getstr){
        QStringList strlist;
        if(getstr==myobj::enumstr(metaObject(),"ftsbType",ViewFilter)){strlist<<"room$"<<"rtb$";}
        return strlist;
    }
    QStringList trans();
};
Q_DECLARE_METATYPE(myfts)

class myprs:public mysk
{
    Q_OBJECT
    Q_ENUMS(prsbType)
    Q_ENUMS(prsbAbb)
public:
    explicit myprs(QObject *parent=0):mysk(parent){}
    int getType(){return ProhibitSkill;}
    enum prsbType{IsProhibited=1};
    enum prsbAbb{prs_ip=IsProhibited};
    QString findRemarkByName_event(QString getname){
        if(getname==myobj::enumstr(metaObject(),"prsbType",IsProhibited)){
            return tr("We use this function to check when the target should be prohibited. ")
                    +tr("Return true means the target should be prohibited. ")
                    +tr("For example, with Kongcheng, when the user use Duel, we cannot be chosen as the target. ");
        }
        return getname;
    }
    QStringList funtaglist(QString getstr){
        QStringList strlist;
        if(getstr==myobj::enumstr(metaObject(),"prsbType",IsProhibited)){strlist<<"rtb$";}
        return strlist;
    }
    QStringList trans();
};
Q_DECLARE_METATYPE(myprs)

class mymcs:public mysk
{
    Q_OBJECT
    Q_ENUMS(mcsbType)
    Q_ENUMS(mcsbAbb)
public:
    explicit mymcs(QObject *parent=0):mysk(parent){}
    int getType(){return MaxCardsSkill;}
    enum mcsbType{ExtraFunc=1};
    enum mcsbAbb{mcs_ef=ExtraFunc};
    QString findRemarkByName_event(QString getname){
        if(getname==myobj::enumstr(metaObject(),"mcsbType",ExtraFunc)){
            return tr("We use this function to calculate the additional number of maxcards. ")
                    +tr("Return value is the additional number beyond the general maxcards. ")
                    +tr("For example, with Xueyi, our additional number of maxcards is 2*X, X is the number of Qun except us. ");
        }
        return getname;
    }
    QStringList funtaglist(QString getstr){
        QStringList strlist;
        if(getstr==myobj::enumstr(metaObject(),"mcsbType",ExtraFunc)){strlist<<"rtn$";}
        return strlist;
    }
    QStringList trans();
};
Q_DECLARE_METATYPE(mymcs)

class mytms:public mysk
{
    Q_OBJECT
    Q_ENUMS(tmsbType)
    Q_ENUMS(tmsbAbb)
    Q_ENUMS(tmsProperty)
    Q_PROPERTY(QString PatternProperty READ getPatternProperty WRITE setPatternProperty)
    Q_PROPERTY(QString PatternPropertyRemark READ getPatternPropertyRemark WRITE setPatternPropertyRemark)
public:
    explicit mytms(QObject *parent=0):mysk(parent){pattern="Slash";}
    int getType(){return TargetModSkill;}
    enum tmsbType{ExtraTarget=0,DistanceLimit=1,Residue=2};
    enum tmsbAbb{tms_et=ExtraTarget,tms_dl=DistanceLimit,tms_r=Residue};
    enum tmsProperty{Pattern=11};
    QString pattern;
    QString getPatternProperty();
    void setPatternProperty(QString getstr);
    QString getPatternPropertyRemark();
    void setPatternPropertyRemark(QString getstr);
    QString findRemarkByName_event(QString getname){
        const QMetaObject *mob=metaObject();
        if(getname==myobj::enumstr(mob,"tmsbType",ExtraTarget)){
            return tr("We use this function to calculate the additional number of targets. ")
                    +tr("Return value is the additional number. ")
                    +tr("For example, with Tianyi successing, the additional number of targets of Slash is 1. ");
        }
        if(getname==myobj::enumstr(mob,"tmsbType",DistanceLimit)){
            return tr("We use this function to calculate the additional distance beyond the limitation. ")
                    +tr("Return value is the additional distance beyond the limitation. ")
                    +tr("For example, with Duanliang, the additional distance for SupplyShortage is 1. ");
        }
        if(getname==myobj::enumstr(mob,"tmsbType",Residue)){
            return tr("We use this function to calculate the additional times for use beyond the limitation. ")
                    +tr("Return value is the additional times for use beyond the limitation. ")
                    +tr("For example, with Tianyi successing, the additional times for use Slash is 1. ");
        }
        return getname;
    }
    QStringList funtaglist(QString getstr){
        const QMetaObject *mob=metaObject();
        QStringList strlist;
        if(getstr==myobj::enumstr(mob,"tmsbType",ExtraTarget)){strlist<<"rtn$";}
        if(getstr==myobj::enumstr(mob,"tmsbType",DistanceLimit)){strlist<<"rtn$";}
        if(getstr==myobj::enumstr(mob,"tmsbType",Residue)){strlist<<"rtn$";}
        return strlist;
    }
    QStringList trans();
};
Q_DECLARE_METATYPE(mytms)

class myexs:public mysk
{
    Q_OBJECT
    Q_ENUMS(exsProperty)
    Q_PROPERTY(QString SKNameProperty READ getSKNameProperty WRITE setSKNameProperty)
    Q_PROPERTY(QString SKNamePropertyRemark READ getSKNamePropertyRemark WRITE setSKNamePropertyRemark)
public:
    explicit myexs(QObject *parent=0):mysk(parent){
        tgtsk=NULL;
        skname=myobj::myconstskstrlist.first().split("|").first();
    }
    int getType(){return ExistingSkill;}
    enum exsProperty{SKName=11};
    QString skname;
    mysk *tgtsk;
    QString getskname();
    void setskname(QString getstr);
    QString getSKNameProperty();
    void setSKNameProperty(QString getstr);
    QString getSKNamePropertyRemark();
    void setSKNamePropertyRemark(QString getstr);
    QStringList propertystrlist(bool b4tab=false){
        const QMetaObject *mob=metaObject();
        QStringList strlist;
        strlist<<myobj::enumstr(mob,"skProperty",Name);
        strlist<<myobj::enumstr(mob,"skProperty",Owner);
        if(b4tab){}
        strlist<<myobj::enumstrlist(mob,"exsProperty");
        return strlist;
    }
    QString propertystr_get();
    void propertystr_set(QString getstr);
    QString findRemarkByName_event(QString getname){return getname;}
    QStringList trans(){return QStringList();}
};
Q_DECLARE_METATYPE(myexs)

#endif // MYTRS_H
