#include "mysk.h"
#include "mysys.h"
int mysk::globalint=0;
/*
int mysk::str2property(QString getstr,int gettype){
    switch(gettype){
    case TriggerSkill:return mytrs::str2property(getstr);
    case ViewAsSkill:return myvs::str2property(getstr);
    default:return str2property(getstr);
    }
}
*/
void mysk::propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap,bool b4remark){
    if(b4remark){}
    strmap.insert(property2str(Name),name);
    strmap.insert(property2str(Translation),translation);
    strmap.insert(property2str(Owner),owner?owner->name:mygeneral::nullname());
    QStringList gstrlist=getsys()->getgstrlist();
    gstrlist.prepend(mygeneral::nullname());
    strlistmap.insert(property2str(Owner),gstrlist);
    strmap.insert(property2str(Description),description);
    foreach(QString stri,wordslist){
        strmap.insertMulti(property2str(Words),stri);
    }
}
void mysk::propertymap_set(QMap<QString, QString> &strmap,bool b4remark){
    if(b4remark){}
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
QString mysk::propertystr_get(){
    QStringList strlist;
    strlist<<propertystr_get(true)<<propertystr_get(false);
    return strlist.join("|");
}
QStringList mysk::propertystr_get(bool front){
    QStringList strlist;
    if(front){
        strlist<<type2abb(getType());
        strlist<<name;
        strlist<<translation;
        strlist<<(owner?owner->name:mygeneral::nullname());
    }
    else{
        strlist<<property2prefix(Description)+description;
        strlist<<mycode::mymdf(wordslist,property2prefix(Words));
    }
    return strlist;
}
void mysk::propertystr_set(QString getstr){
    QStringList frlist,bklist,midlist;
    if(!propertystr_dvd(getstr,frlist,midlist,bklist)){qWarning()<<"propertystr_set:"<<getstr;return;}
    propertystr_set(frlist,true);
    propertystr_set(bklist,false);
}
void mysk::propertystr_set(QStringList getstrlist, bool front){
    QMap<QString,QString> strmap;
    if(front){
        if(getstrlist.length()!=4){qWarning()<<"propertystr_setfr"<<getstrlist;return;}
        strmap.insert(property2str(Name),getstrlist.at(1));
        strmap.insert(property2str(Translation),getstrlist.at(2));
        strmap.insert(property2str(Owner),getstrlist.at(3));
    }
    else{
        if(getstrlist.isEmpty()){qWarning()<<"propertystr_setbk";}
        foreach(QString stri,getstrlist){
            if(stri.startsWith(property2prefix(Description))){
                strmap.insert(property2str(Description),stri.mid(property2prefix(Description).length()));
            }
            if(stri.startsWith(property2prefix(Words))){
                strmap.insertMulti(property2str(Words),stri.mid(property2prefix(Words).length()));
            }
        }
    }
    propertymap_set(strmap,false);
}
bool mysk::propertystr_dvd(QString getstr, QStringList &frlist, QStringList &midlist, QStringList &bklist){
    QStringList strlist=getstr.split("|");
    int i1,i2;
    for(i2=strlist.length()-1;i2>=0;i2--){
        bklist.prepend(strlist.at(i2));
        if(strlist.at(i2).startsWith(":")){break;}
    }
    for(i1=0;i1<=3;i1++){
        frlist<<strlist.at(i1);
    }
    for(int i=i1;i<i2;i++){
        midlist<<strlist.at(i);
    }
    return i1<=i2;
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
    if(owner){owner->sklist.removeOne(this);}
    owner=getp;
    if(getp&&!getp->sklist.contains(this)){getp->sklist.append(this);}
    sig_update();
}
QStringList mysk::eventstrlist(){
    QString tstr_type=type2abb(getType())+"bType";
    return myobj::enumstrlist(metaObject(),tstr_type.toUtf8());
}
QString mysk::bstr2abb(QString getstr){
    QString tstr_type=type2abb(getType())+"bType";
    QString tstr_abb=type2abb(getType())+"bAbb";
    if(myobj::enumcontains(metaObject(),tstr_type.toUtf8(),getstr)){
        return myobj::enumstr(metaObject(),tstr_abb.toUtf8(),myobj::enumint(metaObject(),tstr_type.toUtf8(),getstr));
    }
    else{return QString();}
}
void mysk::iniObj(){
    foreach(QString stri,eventstrlist()){
        QString abb=bstr2abb(stri);
        if(abb==""){continue;}
        QList<myobj *> tobjlist;
        myobj::newConst(tobjlist,abb,parent(),true);
        foreach(myobj *ip,tobjlist){ip->blockstr=stri;}
        avlobjlist<<tobjlist;
    }
    myobj::newConst(avlobjlist,"",parent());
}
void mysk::getavlobjlist(int gettype, QList<myobj *> &list,QString getstr,bool b4nil){
    QList<myobj *> tlist;
    foreach(myobj *ip,avlobjlist){
        if(ip->matchBlock(getstr)&&!ip->isGlobal){tlist<<ip;}
    }
    getsys()->getavlobjlist_global(tlist);
    foreach(myobj *ip,tlist){
        if(ip->matchType(gettype)){
            if(!b4nil&&ip->name=="nil"){continue;}
            list<<ip;
        }
    }
}

QStringList mysk::trans4avlobjlist(QString getstr){
    QStringList strlist;
    foreach(myobj *ip,avlobjlist){
        if(ip->blockstr!=getstr){continue;}
        if(ip->noDeclaration){continue;}
        if(myobj::isConst(ip->name)){
            strlist<<myobj::transConst(ip->name);
            continue;
        }
        strlist<<QString("local %1").arg(ip->name);
    }
    return strlist;
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
void mysk::addFunction(QString geteventstr, QString getblockstr,
                       QString getfunstr, QList<myobj *> &getobjlist,
                       QStringList &getrtrmlist, QStringList &getblrmlist,bool b4redo){
    myfunction *pfunc=new myfunction(this);
    pfunc->funname=getfunstr;
    pfunc->objlist<<getobjlist;
    //pfunc->inilist<<QVariant(geteventstr)<<QVariant(getblockstr);
    pfunc->myini(geteventstr,getblockstr,getrtrmlist,getblrmlist);
    avlobjlist<<pfunc->rtobjlist;
    iniBlock(geteventstr)->addBlock(pfunc,getblockstr);

    mydo *pdo=new mydo(this);
    pdo->psktgt=this;
    pdo->objlist<<pfunc->rtobjlist;
    pdo->blocklist.append(pfunc);
    getsys()->dolist<<pdo;
    if(!b4redo){getsys()->undostrlist.clear();}
    //rfrObj();
    //addusdobjlist(getobjlist);
    sig_update();
}
myblock *mysk::iniBlock(QString getstr){
    if(!eventstrlist().contains(getstr)){qWarning()<<"iniblock"<<getstr;return NULL;}
    myblock *pt=findBlockByName(getstr);
    if(pt){return pt;}
    pt=new myblock(this);
    pt->name=getstr;
    blocklist<<pt;
    return pt;
}

bool mysk::removeObj(myobj *getp){
    return avlobjlist.removeOne(getp);
}
myobj *mysk::findObjByName(QString getname, QString getstr){
    QList<myobj *> tlist;
    getavlobjlist(myobj::all,tlist,getstr,true);
    foreach(myobj *ip,tlist){
        if(ip->name==getname){return ip;}
    }
    return NULL;
}
myblock *mysk::findBlockByName(QString getname){
    myblock *pr=NULL;
    foreach(myblock *ip,blocklist){
        pr=ip->findBlockByName(getname);
        if(pr){return pr;}
    }
    return NULL;
}
myfunction *mysk::findFuncByObj(myobj *getp){
    myfunction *pr=NULL;
    foreach(myblock *ip,blocklist){
        pr=ip->findFuncByObj(getp);
        if(pr){return pr;}
    }
    return NULL;
}
QString mysk::findRemarkByName_block(QString getname){
    if(eventstrlist().contains(getname)){return "default";}
    QString tstr="";
    myblock *pt=findBlockByName(getname);
    if(pt){
        tstr+=pt->remark;
        if(pt->upperLayer->getType()==myblock::Function){
            tstr+="  /from:";
            tstr+=myfun::findRemarkByName(static_cast<myfunction *>(pt->upperLayer)->funname);
        }
    }
    else{qWarning()<<getname;}
    return tstr;
}
QStringList mysk::blockstrlist(QString getstr){
    myblock *pt=findBlockByName(getstr);
    if(pt){return pt->need4block();}
    QStringList strlist;
    if(eventstrlist().contains(getstr)){strlist<<getstr;}
    return strlist;
}
/*
QStringList mysk::trans4design(){
    QStringList strlist;
    foreach(mydo *ip,dolist){
        strlist<<ip->trans();
    }
    return strlist;
}
*/
void mysk::dotrans(QString getstr){
    qWarning()<<"dotrans"<<name<<getstr;
    QString tstr=getstr;
    tstr.replace("\\|","\\\\");
    QStringList strlist=tstr.split("|");
    strlist.replaceInStrings("\\\\","|");
    QString geteventstr=strlist.first();
    QString getblockstr;
    if(eventstrlist().contains(strlist.at(1))){getblockstr=strlist.at(1);}
    else{
        int index1=strlist.at(1).split("->").first().toInt();
        int index2=strlist.at(1).split("->").last().toInt();
        getblockstr=getsys()->dolist.at(index1)->getBlockName(index2);
    }
    QString getfunstr=strlist.at(2);
    QList<myobj *> getobjlist;
    QStringList tstrlist;
    tstrlist.clear();
    tstrlist=strlist.at(3).split(",");
    if(strlist.at(3)==""){tstrlist.clear();}
    for(int i=0;i<tstrlist.length();i++){
        myobj *pobj=NULL;
        if(tstrlist.at(i).contains("->")){
            int index1=tstrlist.at(i).split("->").first().toInt();
            int index2=tstrlist.at(i).split("->").last().toInt();
            pobj=getsys()->dolist.at(index1)->getObj(index2);
        }
        else{
            pobj=findObjByName(tstrlist.at(i),geteventstr);
            if(!pobj){
                pobj=new myobj(getsys());
                pobj->name=tstrlist.at(i);
                int gettype=myobj::str2type(myfun::need(getfunstr).at(i));
                if(!myobj::b4input(gettype)){qWarning()<<"dotrans_b4input"<<gettype;}
                pobj->type=gettype;
                pobj->isVerified=true;
                //pobj->isDynamic=false;
            }
        }
        if(!pobj){qWarning()<<"dotrans_pobj"<<name<<getstr;}
        getobjlist<<pobj;
    }
    QStringList getrtrmlist;
    getrtrmlist<<strlist.at(4).split(",");
    QStringList getblrmlist;
    if(strlist.length()>=6){getblrmlist<<strlist.at(5).split(",");}
    addFunction(geteventstr,getblockstr,getfunstr,getobjlist,getrtrmlist,getblrmlist,true);
}
mysys *mysk::getsys(){
    return static_cast<mysys *>(parent());
}
void mysk::sig_update(){
    getsys()->sig_update();
}

void mytrs::iniObj(){
    QStringList strlist;
    strlist<<"trs";
    myobj::newConst(avlobjlist,"trs",parent());
    foreach(QString stri,myevent::geteventstrlist()){
        myevent::getavlobjlist(stri,avlobjlist,parent());
    }
}
QStringList mytrs::trans(){
    QStringList strlist;
    strlist<<name+"=sgs.CreateTriggerSkill{"<<QString("name=\"%1\",").arg(name);
    QStringList tstrlist;
    foreach(myblock *ip,blocklist){tstrlist<<myevent::trans(ip->name);}
    strlist<<QString("events={%1},").arg(tstrlist.join(","));
    strlist<<QString("frequency=%1,").arg(type2trans(subtype));
    strlist<<"on_trigger=function(self,event,player,data)";
    strlist<<"local room=player:getRoom()";
    strlist<<trans4avlobjlist();
    foreach(myblock *ip,blocklist){
        QStringList blockstrlist;
        blockstrlist<<QString("if event==%1 then").arg(myevent::trans(ip->name));
        blockstrlist<<mycode::myindent(myevent::trans4eventdata(ip->name,true));
        blockstrlist<<mycode::myindent(trans4avlobjlist(ip->name));
        blockstrlist<<ip->trans();
        blockstrlist<<mycode::myindent(myevent::trans4eventdata(ip->name,false));
        blockstrlist<<"end";
        strlist<<mycode::myindent(blockstrlist);
    }
    strlist<<"end,";
    strlist<<"can_trigger=function(self,player)";
    strlist<<"local room=player:getRoom()\nlocal selfplayer=room:findPlayerBySkillName(self:objectName())";
    strlist<<"if selfplayer==nil then return false end\nreturn selfplayer:isAlive()";
    strlist<<"end,\n}";
    return strlist;
}
QString mytrs::findRemarkByName_event(QString getname){
    return myevent::findRemarkByName(getname);
}
void mytrs::propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap,bool b4remark){
    mysk::propertymap_get(strmap,strlistmap,b4remark);
    strmap.insert(property2str(Subtype),subtype2str(subtype));
    strlistmap.insert(property2str(Subtype),subtypestrlist());
}
void mytrs::propertymap_set(QMap<QString, QString> &strmap,bool b4remark){
    if(strmap.contains(property2str(Subtype))){subtype=str2subtype(strmap.value(property2str(Subtype)));}
    mysk::propertymap_set(strmap,b4remark);
}
QString mytrs::propertystr_get(){
    QStringList strlist;
    strlist<<mysk::propertystr_get(true);
    strlist<<subtype2str(subtype);
    strlist<<mysk::propertystr_get(false);
    return strlist.join("|");
}
void mytrs::propertystr_set(QString getstr){
    QStringList frlist,bklist,midlist;
    if(!propertystr_dvd(getstr,frlist,midlist,bklist)){qWarning()<<"propertystr_set:0"<<getstr;return;}
    if(midlist.length()!=1){qWarning()<<"propertystr_set:1"<<getstr;return;}
    QMap<QString,QString> strmap;
    strmap.insert(property2str(Subtype),midlist.first());
    propertymap_set(strmap,false);
    mysk::propertystr_set(frlist,true);
    mysk::propertystr_set(bklist,false);
}

QStringList myvs::trans(){
    QStringList strlist;
    strlist<<name+"=sgs.CreateViewAsSkill{";
    strlist<<QString("name=\"%1\",").arg(name);
    strlist<<QString("n=%1,").arg(vsn);
    myblock *pt;
    pt=findBlockByName(myobj::enumstr(metaObject(),"vsbType",ViewFilter));
    if(pt&&!pt->blocklist.isEmpty()){
        strlist<<"view_filter=function(self,selected,to_select)";
        strlist<<mycode::myindent(trans4avlobjlist(myobj::enumstr(metaObject(),"vsbType",ViewFilter)));
        strlist<<pt->trans();
        strlist<<"end,";
    }
    strlist<<"view_as=function(self,cards)";
    if(vsn<10){strlist<<"if #cards~="+QString::number(vsn)+" then return nil end";}
    strlist<<QString("local a_card=sgs.Sanguosha:cloneCard(\"%1\",sgs.Card_SuitToBeDecided,0)").arg(objname_viewas);
    strlist<<"for var=1,#cards,1 do a_card:addSubcard(cards[var]) end";
    strlist<<"a_card:setSkillName(self:objectName())";
    strlist<<"return a_card";
    strlist<<"end,";
    pt=findBlockByName(myobj::enumstr(metaObject(),"vsbType",EnabledAtPlay));
    if(pt&&!pt->blocklist.isEmpty()){
        strlist<<"enabled_at_play=function(self,player)";
        strlist<<mycode::myindent(trans4avlobjlist(myobj::enumstr(metaObject(),"vsbType",EnabledAtPlay)));
        strlist<<pt->trans();
        strlist<<"end,";
    }
    pt=findBlockByName(myobj::enumstr(metaObject(),"vsbType",EnabledAtResponse));
    if(pt&&!pt->blocklist.isEmpty()){
        strlist<<"enabled_at_response=function(self,player,pattern)";
        strlist<<mycode::myindent(trans4avlobjlist(myobj::enumstr(metaObject(),"vsbType",EnabledAtResponse)));
        strlist<<pt->trans();
        strlist<<"end,";
    }
    if(objname_viewas==myobj::objname_nullification()){
        strlist<<"enabled_at_nullification=function(self,player)";
        strlist<<"return true";
        strlist<<"end,";
    }
    strlist<<"}";
    return strlist;
}

myblock *myvs::iniBlock(QString getstr){
    return mysk::iniBlock(getstr);
}

QString myvs::findRemarkByName_event(QString getname){
    if(getname==myobj::enumstr(metaObject(),"vsbType",ViewFilter)){return getname;}
    if(getname==myobj::enumstr(metaObject(),"vsbType",EnabledAtPlay)){return getname;}
    if(getname==myobj::enumstr(metaObject(),"vsbType",EnabledAtResponse)){return getname;}
    return QString();
}
void myvs::propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap,bool b4remark){
    mysk::propertymap_get(strmap,strlistmap,b4remark);
    strmap.insert(property2str(CardsNum),QString::number(vsn));
    if(b4remark){
        strmap.insert(property2str(CardViewAs),myobj::name2remark(objname_viewas));
        strlistmap.insert(property2str(CardViewAs),myobj::getconstrmlist_tag("ob"));
    }
    else{
        strmap.insert(property2str(CardViewAs),objname_viewas);
        strlistmap.insert(property2str(CardViewAs),myobj::getconstlist_tag("ob"));
    }
}
void myvs::propertymap_set(QMap<QString, QString> &strmap,bool b4remark){
    if(strmap.contains(property2str(CardsNum))){
        bool b;
        int getn=strmap.value(property2str(CardsNum)).toInt(&b);
        if(b){vsn=getn;}
    }
    if(strmap.contains(property2str(CardViewAs))){
        QString getstr=strmap.value(property2str(CardViewAs));
        if(b4remark){objname_viewas=myobj::remark2name(getstr);}
        else{objname_viewas=getstr;}
    }
    mysk::propertymap_set(strmap,b4remark);
}
QString myvs::propertystr_get(){
    QStringList strlist;
    strlist<<mysk::propertystr_get(true);
    strlist<<QString::number(vsn);
    strlist<<objname_viewas;
    strlist<<mysk::propertystr_get(false);
    return strlist.join("|");
}
void myvs::propertystr_set(QString getstr){
    QStringList frlist,bklist,midlist;
    if(!propertystr_dvd(getstr,frlist,midlist,bklist)){qWarning()<<"propertystr_set:0"<<getstr;return;}
    if(midlist.length()!=2){qWarning()<<"propertystr_set:2"<<getstr;return;}
    QMap<QString,QString> strmap;
    strmap.insert(property2str(CardsNum),midlist.at(0));
    strmap.insert(property2str(CardViewAs),midlist.at(1));
    propertymap_set(strmap,false);
    mysk::propertystr_set(frlist,true);
    mysk::propertystr_set(bklist,false);
}

