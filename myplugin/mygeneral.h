#ifndef MYGENERAL_H
#define MYGENERAL_H

#include <QObject>
#include "mysk.h"
class mysys;
class mygeneral : public QObject
{
    Q_OBJECT
    Q_ENUMS(generalProperty)
    Q_ENUMS(kingdomType)
public:
    explicit mygeneral(QObject *parent = 0):QObject(parent){sex=true;}
    static QString tabstr(){return tr("General");}
    enum generalProperty{Name=0,Translation=1,Kingdom=2,Sex=3,HP=4,Title=5,Word=6,CV=7};
    static QString property2str(int getproperty){return myobj::enumstr(&staticMetaObject,"generalProperty",getproperty);}
    static int str2property(QString getstr){return myobj::enumint(&staticMetaObject,"generalProperty",getstr);}
    static QStringList propertystrlist(){return myobj::enumstrlist(&staticMetaObject,"generalProperty");}
    static QString property2prefix(int getproperty){
        switch(getproperty){
        case Title:return "#";
        case Word:return "~";
        case CV:return "cv:";
        default:qWarning()<<"property2prefix"<<getproperty;return QString();
        }
    }
    static QString nullname(){return tr("NULL");}
    QString name;
    enum kingdomType{god=0,wei=1,shu=2,wu=3,qun=4};
    int kingdom;
    int hp;
    bool sex;
    QString translation;
    QString title;
    QString word;
    QString cv;
    QList<mysk *> sklist;
    static int globalint;
    static QString kingdom2str(int getkingdom){return myobj::enumstr(&staticMetaObject,"kingdomType",getkingdom);}
    static QStringList kingdomstrlist(){return myobj::enumstrlist(&staticMetaObject,"kingdomType");}
    static int str2kingdom(QString getstr){return myobj::enumint(&staticMetaObject,"kingdomType",getstr,god);}
    static QString sex2str(bool getsex){
        return (getsex?"m":"f");
    }
    static bool str2sex(QString getstr){
        if(getstr==sex2str(true)){return true;}
        if(getstr==sex2str(false)){return false;}
        return true;
    }
    static QStringList sexstrlist(){
        return QStringList()<<sex2str(true)<<sex2str(false);
    }
    static QString sex2trans(bool getsex){
        return getsex?"true":"false";
    }
    QString trans(){
        return QString("%1=sgs.General(extension,\"%1\",\"%2\",%3,%4)").arg(name,kingdom2str(kingdom)).arg(hp).arg(sex2trans(sex));
    }
    mysys *getsys();
    void setName(QString getname);
    void setDefaultName();
    void propertymap_get(QMap<QString,QString> &strmap,QMap<QString,QStringList> &strlistmap,bool b4remark);
    void propertymap_set(QMap<QString,QString> &strmap,bool b4remark);
    QString propertystr_get();
    void propertystr_set(QString getstr);
signals:
    
public slots:
    
};

#endif // MYGENERAL_H
