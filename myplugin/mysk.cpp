#include "mysk.h"
#include "mysys.h"
int mysk::globalint=0;
int mysk::str2property(QString getstr,int gettype){
    switch(gettype){
    case TriggerSkill:return mytrs::str2property(getstr);
    case ViewAsSkill:return myvs::str2property(getstr);
    default:return str2property(getstr);
    }
}
void mysk::propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap){
    strmap.insert(property2str(Name),name);
    strmap.insert(property2str(Translation),translation);
    strmap.insert(property2str(Owner),owner?owner->name:"NULL");
    QStringList gstrlist=getsys()->getgstrlist();
    gstrlist.prepend("NULL");
    strlistmap.insert(property2str(Owner),gstrlist);
    strmap.insert(property2str(Description),description);
    foreach(QString stri,wordslist){
        strmap.insertMulti(property2str(Words),stri);
    }
}
void mysk::propertymap_set(QMap<QString, QString> &strmap){
    if(strmap.contains(property2str(Name))){setName(strmap.value(property2str(Name)));}
    if(strmap.contains(property2str(Translation))){translation=strmap.value(property2str(Translation));}
    if(strmap.contains(property2str(Owner))){
        setOwner(getsys()->findGeneralByName(strmap.value(property2str(Owner))));
    }
    if(strmap.contains(property2str(Description))){description=strmap.value(property2str(Description));}
    if(strmap.contains(property2str(Words))){
        wordslist.clear();
        wordslist.append(strmap.values(property2str(Words)));
    }
    getsys()->sig_update();
}
void mysk::getavlobjlist(int gettype, QList<myobj *> &list,QString getstr){
    QList<myobj *> tlist;
    foreach(myobj *ip,avlobjlist){
        if(ip->matchBlock(getstr)&&!ip->isGlobal){
            tlist<<ip;
        }
    }
    getsys()->getavlobjlist_global(tlist);
    foreach(myobj *ip,tlist){
        if(ip->matchType(gettype)){list<<ip;}
    }
}
/*
void mysk::getusdobjlist(int gettype, QList<myobj *> &list, QString getstr){
    foreach(myobj *ip,usdobjlist){
        if(ip->matchType(gettype)&&ip->matchBlock(getstr)){
            list<<ip;
        }
    }
    //qWarning()<<"getusd:"<<usdobjlist.length()<<list.length();
}
void mysk::addusdobjlist(myobj *getp){
    bool b=false;
    foreach(myobj *ip,usdobjlist){
        if((ip->name==getp->name)&&(ip->blockstr==getp->blockstr)){b=true;break;}
    }
    if(!b){usdobjlist<<getp;}
}
void mysk::addusdobjlist(QList<myobj *> &list){
    foreach(myobj *ip,list){addusdobjlist(ip);}
}
*/
myobj *mysk::findObjByName(QString getname, QString getstr){
    QList<myobj *> tlist;
    getavlobjlist(myobj::All,tlist,getstr);
    foreach(myobj *ip,tlist){
        if(ip->name==getname){return ip;}
    }
    return NULL;
}
void mysk::setDefaultName(){
    setName("sk"+QString::number(globalint++));
}
void mysk::setName(QString getname){
    if(getname==""){setDefaultName();return;}
    foreach(mysk *ip,getsys()->sklist){
        if(ip==this){continue;}
        if(ip->name==getname){setDefaultName();return;}
    }
    name=getname;
}
void mysk::setOwner(mygeneral *getp){
    if(owner==getp){return;}
    if(owner){
        owner->sklist.removeOne(this);
    }
    owner=getp;
    if(getp&&!getp->sklist.contains(this)){getp->sklist.append(this);}
    sig_update();
}
mysys *mysk::getsys(){
    return static_cast<mysys *>(parent());
}
void mysk::sig_update(){
    getsys()->sig_update();
}

