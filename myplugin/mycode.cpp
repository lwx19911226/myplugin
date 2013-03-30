#include "mycode.h"
#include "mysk.h"
#include "mysys.h"

int myblock::globalint=0;
void myblock::addBlock(myblock *getp){
    if(getp->name==name){
        foreach(myblock *ip,getp->blocklist){
            if(ip->getType()>=Block){
                ip->upperLayer=this;
            }
        }
        blocklist.append(getp->blocklist);
    }
    else{
        getp->upperLayer=this;
        blocklist.append(getp);
    }
}
void myblock::addBlock(myblock *getp, QString getstr){
    if(!matchName(getstr)){return;}
    if(name==getstr){
        addBlock(getp);
        return;
    }
    foreach(myblock *ip,blocklist){
        if(ip->matchName(getstr)){
            ip->addBlock(getp,getstr);
            return;
        }
    }    
}
bool myblock::matchName(QString getstr){
    return getstr.startsWith(name);
}
QStringList myblock::trans(){
    QStringList strlist;
    //QString blank=QString("    ");
    foreach(myblock *ip,blocklist){
        strlist<<ip->trans();
    }
    return mycode::myindent(strlist);
}
int myblock::getLayer(){
    myblock *p=this;
    int cnt=1;
    while(1){
        p=p->upperLayer;
        if(!p){return cnt;}
        if(p->getType()>Block){continue;}
        cnt++;
    }
}
QStringList myblock::need4block(){
    QStringList list;
    list.append(name);
    foreach(myblock *ip,blocklist){
        list<<ip->need4block();
    }
    return list;
}
myblock *myblock::findBlockByName(QString getname){
    if(name==getname){return this;}
    myblock *pblock;
    foreach(myblock *ip,blocklist){
        if(ip->getType()>=Block){
            pblock=ip->findBlockByName(getname);
            if(pblock){return pblock;}
        }
    }
    return NULL;
}
myfunction *myblock::findFuncByObj(myobj *getp){
    myfunction *pfunc;
    foreach(myblock *ip,blocklist){
        pfunc=ip->findFuncByObj(getp);
        if(pfunc){return pfunc;}
    }
    return NULL;
}

bool myblock::removeBlock(myblock *getp){
    for(int i=0;i<blocklist.length();i++){
        //qWarning()<<name<<i;
        if(blocklist.at(i)==getp){
            blocklist.removeAt(i);
            //getp->inilist.clear();
            //getp->inilist<<QVariant(getEvent())<<QVariant(name)<<QVariant(i);
            return true;
        }
        if(blocklist.at(i)->getType()>=Block){
            if(blocklist.at(i)->removeBlock(getp)){return true;}
        }
    }
    return false;
}
/*
bool myblock::insertBlock(myblock *getp){
    if(getp->inilist.length()<3){return false;}
    if(getp->inilist.at(1).toString()==name){
        int index=getp->inilist.at(2).toInt();
        if(getp->getType()>=Block){
            getp->upperLayer=this;
        }
        blocklist.insert(index,getp);
        return true;
    }
    foreach(myblock *ip,blocklist){
        if(ip->getType()>=Block){
            if(ip->insertBlock(getp)){return true;}
        }
    }
    return false;
}
*/
QTreeWidgetItem *myblock::mytreeitem(){
    QStringList strlist;
    strlist<<name<<myobj::enumstr(metaObject(),"blockType",getType())<<getRemark();
    QTreeWidgetItem *pitem=new QTreeWidgetItem(strlist);
    foreach(myblock *ip,blocklist){pitem->addChild(ip->mytreeitem());}
    return pitem;
}

myblock *myblock::getTopBlock(){
    myblock *p=this;
    while(p->upperLayer){p=p->upperLayer;}
    return p;
}
mysk *myblock::getsk0(){
    return static_cast<mysk *>(getTopBlock()->parent());
}
QString myblock::getEvent(){
    myblock *p=getTopBlock();
    if(!getsk0()->eventstrlist().contains(p->name)){qWarning()<<"getevent:"<<p->name;}
    return p->name;
}
QString myblock::getRemark(){
    if(getsk0()->eventstrlist().contains(name)){return getsk0()->findRemarkByName_event(name);}
    QString tstr=remark;
    if(upperLayer&&upperLayer->getType()==myblock::Function){
        if(tstr==""){
            tstr=myobj::enumstr(metaObject(),"blockType",Block)
                    +QString::number(upperLayer->blocklist.indexOf(this)+1);
        }
        tstr+="  /from: ";
        tstr+=upperLayer->getRemark();
    }
    return tstr;
}

