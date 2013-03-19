#ifndef MYGENERAL_H
#define MYGENERAL_H

#include <QObject>
#include "mysk.h"
class mysys;
class mygeneral : public QObject
{
    Q_OBJECT
public:
    explicit mygeneral(QObject *parent = 0):QObject(parent){sex=true;}
    static QString tabstr(){return tr("General");}
    enum generalProperty{Name=0,Translation=1,Kingdom=2,Sex=3,HP=4,Title=5,Word=6,CV=7};
    static QString property2str(int getproperty){
        switch(getproperty){
        case Name:return tr("Name");
        case Translation:return tr("Translation");
        case Kingdom:return tr("Kingdom");
        case Sex:return tr("Sex");
        case HP:return tr("HP");
        case Title:return tr("Title");
        case Word:return tr("Word");
        case CV:return tr("CV");
        default:qWarning()<<"property2str:"<<getproperty;return QString();
        }
    }
    static int str2property(QString getstr){
        if(getstr==property2str(Name)){return Name;}
        if(getstr==property2str(Translation)){return Translation;}
        if(getstr==property2str(Kingdom)){return Kingdom;}
        if(getstr==property2str(Sex)){return Sex;}
        if(getstr==property2str(HP)){return HP;}
        if(getstr==property2str(Title)){return Title;}
        if(getstr==property2str(Word)){return Word;}
        if(getstr==property2str(CV)){return CV;}
        qWarning()<<"str2property:"<<getstr;
        return 0;
    }
    static QStringList propertystrlist(){
        QStringList strlist;
        strlist<<property2str(Name)<<property2str(Translation)
              <<property2str(Kingdom)<<property2str(Sex)<<property2str(HP)
                <<property2str(Title)<<property2str(Word)<<property2str(CV);
        return strlist;
    }
    QString name;
    enum kingdomType{kingdom_Default=0,kingdom_Wei=1,kingdom_Shu=2,kingdom_Wu=3,kingdom_Qun=4,kingdom_God=5};
    int kingdom;
    int hp;
    bool sex;
    QString translation;
    QString title;
    QString word;
    QString cv;
    QList<mysk *> sklist;
    static int globalint;
    static QString kingdom2str(int getkingdom){
        switch(getkingdom){
        case kingdom_Wei:return "wei";
        case kingdom_Shu:return "shu";
        case kingdom_Wu:return "wu";
        case kingdom_Qun:return "qun";
        case kingdom_God:return "god";
        case kingdom_Default:return "god";
        default:return "god";
        }
    }
    static QStringList kingdomstrlist(){
        QStringList strlist;
        strlist<<kingdom2str(kingdom_Wei)<<kingdom2str(kingdom_Shu)<<kingdom2str(kingdom_Wu)<<kingdom2str(kingdom_Qun)<<kingdom2str(kingdom_God);
        return strlist;
    }
    static int str2kingdom(QString getstr){
        if(getstr==kingdom2str(kingdom_Wei)){return kingdom_Wei;}
        if(getstr==kingdom2str(kingdom_Shu)){return kingdom_Shu;}
        if(getstr==kingdom2str(kingdom_Wu)){return kingdom_Wu;}
        if(getstr==kingdom2str(kingdom_Qun)){return kingdom_Qun;}
        if(getstr==kingdom2str(kingdom_God)){return kingdom_God;}
        return kingdom_God;
    }
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
    void propertymap_get(QMap<QString,QString> &strmap,QMap<QString,QStringList> &strlistmap);
    void propertymap_set(QMap<QString,QString> &strmap);
signals:
    
public slots:
    
};

#endif // MYGENERAL_H
