#ifndef MYDO_H
#define MYDO_H

#include <QObject>
#include "myobj.h"
#include "mycode.h"

class mysys;
class mysk;
class mytrs;
class mydo : public QObject
{
    Q_OBJECT
public:
    explicit mydo(QObject *parent = 0):QObject(parent){psktgt=NULL;}
    //enum doType{Sentence=1,Condition=2,Foreach=3};
    mysk *psktgt;
    QList<myobj *> objlist;
    QList<myblock *> blocklist;
    //int type;

    QString trans();
    QString trans4block(QString getblockstr);
    QString trans4obj(myobj *getp);
    QString getBlockName(int index);
    myobj *getObj(int index);
    void getprevlist(QList<mydo *> &list);
    mysys *getsys();
    //static void dotrans(mysk *psk0,QString gettrans);
signals:
    
public slots:
    
};

#endif // MYDO_H
