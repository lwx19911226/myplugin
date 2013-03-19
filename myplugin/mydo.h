#ifndef MYDO_H
#define MYDO_H

#include <QObject>
#include "myobj.h"
#include "mycode.h"

class mytrs;
class mydo : public QObject
{
    Q_OBJECT
public:
    explicit mydo(QObject *parent = 0):QObject(parent){}
    //enum doType{Sentence=1,Condition=2,Foreach=3};
    QList<myobj *> objlist;
    QList<myblock *> blocklist;
    //int type;

    QString trans();
    QString trans4block(QString getblockstr);
    QString trans4obj(myobj *getp);
    QString getBlockName(int index);
    myobj *getObj(int index);
    void getprevlist(QList<mydo *> &list);
    static void dotrans(mytrs *psk0,QString gettrans);
    /*
    static QString type2trans(int gettype){
        switch(gettype){
        case Sentence:return "s";
        case Condition:return "c";
        case Foreach:return "f";
        default:return "";
        }
    }
    static QString type2str(int gettype){
        switch(gettype){
        case Sentence:return "sentence";
        case Condition:return "condition";
        case Foreach:return "foreach";
        default:return "";
        }
    }
    static int str2type(QString getstr){
        if(getstr==type2str(Sentence)){return Sentence;}
        if(getstr==type2str(Condition)){return Condition;}
        if(getstr==type2str(Foreach)){return Foreach;}
        return 0;
    }
    static QStringList typestrlist(){
        QStringList strlist;
        strlist<<type2str(Sentence)<<type2str(Condition)<<type2str(Foreach);
        return strlist;
    }
*/

    
signals:
    
public slots:
    
};

#endif // MYDO_H
