#ifndef MYSYS_H
#define MYSYS_H

#include <QObject>
#include "mygeneral.h"


class mysys : public QObject
{
    Q_OBJECT
    Q_ENUMS(QSVersion)
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
    enum QSVersion{VersionUnknown=0,V0224=1,V0610=2};
    int qsv;

    explicit mysys(QObject *parent = 0):QObject(parent){
        packagename="mypackage";psk0=NULL;pg0=NULL;qsv=VersionUnknown;
    }
    void myini(QString getpath,int getqsv);
    //void myini_design(QString path);
    static QString qsv2str(int getqsv){return myobj::enumstr(&staticMetaObject,"QSVersion",getqsv);}
    static int str2qsv(QString getstr){return myobj::enumint(&staticMetaObject,"QSVersion",getstr,VersionUnknown);}
    static QStringList qsvstrlist(){return myobj::enumstrlist(&staticMetaObject,"QSVersion");}
    void getsklist(QList<mysk *> &rsklist,int gettype);
    void getsklist_noexs(QList<mysk *> &rsklist);
    void getavlobjlist_global(QList<myobj *> &list);    
    mygeneral *newGeneral(QString getname,int getkingdom=mygeneral::god,int gethp=4,bool getsex=true);
    mysk *newSkill(QString getname,int gettype);
    mysk *newsk(int gettype);
    //mytrs *newTrs(QString getname);
    //myvs *newVs(QString getname);
    bool delGeneral(mygeneral *getp);
    bool delSkill(mysk *getp);
    mygeneral *findGeneralByName(QString getname);
    mysk *findSkillByName(QString getname);
    myfunction *findFuncByRTObj(myobj *);
    mydo *findDoByTag(QString dotag);
    QStringList trans();
    QStringList trans4design();

    void undo();
    void redo();
    void myrfr_dotag();

    QStringList getgstrlist(){
        QStringList strlist;
        foreach(mygeneral *ip,glist){strlist<<ip->name;}
        return strlist;
    }
    QStringList getskstrlist(int gettype){
        QStringList strlist;
        foreach(mysk *ip,sklist){
            if(ip->getType()==gettype){strlist<<ip->name;}
        }
        return strlist;
    }
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