void mytrs::addFunction(QString geteventstr,QString getblockstr,
                   QString getfunstr ,QList<myobj *> &getobjlist,
                   QStringList &getrtrmlist, QStringList &getblrmlist){
    myfunction *pfunc=new myfunction(this);
    pfunc->funname=getfunstr;
    pfunc->objlist<<getobjlist;
    pfunc->inilist<<QVariant(geteventstr)<<QVariant(getblockstr);
    pfunc->myini(getrtrmlist,getblrmlist);
    avlobjlist<<pfunc->rtobjlist;
    bool b=false;
    foreach(myopr *ip,oprlist){
        if(ip->eventstr==geteventstr){
            ip->addBlock(pfunc,getblockstr);
            b=true;
            break;
        }
    }
    if(!b){
        myopr *p_opr=new myopr(this);
        p_opr->eventstr=geteventstr;
        p_opr->myini();
        p_opr->addBlock(pfunc,getblockstr);
        oprlist.append(p_opr);
    }
    mydo *pdo=new mydo(this);
    //pdo->type=mydo::Sentence;
    pdo->objlist<<pfunc->rtobjlist;
    pdo->blocklist.append(pfunc);
    dolist.append(pdo);
    undolist.clear();
    //rfrObj();
    //addusdobjlist(getobjlist);
    sig_update();
}
/*
void mytrs::addCondition(QString geteventstr, QString getblockstr, QList<myobj *> &getobjlist,QString getrm){
    mycondition *p=new mycondition(this);
    //p->name=getblockstr+","+getobjlist.first()->name;
    p->tgtobj=getobjlist.takeFirst();
    p->caseobjlist.append(getobjlist);
    p->remark=getrm;
    p->inilist<<QVariant(geteventstr)<<QVariant(getblockstr);//qWarning()<<"addcondition:"<<geteventstr<<getblockstr;
    p->myini();
    //seg4event.addCondition(p);
    bool b=false;
    foreach(myopr *ip,oprlist){
        if(ip->eventstr==geteventstr){
            ip->addBlock(p,getblockstr);
            b=true;
            break;
        }
    }
    if(!b){
        myopr *p_opr=new myopr(this);
        p_opr->eventstr=geteventstr;
        p_opr->myini();
        //p_opr->setBlock(NULL);
        p_opr->addBlock(p,getblockstr);
        oprlist.append(p_opr);
    }

    mydo *pdo=new mydo(this);
    //pdo->type=mydo::Condition;
    pdo->blocklist.append(p);
    dolist.append(pdo);

    addusdobjlist(getobjlist);
    sig_update();

    //myshow();
}
void mytrs::addForeach(QString geteventstr, QString getblockstr, myobj *getobj, QString getrm){
    myforeach *p=new myforeach(this);
    //p->name=getblockstr+","+getobj->name;
    p->tgtobj=getobj;
    p->remark=getrm;
    p->inilist<<QVariant(geteventstr)<<QVariant(getblockstr);
    p->myini();
    avlobjlist.append(p->iobj);
    bool b=false;
    foreach(myopr *ip,oprlist){
        if(ip->eventstr==geteventstr){
            ip->addBlock(p,getblockstr);
            b=true;
            break;
        }
    }
    if(!b){
        myopr *p_opr=new myopr(this);
        p_opr->eventstr=geteventstr;
        p_opr->myini();
        //p_opr->setBlock(NULL);
        p_opr->addBlock(p,getblockstr);
        oprlist.append(p_opr);
    }
    mydo *pdo=new mydo(this);
    //pdo->type=mydo::Foreach;
    pdo->blocklist<<p;
    pdo->objlist<<p->iobj;
    dolist.append(pdo);

    addusdobjlist(getobj);
    sig_update();
    //myshow();
}
*/
void mytrs::undo(){
    if(dolist.isEmpty()){return;}
    mydo *pdo=dolist.takeLast();
    foreach(myblock *ip,pdo->blocklist){
        bool b=false;
        foreach(myopr *ipopr,oprlist){
            if(ipopr->removeBlock(ip)){
                b=true;break;
            }
        }
        if(!b){qWarning()<<"removecode:"<<ip->name;}
    }
    foreach(myobj *ip,pdo->objlist){
        if(!removeObj(ip)){qWarning()<<"removeobj:"<<ip->name;}
    }
    undolist.append(pdo);    
    sig_update();
}
void mytrs::redo(){
    if(undolist.isEmpty()){return;}    
    mydo *pdo=undolist.takeLast();
    foreach(myblock *ip,pdo->blocklist){
        bool b=false;
        foreach(myopr *ipopr,oprlist){
            if(ipopr->insertBlock(ip)){b=true;break;}
        }
        if(!b){qWarning()<<"insertcode:"<<ip->name;}
    }
    avlobjlist.append(pdo->objlist);
    dolist.append(pdo);

    sig_update();
}

void mytrs::rfrObj(){
    //avlobjlist.clear();
    //usdobjlist.clear();
    //iniObj();
}

void mytrs::iniObj(){
    QStringList strlist;
    strlist<<"trs";
    myobj::newConst(avlobjlist,"trs",parent());
    foreach(QString stri,myevent::geteventstrlist()){
        myevent::getavlobjlist(stri,avlobjlist,parent());
    }
}
bool mytrs::removeObj(myobj *getp){
    return avlobjlist.removeOne(getp);
}

