#ifndef MYINPUTITEM_H
#define MYINPUTITEM_H

#include <QObject>
#include "mysys.h"

class myinputitem : public QObject
{
    Q_OBJECT
public:
    explicit myinputitem(QObject *parent = 0):QObject(parent){
        pf=NULL;
        visible=true;
    }

    ~myinputitem(){
        foreach(myinputitem *ip,pchlist){delete ip;}
    }
    myinputitem *pf;
    QList<myinputitem *> pchlist;
    QVariant mydata;
    //enum mytype_stc{stc_Start=0,stc_Event=1,stc_Block=2,stc_Fun=3,stc_Obj=4};
    //enum mytype_condition{condition_Start=0,condition_Event=1,condition_Block=2,condition_Obj0=3,condition_Obj=4};
    //enum mytype_foreach{foreach_Start=0,foreach_Event=1,foreach_Block=2,foreach_Obj=3};
    enum mytype_func{func_Start=0,func_Event=1,func_Block=2,func_Fun=3,func_Obj=4};
    int type;
    bool visible;

    int getcnt(){return 1;}
    myinputitem *newChild(QVariant getdata,bool extra=false,bool front=false);
    myinputitem *getChild(int i,bool visibility=false);
    int getChildren_cnt(bool visibility=false);
    int getRank(bool visibility=false);
    myinputitem *getParent(int gettype,bool self=false);
    myinputitem *getParent(QString str, bool self=false);
    bool sameValue(myinputitem *getp){return (type==getp->type)&&(mydata==getp->mydata);}
    myinputitem *search(myinputitem *getp);
    QString getstr(){return mydata.toString();}
    static bool spstr(QString getstr){return getstr.startsWith(">")||(getstr==str4input());}
    static QString str4input(){return tr("input others");}
    void delAllChildren(){
        foreach(myinputitem *ip,pchlist){delete ip;}
        pchlist.clear();
    }
    static void mycpy(myinputitem *root0,myinputitem *root1,bool visibility=false);

    void myshow(){
        qWarning()<<getstr()<<type;
        foreach(myinputitem *ip,pchlist){ip->myshow();}
    }
signals:
    
public slots:
    
};

#endif // MYINPUTITEM_H
