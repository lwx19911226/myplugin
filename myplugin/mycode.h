#ifndef MYCODE_H
#define MYCODE_H

#include <QObject>
#include <QMap>
#include <QTreeWidgetItem>
#include "myfun.h"
#include "myevent.h"

class mycode : public QObject
{
    Q_OBJECT
public:
    explicit mycode(QObject *parent = 0):QObject(parent){}
    //virtual int getType(){return Code;}
    static QStringList mymdf(QStringList getlist,QString getstr,bool front=true){
        QString getstr_front=(front?getstr:"");
        QString getstr_back=(front?"":getstr);
        return mymdf(getlist,getstr_front,getstr_back);
    }
    static QStringList mymdf(QStringList getlist, QString getstr_front, QString getstr_back){
        return getlist.replaceInStrings(QRegExp("^(.*)$"),getstr_front+"\\1"+getstr_back);        
    }
    static QStringList myindent(QStringList getlist){
        return mymdf(getlist,QString("    "));
    }
    static QStringList mysplit(QString getstr,bool b4r=false){
        QString tstr=getstr;
        tstr.replace("\\|","\\\\");
        QString rstr="|";
        if(b4r){rstr="\\|";}
        return tstr.split("|").replaceInStrings("\\\\",rstr);
    }

signals:
    
public slots:
    
};
class mysk;
class myfunction;
class myblock : public QObject
{
    Q_OBJECT
    Q_ENUMS(blockType)
public:
    explicit myblock(QObject *parent = 0):QObject(parent){upperLayer=NULL;}
    enum blockType{Block=1,Function=2};
    virtual int getType(){return Block;}
    static int globalint;
    QList<myblock *> blocklist;
    myblock *upperLayer;
    QString name;
    QString remark;
    //QList<QVariant> inilist;
    void addBlock(myblock *);
    virtual void addBlock(myblock *,QString);
    //virtual void addStc(myfunction *);
    bool matchName(QString getstr);
    virtual QStringList trans();
    virtual QStringList need4block();
    myblock *findBlockByName(QString getname);
    virtual myfunction *findFuncByObj(myobj *);
    bool removeBlock(myblock *getp);
    //virtual bool insertBlock(myblock *getp);
    QTreeWidgetItem *mytreeitem();
    int getLayer();
    QString getEvent();
    virtual QString getRemark();
    myblock *getTopBlock();
    mysk *getsk0();
    int getqsv0();
    virtual void myshow(){qWarning()<<name<<getLayer()<<blocklist.length();for(int i=0;i<blocklist.length();i++){blocklist.at(i)->myshow();}}
};

class myfunction : public myblock
{
    Q_OBJECT
public:
    explicit myfunction(QObject *parent = 0):myblock(parent){}
    int getType(){return Function;}
    QString funname;
    QList<myobj *> objlist;
    QList<myobj *> rtobjlist;
    QList<int> vrlist;
    QStringList trans();
    void myini(QString geteventstr,QString getblockstr,QStringList &rtrmlist,QStringList &blrmlist);
    QStringList need4block();
    void addBlock(myblock *, QString);
    myfunction *findFuncByObj(myobj *);
    QString getRemark();
    static QString str4parameter(){return tr("Parameter");}
    static QString str4returnvalue(){return tr("Return value");}
    void myshow(){qWarning()<<funname;}
};
/*
class myopr : public myblock
{
    Q_OBJECT
public:
    explicit myopr(QObject *parent = 0):myblock(parent){

    }
    int getType(){return Opr;}
    QString eventstr;
    void myini();    
    QStringList trans();
signals:

public slots:

};
*/
/*
class mycondition:public myblock
{
    Q_OBJECT
public:
    explicit mycondition(QObject *parent = 0):myblock(parent){}
    int getType(){return Condition;}
    myobj *tgtobj;
    QList<myobj *> caseobjlist;
    QMap<myobj *,myblock *> map;

    void myini();
    //void addStc(myfunction *,myobj *);
    void addBlock(myblock *, QString);
    QStringList trans();
    //bool matchName(QString getstr);
    QStringList need4block();
};
class myforeach:public myblock
{
    Q_OBJECT
public:
    explicit myforeach(QObject *parent = 0):myblock(parent){iobj=NULL;}
    int getType(){return Foreach;}
    myobj *tgtobj;
    myobj *iobj;

    void myini();
    QStringList trans();
    QStringList need4block();
    //bool matchName(QString getstr);
    void addBlock(myblock * getp, QString getstr);
};
*/



#endif // MYCODE_H
