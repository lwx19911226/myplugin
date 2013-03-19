#include "mydo.h"
#include "mysk.h"
QString mydo::trans4block(QString getblockstr){
    QList<mydo *> prevlist;
    getprevlist(prevlist);
    if(myevent::isEvent(getblockstr)){return getblockstr;}
    else{
        for(int i=0;i<prevlist.length();i++){
            myfunction *pt=static_cast<myfunction *>(prevlist.at(i)->blocklist.first());
            QStringList tstrlist;
            tstrlist=pt->need4block();
            if(tstrlist.contains(getblockstr)){
                return QString("%1->%2").arg(i).arg(tstrlist.indexOf(getblockstr));
            }
            /*
            if(prevlist.at(i)->type==Sentence){
                myfunction *pt=static_cast<myfunction *>(prevlist.at(i)->blocklist.first());
                QStringList tstrlist;
                tstrlist=pt->need4block();
                if(tstrlist.contains(getblockstr)){
                    return QString("%1->%2").arg(i).arg(tstrlist.indexOf(getblockstr));
                }
            }
            if(prevlist.at(i)->type==Condition){
                mycondition *pt=static_cast<mycondition *>(prevlist.at(i)->blocklist.first());
                QStringList tstrlist;
                tstrlist=pt->need4block();
                if(tstrlist.contains(getblockstr)){
                    //i
                    //tstrlist.indexOf(getblockstr)
                    return QString("%1->%2").arg(i).arg(tstrlist.indexOf(getblockstr));
                }
            }
            else if(prevlist.at(i)->type==Foreach){
                myforeach *pt=static_cast<myforeach *>(prevlist.at(i)->blocklist.first());
                QStringList tstrlist;
                tstrlist=pt->need4block();
                if(tstrlist.contains(getblockstr)){
                    return QString("%1->%2").arg(i).arg(tstrlist.indexOf(getblockstr));
                }
            }
            */
        }
    }
    return QString();
}
QString mydo::trans4obj(myobj *getp){
    QList<mydo *> prevlist;
    getprevlist(prevlist);
    for(int i=0;i<prevlist.length();i++){
        if(prevlist.at(i)->objlist.contains(getp)){
            //i
            //prevlist.at(i)->objlist.indexOf(getp)
            return QString("%1->%2").arg(i).arg(prevlist.at(i)->objlist.indexOf(getp));
        }
    }
    return getp->name;
    /*
    if(!getp->isDynamic){return getp->name;}
    else{
        for(int i=0;i<prevlist.length();i++){
            if(prevlist.at(i)->objlist.contains(getp)){
                //i
                //prevlist.at(i)->objlist.indexOf(getp)
                return QString("%1->%2").arg(i).arg(prevlist.at(i)->objlist.indexOf(getp));
            }            
        }
    }
    */
    qWarning()<<"trans4obj:"<<getp->name;
    return QString();
}