void myfunction::myini(QString geteventstr,QString getblockstr,QStringList &rtrmlist, QStringList &blrmlist){
    name=getblockstr+","+funname+QString::number(globalint++);
    //QString tstr=myfun::getTrans(funname);
    for(int i=0;i<objlist.length();i++){
        if(myfun::notnil(funname,i)&&!objlist.at(i)->isVerified){
            objlist.at(i)->isVerified=true;
            vrlist<<i;
        }
    }
    QStringList list=myfun::get(funname);
    for(int i=0;i<list.length();i++){
        myobj *p=new myobj(this);
        QString typestr=myobj::gettypestr(list.at(i));
        QString typesuffix=myobj::gettypesuffix(list.at(i));
        if(typesuffix.contains("$")){
            p->noDeclaration=true;
            p->isGlobal=true;
            //p->isDynamic=false;
        }
        else{
            p->blockstr=geteventstr;
        }
        p->name=QString("%1_%2_%3").arg(funname,typestr).arg(globalint++);
        p->type=myobj::str2type(list.at(i));
        p->remark=rtrmlist.at(i);
        p->isVerified=myfun::notnil(funname,objlist.length()+i);
        rtobjlist.append(p);
    }
    for(int i=0;i<myfun::getBlock_cnt(funname);i++){
        myblock *p=new myblock(this);
        p->name=name+"_"+QString::number(i+1);
        if(i<blrmlist.length()){p->remark=blrmlist.at(i);}
        myblock::addBlock(p);
    }
}
QStringList myfunction::trans(){
    QString str=myfun::getTrans(funname);
    QList<myobj *> list;
    list<<objlist<<rtobjlist;
    for(int i=0;i<list.length();i++){
        QRegExp rx("<"+QString::number(i+1)+":([^>]*)>");
        if(rx.indexIn(str)!=-1){
            QString tstr;
            if(vrlist.contains(i)){
                if(rx.cap(1)!=""){tstr="\\1";}
                //else if(myobj::b4vrstr(objlist.at(i)->type)){tstr="";}
                else{tstr="if not %"+QString::number(i+1)+" then return defaultReturn end;";}
            }
            str.replace(rx,tstr);
        }
    }
    if(str.contains("%%")){str.replace(QRegExp("%%"),QString::number(globalint++));}
    for(int i=0;i<list.length();i++){
        if(str.contains("%"+QString::number(i+1))){
            str.replace(QRegExp("([^\\\\]?)%"+QString::number(i+1)),"\\1"+list[i]->trans());
        }
    }
    QStringList strlist=str.split(";");
    QStringList tlist;
    int j=0;
    for(int i=0;i<strlist.length();i++){
        if(strlist.at(i)=="%block"){
            tlist<<blocklist.at(j++)->trans();
        }
        else{tlist<<strlist.at(i);}
    }
    return tlist;
}
QStringList myfunction::need4block(){
    QStringList strlist;
    foreach(myblock *ip,blocklist){strlist<<ip->need4block();}
    return strlist;
}
void myfunction::addBlock(myblock *getp, QString getstr){
    foreach(myblock *ip,blocklist){ip->addBlock(getp,getstr);}
}
myfunction *myfunction::findFuncByObj(myobj *getp){
    if(rtobjlist.contains(getp)){return this;}
    return myblock::findFuncByObj(getp);
}
QString myfunction::getRemark(){
    QString tstr;
    QString rmstr=myfun::findRemarkByName(funname);
    for(int i=0;i<objlist.length();i++){
        if(!rmstr.contains(str4parameter()+QString::number(i+1))){qWarning()<<"getremark"<<funname<<i;}
        tstr=getsk0()->findRemarkByName_obj(objlist.at(i),getsk0()->getsys()->findFuncByObj(objlist.at(i)),false);
        if(tstr.contains(str4parameter())){tstr.replace(str4parameter(),str4parameter()+" ");}
        if(tstr.contains(str4returnvalue())){tstr.replace(str4returnvalue(),str4returnvalue()+" ");}
        rmstr.replace(str4parameter()+QString::number(i+1),
                     QString("\"%1|%2\"").arg(objlist.at(i)->name,tstr));
    }

    for(int i=0;i<rtobjlist.length();i++){
        if(!rmstr.contains(str4returnvalue()+QString::number(i+1))){qWarning()<<"getremark"<<funname<<i;}
        tstr=getsk0()->findRemarkByName_obj(rtobjlist.at(i),this,false);
        if(tstr.contains(str4parameter())){tstr.replace(str4parameter(),str4parameter()+" ");}
        if(tstr.contains(str4returnvalue())){tstr.replace(str4returnvalue(),str4returnvalue()+" ");}
        rmstr.replace(str4returnvalue()+QString::number(i+1),
                     QString("\"%1|%2\"").arg(rtobjlist.at(i)->name,tstr));
    }
    rmstr.replace(str4parameter()+" ",str4parameter());
    rmstr.replace(str4returnvalue()+" ",str4returnvalue());
    return rmstr;
}

