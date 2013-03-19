#include "myopr.h"
#include "mysk.h"

myopr::myopr(QObject *parent) :
    QObject(parent)
{
    blockp=NULL;
}

void myopr::addBlock(myblock *getp){
    if(blockp==NULL){}
    blockp->addBlock(getp);
}
void myopr::addBlock(myblock *getp, QString getstr){
    if(blockp==NULL){}
    blockp->addBlock(getp,getstr);
}

void myopr::myini(){
    blockp=new myblock(parent());
    blockp->name=eventstr;
}

QStringList myopr::trans(){
    QStringList strlist;
    //QString str="if event==";
    //str+=myevent::trans(eventstr);
    //str+=" then\n";
    strlist<<QString("if event==%1 then").arg(myevent::trans(eventstr));
    //QList<myobj *> usdlist;
    //static_cast<mytrs *>(parent())->getusdobjlist(myobj::All,usdlist,eventstr);
    strlist<<mycode::myindent(myevent::trans4eventdata(eventstr,true));
    //strlist=myevent::trans4eventdata(eventstr);
    //foreach(QString stri,strlist){
    //    str+="    "+stri+"\n";
    //}
    //strlist.clear();
    //str+="    "+myevent::trans4eventdata(eventstr);
    strlist<<mycode::myindent(static_cast<mytrs *>(parent())->trans4avlobjlist(eventstr));
    //strlist=static_cast<mytrs *>(QObject::parent())->trans4avlobjlist(eventstr);
    //foreach(QString stri,strlist){
    //    str+="    "+stri+"\n";
    //}
    //strlist.clear();
    //str+="    "+static_cast<mytrs *>(QObject::parent())->trans4avlobjlist(eventstr);
    strlist<<blockp->trans();
    strlist<<mycode::myindent(myevent::trans4eventdata(eventstr,false));
    strlist<<"end";
    return strlist;
}

QStringList myopr::need4block(){
    return blockp->need4block();
}

myblock *myopr::findBlockByName(QString getname){
    return blockp->findBlockByName(getname);
}
myfunction *myopr::findFuncByObj(myobj *getp){
    return blockp->findFuncByObj(getp);
}

bool myopr::removeBlock(myblock *getp){
    return blockp->removeBlock(getp);
    //if(blockp->removeBlock(getp,list)){
    //    list.prepend(QVariant(eventstr));
    //    return true;
    //}
    //return false;
}
bool myopr::insertBlock(myblock *getp){
    if(getp->inilist.isEmpty()){return false;}
    if(getp->inilist.at(0).toString()!=eventstr){return false;}
    return blockp->insertBlock(getp);
}
/*
void myopr::setBlock(myblock *getp){
    blockp=new myblock(parent());
    blockp->name=eventstr;
    if(getp!=NULL){
        if(getp->inilist.length()>1){blockp->name=getp->inilist.at(1).toString();}
        blockp->blocklist<<getp;
    }
}
*/