QString mydo::trans(){
    if(blocklist.isEmpty()){qWarning()<<"mydo trans";return "";}
    QString str="";
    QStringList strlist;
    strlist.clear();
    myfunction *pstc=static_cast<myfunction *>(blocklist.first());
    strlist<<pstc->inilist.first().toString();
    QString getblockstr=pstc->inilist.at(1).toString();
    strlist<<trans4block(getblockstr);
    strlist<<pstc->funname;
    QStringList tstrlist;
    tstrlist.clear();
    foreach(myobj *ip,pstc->objlist){
        tstrlist<<trans4obj(ip);
    }
    strlist<<tstrlist.join(",");
    tstrlist.clear();
    foreach(myobj *ip,objlist){
        tstrlist<<ip->remark;
    }
    strlist<<tstrlist.join(",");
    tstrlist.clear();
    foreach(myblock *ip,pstc->blocklist){
        tstrlist<<ip->remark;
    }
    strlist<<tstrlist.join(",");
    strlist.replaceInStrings(QRegExp("\\|"),QString("\\|"));
    str+=strlist.join("|");
    return str;
    /*
    str+=type2trans(type)+":";
    if(type==Sentence){
        strlist.clear();
        myfunction *pstc=static_cast<myfunction *>(blocklist.first());
        strlist<<pstc->inilist.first().toString();
        QString getblockstr=pstc->inilist.at(1).toString();
        strlist<<trans4block(getblockstr);
        strlist<<pstc->funname;
        QStringList tstrlist;
        tstrlist.clear();
        foreach(myobj *ip,pstc->objlist){
            tstrlist<<trans4obj(ip);
        }
        strlist<<tstrlist.join(",");
        tstrlist.clear();
        foreach(myobj *ip,objlist){
            tstrlist<<ip->remark;
        }
        strlist<<tstrlist.join(",");
        tstrlist.clear();
        foreach(myblock *ip,pstc->blocklist){
            tstrlist<<ip->remark;
        }
        strlist<<tstrlist.join(",");
        strlist.replaceInStrings(QRegExp("\\|"),QString("\\|"));
        str+=strlist.join("|");
    }
    else if(type==Condition){
        strlist.clear();
        mycondition *pc=static_cast<mycondition *>(blocklist.first());
        strlist<<pc->inilist.first().toString();
        QString getblockstr=pc->inilist.at(1).toString();
        strlist<<trans4block(getblockstr);
        QStringList tstrlist;
        tstrlist.clear();
        tstrlist<<trans4obj(pc->tgtobj);
        foreach(myobj *ip,pc->caseobjlist){
            tstrlist<<trans4obj(ip);
        }
        strlist<<tstrlist.join(",");
        strlist<<pc->remark;
        strlist.replaceInStrings("|","\\|");
        str+=strlist.join("|");
    }
    else if(type==Foreach){
        strlist.clear();
        myforeach *pfch=static_cast<myforeach *>(blocklist.first());
        strlist<<pfch->inilist.first().toString();
        QString getblockstr=pfch->inilist.at(1).toString();
        strlist<<trans4block(getblockstr);
        strlist<<trans4obj(pfch->tgtobj);
        strlist<<pfch->remark;
        strlist.replaceInStrings("|","\\|");
        str+=strlist.join("|");
    }
    */
}
void mydo::getprevlist(QList<mydo *> &list){
    if(!list.isEmpty()){qWarning()<<"getprevlist";}
    mytrs *pf=static_cast<mytrs *>(parent());
    if(!pf->dolist.isEmpty()&&pf->dolist.contains(this)&&pf->dolist.first()!=this){
        list<<pf->dolist.mid(0,pf->dolist.indexOf(this));
    }
}
void mydo::dotrans(mytrs *psk0, QString gettrans){
    qWarning()<<gettrans;
    QString tstr=gettrans;
    tstr.replace("\\|","\\\\");
    QStringList strlist=tstr.split("|");
    strlist.replaceInStrings("\\\\","|");
    QString geteventstr=strlist.first();
    QString getblockstr;
    if(myevent::isEvent(strlist.at(1))){getblockstr=strlist.at(1);}
    else{
        int index1=strlist.at(1).split("->").first().toInt();
        int index2=strlist.at(1).split("->").last().toInt();
        getblockstr=psk0->dolist.at(index1)->getBlockName(index2);
    }
    QString getfunstr=strlist.at(2);
    QList<myobj *> getobjlist;
    QStringList tstrlist;
    tstrlist.clear();
    tstrlist=strlist.at(3).split(",");
    if(strlist.at(3)==""){tstrlist.clear();}
    for(int i=0;i<tstrlist.length();i++){
        if(tstrlist.at(i).contains("->")){
            int index1=tstrlist.at(i).split("->").first().toInt();
            int index2=tstrlist.at(i).split("->").last().toInt();
            getobjlist<<psk0->dolist.at(index1)->getObj(index2);
        }
        else{
            myobj *pobj=psk0->findObjByName(tstrlist.at(i),geteventstr);
            if(!pobj){
                pobj=new myobj(psk0->parent());
                pobj->name=tstrlist.at(i);
                if(myfun::need(getfunstr).at(i)==""){qWarning()<<"0227here1?";}
                pobj->type=myobj::str2type(myfun::need(getfunstr).at(i));
                //pobj->isDynamic=false;
            }
            getobjlist<<pobj;
        }
    }
    QStringList getrtrmlist;
    getrtrmlist<<strlist.at(4).split(",");
    QStringList getblrmlist;
    if(strlist.length()>=6){getblrmlist<<strlist.at(5).split(",");}
    psk0->addFunction(geteventstr,getblockstr,getfunstr,getobjlist,getrtrmlist,getblrmlist);
    /*
    if(gettrans.startsWith(type2trans(Sentence)+":")){
        QString tstr=gettrans.mid(type2trans(Sentence).length()+1);
        tstr.replace("\\|","\\\\");
        QStringList strlist=tstr.split("|");
        strlist.replaceInStrings("\\\\","|");
        QString geteventstr=strlist.first();
        QString getblockstr;
        if(myevent::isEvent(strlist.at(1))){getblockstr=strlist.at(1);}
        else{
            int index1=strlist.at(1).split("->").first().toInt();
            int index2=strlist.at(1).split("->").last().toInt();
            getblockstr=psk0->dolist.at(index1)->getBlockName(index2);
        }
        QString getfunstr=strlist.at(2);
        QList<myobj *> getobjlist;
        QStringList tstrlist;
        tstrlist.clear();
        tstrlist=strlist.at(3).split(",");
        if(strlist.at(3)==""){tstrlist.clear();}
        for(int i=0;i<tstrlist.length();i++){
            if(tstrlist.at(i).contains("->")){
                int index1=tstrlist.at(i).split("->").first().toInt();
                int index2=tstrlist.at(i).split("->").last().toInt();
                getobjlist<<psk0->dolist.at(index1)->getObj(index2);
            }
            else{
                myobj *pobj=psk0->findObjByName(tstrlist.at(i),geteventstr);
                if(!pobj){
                    pobj=new myobj(psk0->parent());
                    pobj->name=tstrlist.at(i);
                    if(myfun::need(getfunstr).at(i)==""){qWarning()<<"0227here1?";}
                    pobj->type=myobj::str2type(myfun::need(getfunstr).at(i));
                    pobj->isDynamic=false;
                }
                getobjlist<<pobj;
            }
        }
        QStringList getrtrmlist;
        getrtrmlist<<strlist.at(4).split(",");
        QStringList getblrmlist;
        if(strlist.length()>=6){getblrmlist<<strlist.at(5).split(",");}
        psk0->addOpr(geteventstr,getblockstr,getfunstr,getobjlist,getrtrmlist,getblrmlist);
        //qWarning()<<geteventstr<<getblockstr<<getfunstr<<getobjlist.length()<<getrtrmlist<<getblrmlist;

    }
    else if(gettrans.startsWith(type2trans(Condition)+":")){
        QString tstr=gettrans.mid(type2trans(Condition).length()+1);
        tstr.replace("\\|","\\\\");
        QStringList strlist=tstr.split("|");
        strlist.replaceInStrings("\\\\","|");
        QString geteventstr=strlist.first();
        QString getblockstr;
        if(myevent::isEvent(strlist.at(1))){getblockstr=strlist.at(1);}
        else{
            int index1=strlist.at(1).split("->").first().toInt();
            int index2=strlist.at(1).split("->").last().toInt();
            getblockstr=psk0->dolist.at(index1)->getBlockName(index2);
        }
        QList<myobj *> getobjlist;
        QStringList tstrlist;
        tstrlist.clear();
        tstrlist=strlist.at(2).split(",");
        for(int i=0;i<tstrlist.length();i++){
            if(tstrlist.at(i).contains("->")){
                int index1=tstrlist.at(i).split("->").first().toInt();
                int index2=tstrlist.at(i).split("->").last().toInt();
                getobjlist<<psk0->dolist.at(index1)->getObj(index2);
            }
            else{
                myobj *pobj=psk0->findObjByName(tstrlist.at(i),geteventstr);
                if(!pobj){
                    pobj=new myobj(psk0->parent());
                    pobj->name=tstrlist.at(i);
                    pobj->type=getobjlist.first()->type;
                    pobj->isDynamic=false;
                }
                getobjlist<<pobj;
            }
        }
        QString getrm=strlist.at(3);
        psk0->addCondition(geteventstr,getblockstr,getobjlist,getrm);
    }
    else if(gettrans.startsWith(type2trans(Foreach)+":")){
        QString tstr=gettrans.mid(type2trans(Foreach).length()+1);
        tstr.replace("\\|","\\\\");
        QStringList strlist=tstr.split("|");
        strlist.replaceInStrings("\\\\","|");
        QString geteventstr=strlist.first();
        QString getblockstr;
        if(myevent::isEvent(strlist.at(1))){getblockstr=strlist.at(1);}
        else{
            int index1=strlist.at(1).split("->").first().toInt();
            int index2=strlist.at(1).split("->").last().toInt();
            getblockstr=psk0->dolist.at(index1)->getBlockName(index2);
        }
        myobj *pobj;
        if(strlist.at(2).contains("->")){
            int index1=strlist.at(2).split("->").first().toInt();
            int index2=strlist.at(2).split("->").last().toInt();
            pobj=psk0->dolist.at(index1)->getObj(index2);
        }
        else{
            pobj=psk0->findObjByName(strlist.at(2),geteventstr);
            if(!pobj){
                qWarning()<<"dotrans foreach:"<<strlist.at(2);
            }
        }
        QString getrm=strlist.at(3);
        psk0->addForeach(geteventstr,getblockstr,pobj,getrm);
    }
    */
}
QString mydo::getBlockName(int index){
    myfunction *pt=static_cast<myfunction *>(blocklist.first());
    QStringList tstrlist;
    tstrlist=pt->need4block();
    if(index>=tstrlist.length()){qWarning()<<"getblockname"<<pt->name;}
    return tstrlist.at(index);
    /*
    if(type==Sentence){
        myfunction *pt=static_cast<myfunction *>(blocklist.first());
        QStringList tstrlist;
        tstrlist=pt->need4block();
        return tstrlist.at(index);
    }
    else if(type==Condition){
        mycondition *pt=static_cast<mycondition *>(blocklist.first());
        QStringList tstrlist;
        tstrlist=pt->need4block();
        return tstrlist.at(index);
    }
    else if(type==Foreach){
        myforeach *pt=static_cast<myforeach *>(blocklist.first());
        QStringList tstrlist;
        tstrlist=pt->need4block();
        return tstrlist.at(index);
    }
    return "";*/
}
myobj *mydo::getObj(int index){
    if(index>=objlist.length()){qWarning()<<"0227";}
    return objlist.at(index);
}