/*
void myopr::myini(){
    name=eventstr;
}
QStringList myopr::trans(){
    QStringList strlist;
    strlist<<QString("if event==%1 then").arg(myevent::trans(eventstr));
    strlist<<mycode::myindent(myevent::trans4eventdata(eventstr,true));
    strlist<<mycode::myindent(static_cast<mytrs *>(parent())->trans4avlobjlist(eventstr));
    strlist<<myblock::trans();
    strlist<<mycode::myindent(myevent::trans4eventdata(eventstr,false));
    strlist<<"end";
    return strlist;
}
*/
/*
void mycondition::myini(){
    name=inilist.at(1).toString()+","+tgtobj->name;
    myblock *p;
    foreach(myobj *ip,caseobjlist){
        p=new myblock(this);
        p->name=name+"_"+ip->name;
        p->remark=remark+" case("+ip->remark+")";
        map.insert(ip,p);
        static_cast<myblock *>(this)->addBlock(p);
    }
    p=new myblock(this);
    p->name=name+"_else";
    p->remark=remark+" case(else)";
    static_cast<myblock *>(this)->addBlock(p);
}
//void mycondition::addStc(myfunction *getstc, myobj *getobj){map[getobj]->addStc(getstc);}
void mycondition::addBlock(myblock *getp, QString getstr){
    QString str=getstr.mid(name.length()+1);    
    if(str.startsWith("else")){
        static_cast<myblock *>(blocklist.last())->addBlock(getp,getstr);
        return;
    }
    foreach(myobj *ip,caseobjlist){
        if(str.startsWith(ip->name)){
            map[ip]->addBlock(getp,getstr);
        }
    }
}
QStringList mycondition::trans(){
    QStringList strlist;
    bool bf=true;
    //QString blank=QString("    ").repeated(getLayer());
    foreach(myobj *ip,caseobjlist){
        //if(!bf){str+="else";}
        strlist<<(bf?"":"else")+QString("if %1==%2 then").arg(tgtobj->name).arg(ip->name);
        strlist<<map[ip]->trans();
        bf=false;
    }
    strlist<<"else";
    strlist<<blocklist.last()->trans();
    strlist<<"end";
    return strlist;
}
QStringList mycondition::need4block(){
    QStringList strlist;
    foreach(myblock *ip,blocklist){
        strlist<<ip->need4block();
    }
    return strlist;
}

void myforeach::myini(){
    name=inilist.at(1).toString()+","+tgtobj->name;

    iobj=new myobj(this);
    iobj->name=tgtobj->name+"_i";
    iobj->type=myobj::getsubtype(tgtobj->type);
    iobj->remark=tgtobj->remark+" iterator";
    iobj->eventstr=inilist.first().toString();
    iobj->noDeclaration=true;

    myblock *p=new myblock(this);
    p->name=name+"_"+tgtobj->name;
    p->remark=remark;
    static_cast<myblock *>(this)->addBlock(p);
}
QStringList myforeach::trans(){
    QStringList strlist;
    QString cpy=QString("%1_cpy%2").arg(tgtobj->name).arg(globalint++);
    //strlist<<QString("for _,%2_i in sgs.qlist(%1) do %2:append(%2_i) end").arg(tgtobj->name,cpy);
    strlist<<QString("for _,%1 in sgs.qlist(%2) do").arg(iobj->name).arg(tgtobj->name);
    strlist<<blocklist.first()->trans();
    strlist<<"end";
    return strlist;
}
QStringList myforeach::need4block(){
    QStringList strlist;
    foreach(myblock *ip,blocklist){
        strlist<<ip->need4block();
    }
    return strlist;
}
void myforeach::addBlock(myblock *getp, QString getstr){
    if(getstr.startsWith(blocklist.first()->name)){
        blocklist.first()->addBlock(getp,getstr);
    }
}
*/
/*
void myfunction::set(QString funname, QList<myobj *> getobjlist,
                QList<QString> &rtnamelist,QList<QString> &rtrmlist,QString geteventstr,
                QList<QString> &blrmlist){
    funp=new myfun(this);
    funp->name=funname;
    funp->setrt(rtnamelist,rtrmlist,geteventstr);
    funp->setBlock(blrmlist);
    funp->objlist.append(getobjlist);
}
*/
