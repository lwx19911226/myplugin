#ifndef MYOPR_H
#define MYOPR_H

#include <QObject>
#include "mycode.h"


class myopr : public QObject
{
    Q_OBJECT
public:
    explicit myopr(QObject *parent = 0);
    QString eventstr;
    myblock *blockp;    
    void addBlock(myblock *);
    void addBlock(myblock *,QString);    
    void myini();
    QStringList need4block();
    QStringList trans();
    myblock *findBlockByName(QString getname);
    myfunction *findFuncByObj(myobj *);
    bool removeBlock(myblock *getp);
    bool insertBlock(myblock *getp);

    //void setEvent(QString geteventstr);
    //void setBlock(myblock *);
signals:
    
public slots:
    
};

#endif // MYOPR_H