QStringList mytrs::trans(){
    QString str;
    QStringList strlist,tstrlist;
    strlist<<name+"=sgs.CreateTriggerSkill{"<<QString("name=\"%1\",").arg(name);
    foreach(myopr *ip,oprlist){
        tstrlist<<myevent::trans(ip->eventstr);
    }
    strlist<<QString("events={%1},").arg(tstrlist.join(","));
    //strlist.clear();
    strlist<<QString("frequency=%1,").arg(type2trans(subtype));
    strlist<<"on_trigger=function(self,event,player,data)";
    strlist<<"local room=player:getRoom()";
    strlist<<trans4avlobjlist();    
    foreach(myopr *ip,oprlist){
        strlist<<ip->trans();
    }
    strlist<<"end,";
    strlist<<"can_trigger=function(self,player)";
    strlist<<"local room=player:getRoom()\nlocal selfplayer=room:findPlayerBySkillName(self:objectName())";
    strlist<<"if selfplayer==nil then return false end\nreturn selfplayer:isAlive()";
    strlist<<"end,\n}";
    return strlist;
}
QStringList mytrs::trans4avlobjlist(QString geteventstr){
    QStringList strlist;    
    foreach(myobj *ip,avlobjlist){
        if(ip->blockstr!=geteventstr){continue;}
        if(ip->noDeclaration){continue;}
        if(myobj::isConst(ip->name)){
            strlist<<myobj::transConst(ip->name);
            continue;
        }        
        strlist<<QString("local %1").arg(ip->name);        
    }
    return strlist;
}
QString mytrs::trans4design(){
    QString str="";
    foreach(mydo *ip,dolist){
        str+=">>>>"+name+"::"+ip->trans()+"\n";
    }
    return str;
}

QStringList mytrs::need4block(QString getstr){
    foreach(myopr *ip,oprlist){
        if(ip->eventstr==getstr){
            return ip->need4block();
        }
    }
    return QStringList();
}

bool mytrs::newEvent(QString geteventstr){
    foreach(myopr *ip,oprlist){
        if(ip->eventstr==geteventstr){return false;}
    }
    return true;
}
void mytrs::newObj(QString geteventstr){
    myevent::getavlobjlist(geteventstr,avlobjlist,parent());
}

myblock *mytrs::findBlockByName(QString getname){
    myblock *pblock;
    foreach(myopr *ip,oprlist){
        pblock=ip->findBlockByName(getname);
        if(pblock){return pblock;}
    }
    return NULL;
    //return seg4event.findBlockByName(getname);
}
myfunction *mytrs::findFuncByObj(myobj *getp){
    myfunction *pfunc;
    foreach(myopr *ip,oprlist){
        pfunc=ip->findFuncByObj(getp);
        if(pfunc){return pfunc;}
    }
    return NULL;
}
QString mytrs::findRemarkByName_event(QString getname){
    return myevent::findRemarkByName(getname);
}
QString mytrs::findRemarkByName_block(QString getname){
    if(myevent::isEvent(getname)){return "default";}
    QString tstr="";
    myblock *pt=findBlockByName(getname);
    if(pt){
        tstr+=pt->remark;
        if(pt->upperLayer->getType()==mycode::Function){
            tstr+="  /from:";
            tstr+=myfun::findRemarkByName(static_cast<myfunction *>(pt->upperLayer)->funname);
        }
    }
    else{qWarning()<<getname;}
    return tstr;
}
void mytrs::propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap){
    mysk::propertymap_get(strmap,strlistmap);
    strmap.insert(property2str(Subtype),type2str(subtype));
    strlistmap.insert(property2str(Subtype),typestrlist());
}
void mytrs::propertymap_set(QMap<QString, QString> &strmap){    
    if(strmap.contains(property2str(Subtype))){subtype=str2type(strmap.value(property2str(Subtype)));}
    mysk::propertymap_set(strmap);
}

