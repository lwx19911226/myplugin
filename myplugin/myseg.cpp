#include "myseg.h"

myseg::myseg(QObject *parent) :
    QObject(parent)
{
}

void myseg_event::addOpr(myopr *getp,QString getstr){
    QTextStream cout(stdout,QIODevice::WriteOnly);
    QTextStream cin(stdin,QIODevice::ReadOnly);
    foreach(myopr *ip,oprlist){
        if(ip->eventstr==getp->eventstr){
            ip->addBlock(getp->blockp,getstr);
            return;
        }
    }
    oprlist.append(getp);
}
/*
void myseg_event::addCondition(QString geteventstr,QString getblockstr,QList<myobj *> &getobjlist,QString getrm){
    mycondition *p=new mycondition;
    p->name=getblockstr+","+getobjlist.first()->name;
    p->tgtobj=getobjlist.first();
    getobjlist.pop_front();
    p->caseobjlist.append(getobjlist);
    p->remark=getrm;
    p->myini();
    foreach(myopr *ip,oprlist){
        if(ip->same(geteventstr)){
            ip->addBlock(p,getblockstr);
            return;
        }
    }
    myopr *p_opr=new myopr;
    p_opr->setEvent(geteventstr);
    p_opr->setBlock(NULL);
    p_opr->addBlock(p,getblockstr);
    oprlist.append(p_opr);
}
*/
void myseg_event::addCondition(mycondition *getp){
    if(getp->inilist.length()<2){return;}
    QString geteventstr=getp->inilist.at(0).toString();
    QString getblockstr=getp->inilist.at(1).toString();
    foreach(myopr *ip,oprlist){
        if(ip->eventstr==geteventstr){
            ip->addBlock(getp,getblockstr);
            return;
        }
    }
    myopr *p_opr=new myopr;
    p_opr->eventstr=geteventstr;
    p_opr->setBlock(NULL);
    p_opr->addBlock(getp,getblockstr);
    oprlist.append(p_opr);
}

QString myseg_event::trans(){
    QString str="";
    foreach(myopr *ip,oprlist){
        str+=ip->trans();
    }
    return str;
}

void myseg_event::need4block(QString geteventstr, QList<QString> &list){
    foreach(myopr *ip,oprlist){
        if(ip->eventstr==geteventstr){
            ip->need4block(list);
        }
    }
}

myblock *myseg_event::findBlockByName(QString getname){
    myblock *pblock;
    foreach(myopr *ip,oprlist){
        pblock=ip->findBlockByName(getname);
        if(pblock){return pblock;}
    }
    return NULL;
}

bool myseg_event::removeCode(mycode *getp){
    foreach(myopr *ip,oprlist){
        if(ip->removeCode(getp)){return true;}
    }
    return false;
}
bool myseg_event::insertCode(mycode *getp){
    foreach(myopr *ip,oprlist){
        if(ip->insertCode(getp)){return true;}
    }
    return false;
}