QStringList mydts::trans(){
    QStringList strlist;
    strlist<<name+"=sgs.CreateDistanceSkill{";
    strlist<<QString("name=\"%1\",").arg(name);
    strlist<<"correct_func=function(self,from,to)";
    QString bname=myobj::enumstr(metaObject(),"dtsbType",CorrectFunc);
    myblock *pt=findBlockByName(bname);
    if(pt&&!pt->blocklist.isEmpty()){
        strlist<<mycode::myindent(trans4avlobjlist(bname));
        strlist<<pt->trans();
    }
    strlist<<"return 0";
    strlist<<"end,";
    strlist<<"}";
    return strlist;
}

QStringList myfts::trans(){
    QStringList strlist;
    strlist<<name+"=sgs.CreateFilterSkill{";
    strlist<<QString("name=\"%1\",").arg(name);
    strlist<<"view_filter=function(self,to_select)";
    QString bname=myobj::enumstr(metaObject(),"ftsbType",ViewFilter);
    myblock *pt=findBlockByName(bname);
    if(pt&&!pt->blocklist.isEmpty()){
        QStringList tstrlist;
        tstrlist<<"local room=sgs.Sanguosha:currentRoom()";
        tstrlist<<trans4avlobjlist(bname);
        strlist<<mycode::myindent(tstrlist);
        strlist<<pt->trans();
    }
    strlist<<"return false";
    strlist<<"end,";
    strlist<<"view_as=function(self,originalCard)";
    strlist<<QString("local filtered=sgs.Sanguosha:cloneCard(\"%1\",sgs.Card_SuitToBeDecided,0)").arg(objname_viewas);
    strlist<<"filtered:setSkillName(self:objectName())";
    strlist<<"local card=sgs.Sanguosha:getWrappedCard(originalCard:getId())";
    strlist<<"card:takeOver(filtered)";
    strlist<<"return card";
    strlist<<"end,";
    strlist<<"}";
    return strlist;
}

/*
void mytrs::addFunction(QString geteventstr,QString getblockstr,
                   QString getfunstr ,QList<myobj *> &getobjlist,
                   QStringList &getrtrmlist, QStringList &getblrmlist,bool b4redo){
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
    if(!b4redo){undostrlist.clear();}
    //rfrObj();
    //addusdobjlist(getobjlist);
    sig_update();
}

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