QStringList myvs::trans(){
    QStringList strlist;
    strlist<<name+"=sgs.CreateViewAsSkill{";
    strlist<<QString("name=\"%1\",").arg(name);
    strlist<<QString("n=%1,").arg(vsn);
    if(vfblock){
        strlist<<"view_filter=function(self,selected,to_select)";
        strlist<<vfblock->trans();
        strlist<<"end,";
    }
    strlist<<"view_as=function(self,cards)";
    strlist<<"if #cards~="+QString::number(vsn)+" then return nil end";
    strlist<<QString("local a_card=sgs.Sanguosha:cloneCard(\"%1\",sgs.Card_SuitToBeDecided,0)").arg(objname_viewas);
    strlist<<"for var=1,#cards,1 do a_card:addSubcard(cards[var]) end";
    strlist<<"a_card:setSkillName(self:objectName())";
    strlist<<"return a_card";
    strlist<<"end,";
    if(epblock){
        strlist<<"enabled_at_play=function(self,player)";
        strlist<<epblock->trans();
        strlist<<"end,";
    }
    if(erblock){
        strlist<<"enabled_at_response=function(self,player,pattern)";
        strlist<<erblock->trans();
        strlist<<"end,";
    }
    strlist<<"}";
    return strlist;
}
void myvs::iniObj(){
    QList<myobj *> vfobjlist,epobjlist,erobjlist;
    myobj::newConst(vfobjlist,"vs_vf",parent(),true);
    foreach(myobj *ip,vfobjlist){ip->blockstr=vsbtype2str(ViewFilter);}
    myobj::newConst(epobjlist,"vs_ep",parent(),true);
    foreach(myobj *ip,epobjlist){ip->blockstr=vsbtype2str(EnabledAtPlay);}
    myobj::newConst(erobjlist,"vs_er",parent(),true);
    foreach(myobj *ip,erobjlist){ip->blockstr=vsbtype2str(EnabledAtResponse);}
    avlobjlist<<vfobjlist<<epobjlist<<erobjlist;
    myobj::newConst(avlobjlist,"",parent());
}
void myvs::addVF(QString getblockstr,QString getfunstr ,QList<myobj *> &getobjlist,
                 QStringList &getrtrmlist, QStringList &getblrmlist){
    myfunction *p_stc=new myfunction(this);
    p_stc->funname=getfunstr;
    p_stc->objlist<<getobjlist;
    p_stc->inilist<<QVariant(vsbtype2str(ViewFilter))<<QVariant(getblockstr);
    p_stc->myini(getrtrmlist,getblrmlist);
    avlobjlist<<p_stc->rtobjlist;
    if(!vfblock){
        vfblock=new myblock(this);
        vfblock->name=vsbtype2str(ViewFilter);
    }
    vfblock->addBlock(p_stc,getblockstr);
    //rfrObj();
    //addusdobjlist(getobjlist);
    sig_update();
}
void myvs::addFunction(QString geteventstr, QString getblockstr,
                       QString getfunstr, QList<myobj *> &getobjlist,
                       QStringList &getrtrmlist, QStringList &getblrmlist){
    if(geteventstr==vsbtype2str(ViewFilter)){
        addVF(getblockstr,getfunstr,getobjlist,getrtrmlist,getblrmlist);
    }
}
QStringList myvs::need4block(QString getstr){
    if(vfblock&&(getstr==vsbtype2str(ViewFilter))){
        return vfblock->need4block();
    }
    return QStringList();
}
myblock *myvs::findBlockByName(QString getname){
    myblock *pr=NULL;
    if(vfblock){
        pr=vfblock->findBlockByName(getname);
        if(pr){return pr;}
    }
    if(epblock){
        pr=epblock->findBlockByName(getname);
        if(pr){return pr;}
    }
    if(erblock){
        pr=erblock->findBlockByName(getname);
        if(pr){return pr;}
    }
    return NULL;
}
myfunction *myvs::findFuncByObj(myobj *getp){
    myfunction *pr=NULL;
    if(vfblock){
        pr=vfblock->findFuncByObj(getp);
        if(pr){return pr;}
    }
    if(epblock){
        pr=epblock->findFuncByObj(getp);
        if(pr){return pr;}
    }
    if(erblock){
        pr=erblock->findFuncByObj(getp);
        if(pr){return pr;}
    }
    return NULL;
}
QString myvs::findRemarkByName_event(QString getname){
    if(getname==vsbtype2str(ViewFilter)){return getname;}
    if(getname==vsbtype2str(EnabledAtPlay)){return getname;}
    if(getname==vsbtype2str(EnabledAtResponse)){return getname;}
    return QString();
}
QString myvs::findRemarkByName_block(QString getname){
    if(getname==vsbtype2str(ViewFilter)){return getname;}
    if(getname==vsbtype2str(EnabledAtPlay)){return getname;}
    if(getname==vsbtype2str(EnabledAtResponse)){return getname;}
    return QString();
}
void myvs::propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap){
    mysk::propertymap_get(strmap,strlistmap);
    strmap.insert(property2str(CardsNum),QString::number(vsn));
    strmap.insert(property2str(CardViewAs),myobj::name2remark(objname_viewas));
    strlistmap.insert(property2str(CardViewAs),myobj::getconstrmlist_tag("ob"));
}
void myvs::propertymap_set(QMap<QString, QString> &strmap){    
    if(strmap.contains(property2str(CardsNum))){
        bool b;
        int getn=strmap.value(property2str(CardsNum)).toInt(&b);
        if(b){vsn=getn;}
    }
    if(strmap.contains(property2str(CardViewAs))){
        QString getstr=strmap.value(property2str(CardViewAs));
        QString getname=myobj::remark2name(getstr);
        objname_viewas=getname;
    }
    mysk::propertymap_set(strmap);
}
