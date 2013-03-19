#ifndef MYSEG_H
#define MYSEG_H

#include <QObject>
//#include <QList>
#include "myopr.h"

class myseg : public QObject
{
    Q_OBJECT
public:
    explicit myseg(QObject *parent = 0);
    
signals:
    
public slots:
    
};

class myseg_event:public myseg
{
public:
    QList<myopr *> oprlist;
    void addOpr(myopr *,QString);
    //void addCondition(QString geteventstr,QString getblockstr,QList<myobj *> &getobjlist,QString getrm);
    void addCondition(mycondition *getp);
    QString trans();
    void need4block(QString geteventstr,QList<QString> &);
    myblock *findBlockByName(QString getname);
    bool removeCode(mycode *getp);
    bool insertCode(mycode *getp);
};

#endif // MYSEG_H
