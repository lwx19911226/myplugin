#ifndef MYSYS_H
#define MYSYS_H

#include <QObject>
#include "mygeneral.h"


class mysys : public QObject
{
    Q_OBJECT
public:
    QList<mysk *> sklist;
    QList<mysk *> sklist_r;
    mysk *psk0;    
    QString packagename;
    QString package_trans;
    QList<mygeneral *> glist;
    QList<mygeneral *> glist_r;
    mygeneral *pg0;
    QList<mydo *> dolist;
    QStringList undostrlist;
    QList<mydo *> dolist_r;
    static int globalint;

    explicit mysys(QObject *parent = 0):QObject(parent){packagename="mypackage";psk0=NULL;pg0=NULL;}
    void myini(QString getpath="");

    void getsklist(QList<mysk *> &rsklist,int gettype);
    void getavlobjlist_global(QList<myobj *> &list);    
    mygeneral *newGeneral(QString getname,int getkingdom=mygeneral::god,int gethp=4,bool getsex=true);
    mysk *newSkill(QString getname,int gettype);
    mytrs *newTrs(QString getname);
    myvs *newVs(QString getname);
    bool delGeneral(mygeneral *getp);
    bool delSkill(mysk *getp);
    mygeneral *findGeneralByName(QString getname);
    mysk *findSkillByName(QString getname);
    QStringList trans();
    QStringList trans4design();
    void myini_design(QString path);
    void undo();
    void redo();

    QStringList getgstrlist(){QStringList strlist;foreach(mygeneral *ip,glist){strlist<<ip->name;}return strlist;}
    QStringList getskstrlist(){QStringList strlist;foreach(mysk *ip,sklist){strlist<<ip->name;}return strlist;}
    void sig_update(){
        //if(!psk0){return;}
        emit update();
    }
    //void myrun();
    //void need4fun(QString funstr,QList<QString> &);
    //QString need4fun(QString funstr,int k);
    //void get4fun(QString funstr,QList<QString> &);
    //QString get4fun(QString funstr,int k);
    //int need4fun_cnt(QString funstr);
    //int get4fun_cnt(QString funstr);
    //void need4block(QString geteventstr,QList<QString> &);
    //void input_opr(QString &geteventstr,QString &getblockstr,QString &getfunstr,QList<myobj *> &getobjlist,QList<QString> &getrtrmlist);
    //void input_condition(QString &geteventstr,QString &getblockstr,QList<myobj *> &getobjlist,QString &getrm);
    //void input_event(QString &geteventstr);
    //void input_block(QString geteventstr,QString &getblockstr);
    //void input_fun(QString &getfunstr);
    //myobj *input_obj(QString gettype);
signals:
    void update();
public slots:

};

#endif // MYSYS_H
