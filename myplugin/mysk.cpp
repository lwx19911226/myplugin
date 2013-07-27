#include "mysk.h"
#include "mysys.h"
int mysk::globalint=0;
void str2first(QStringList &strlist,QString str){
    if(!strlist.removeOne(str)){qWarning()<<"str2first"<<str;return;}
    strlist.prepend(str);
}


QString mysk::getOwnerProperty(){
    QStringList strlist=getsys()->getgstrlist();
    strlist.prepend(mygeneral::nullname());
    QString str=owner?owner->name:mygeneral::nullname();
    str2first(strlist,str);
    return strlist.join("|");
}
void mysk::setOwnerProperty(QString getstr){setOwner(getsys()->findGeneralByName(getstr));}

void mysk::propertymap_get(QMap<QString, QString> &strmap,bool b4remark){
    const QMetaObject *mob=metaObject();
    if(b4remark){}
    for(int i=mysk::metaObject()->propertyOffset();i<mob->propertyCount();i++){
        QString tstr(mob->property(i).name());
        if(!tstr.endsWith("Property")){
            if(!tstr.endsWith("PropertyRemark")){qWarning()<<"propertymap_get"<<tstr;}
            continue;
        }
        int tgti=i;
        if(b4remark){
            int geti=mob->indexOfProperty(QString(tstr+"Remark").toUtf8());
            if(geti!=-1){tgti=geti;}
        }
        tstr=property2str(str2property(tstr.mid(0,tstr.length()-8)));
        QString vstr=mob->property(tgti).read(this).toString();
        if(vstr.contains(",,")){
            foreach(QString stri,vstr.split(",,")){strmap.insertMulti(tstr,stri);}
        }
        else{strmap.insert(tstr,vstr);}
    }
/*
    strmap.insert(property2str(Name),property("NameProperty").toString());
    strmap.insert(property2str(Translation),property("TranslationProperty").toString());
    strmap.insert(property2str(Owner),owner?owner->name:mygeneral::nullname());
    QStringList gstrlist=getsys()->getgstrlist();
    gstrlist.prepend(mygeneral::nullname());
    //strlistmap.insert(property2str(Owner),gstrlist);
    strmap.insert(property2str(Description),description);
    foreach(QString stri,wordslist){
        strmap.insertMulti(property2str(Words),stri);
    }
    */
}
void mysk::propertymap_set(QMap<QString, QString> &strmap,bool b4remark){
    const QMetaObject *mob=metaObject();
    if(b4remark){}
    for(int i=mysk::metaObject()->propertyOffset();i<mob->propertyCount();i++){
        QString tstr(mob->property(i).name());
        if(!tstr.endsWith("Property")){
            if(!tstr.endsWith("PropertyRemark")){qWarning()<<"propertymap_set"<<tstr;}
            continue;
        }
        int tgti=i;
        if(b4remark){
            int geti=mob->indexOfProperty(QString(tstr+"Remark").toUtf8());
            if(geti!=-1){tgti=geti;}
        }
        tstr=property2str(str2property(tstr.mid(0,tstr.length()-8)));
        if(strmap.contains(tstr)){mob->property(tgti).write(this,QVariant(strmap.value(tstr)));}
    }
    getsys()->sig_update();
    /*
    if(strmap.contains(property2str(Name))){setProperty("NameProperty",QVariant(strmap.value(property2str(Name))));}
        //setName(strmap.value(property2str(Name)));}
    if(strmap.contains(property2str(Translation))){setProperty("TranslationProperty",QVariant(strmap.value(property2str(Translation))));}
        //translation=strmap.value(property2str(Translation));}
    if(strmap.contains(property2str(Owner))){
        setOwner(getsys()->findGeneralByName(strmap.value(property2str(Owner))));
    }
    if(strmap.contains(property2str(Description))){description=strmap.value(property2str(Description));}
    if(strmap.contains(property2str(Words))){
        wordslist.clear();
        wordslist.append(strmap.values(property2str(Words)));
    }
    */
}

QString mysk::propertystr_get(){
    const QMetaObject *mob0=mysk::metaObject();
    const QMetaObject *mob=metaObject();
    QStringList strlist,strlist_bk;
    strlist<<type2abb(getType());
    for(int i=mob0->propertyOffset();i<mob0->propertyCount();i++){
        QString tstr(mob->property(i).name());
        if(!tstr.endsWith("Property")){
            if(!tstr.endsWith("PropertyRemark")){qWarning()<<"propertymap_set"<<tstr;}
            continue;
        }
        tstr=property2str(str2property(tstr.mid(0,tstr.length()-8)));
        int getproperty=str2property(tstr);
        QString vstr=mob->property(i).read(this).toString();
        QString vtstr=vstr;
        vtstr.replace("\\|","\\\\");
        if(vtstr.contains("|")){vstr=vtstr.split("|").first().replace("\\\\","\\|");}
        QStringList tstrlist=mycode::mymdf(vstr.split(",,"),property2prefix(getproperty));
        if(getproperty<Description){strlist<<tstrlist;}
        else{strlist_bk<<tstrlist;}
    }
    for(int i=mob->propertyOffset();i<mob->propertyCount();i++){
        QString tstr(mob->property(i).name());
        if(!tstr.endsWith("Property")){
            if(!tstr.endsWith("PropertyRemark")){qWarning()<<"propertymap_set"<<tstr;}
            continue;
        }
        tstr=property2str(str2property(tstr.mid(0,tstr.length()-8)));
        int getproperty=str2property(tstr);
        QString vstr=mob->property(i).read(this).toString();
        QString vtstr=vstr;
        vtstr.replace("\\|","\\\\");
        if(vtstr.contains("|")){vstr=vtstr.split("|").first().replace("\\\\","\\|");}
        strlist<<mycode::mymdf(vstr.split(",,"),property2prefix(getproperty));
    }
    strlist<<strlist_bk;
    return strlist.join("|");
}

void mysk::propertystr_set(QString getstr){
    const QMetaObject *mob0=mysk::metaObject();
    const QMetaObject *mob=metaObject();
    QString getabb;
    QStringList frlist,midlist,bklist;
    if(!propertystr_dvd(getstr,getabb,frlist,midlist,bklist)){qWarning()<<"propertystr_set:"<<getstr;return;}
    QList<int> tgtilist0,tgtilist1;
    for(int i=mob0->propertyOffset();i<mob0->propertyCount();i++){
        QString tstr(mob->property(i).name());
        if(!tstr.endsWith("Property")){
            if(!tstr.endsWith("PropertyRemark")){qWarning()<<"propertymap_set"<<tstr;}
            continue;
        }
        //tstr=property2str(str2property(tstr.mid(0,tstr.length()-8)));
        tgtilist0<<i;
    }
    for(int i=mob->propertyOffset();i<mob->propertyCount();i++){
        QString tstr(mob->property(i).name());
        if(!tstr.endsWith("Property")){
            if(!tstr.endsWith("PropertyRemark")){qWarning()<<"propertymap_set"<<tstr;}
            continue;
        }
        //tstr=property2str(str2property(tstr.mid(0,tstr.length()-8)));
        tgtilist1<<i;
    }
    for(int pi=0;pi<frlist.length();pi++){mob->property(tgtilist0.at(pi)).write(this,QVariant(frlist.at(pi).mid(property2prefix(pi).length())));}
    for(int pi=0;pi<midlist.length();pi++){mob->property(tgtilist1.at(pi)).write(this,QVariant(midlist.at(pi).mid(property2prefix(pi).length())));}
    QStringList tstrlist;
    foreach(QString stri,bklist){
        if(stri.startsWith(property2prefix(Description))){mob->property(tgtilist0.at(Description)).write(this,QVariant(stri.mid(property2prefix(Description).length())));}
        if(stri.startsWith(property2prefix(Words))){
            tstrlist<<stri.mid(property2prefix(Words).length());
        }
    }
    mob->property(tgtilist0.at(Words)).write(this,QVariant(tstrlist.join(",,")));
}
/*
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
        QStringList wdslist;
        foreach(QString stri,getstrlist){
            if(stri.startsWith(property2prefix(Description))){
                strmap.insert(property2str(Description),stri.mid(property2prefix(Description).length()));
            }
            if(stri.startsWith(property2prefix(Words))){
                wdslist<<stri.mid(property2prefix(Words).length());
                //strmap.insertMulti(property2str(Words),stri.mid(property2prefix(Words).length()));
            }
        }
        strmap.insert(property2str(Words),wdslist.join(",,"));
    }
    propertymap_set(strmap,false);
}
*/
bool mysk::propertystr_dvd(QString getstr,QString &getabb,QStringList &frlist, QStringList &midlist, QStringList &bklist){
    QStringList strlist=mycode::mysplit(getstr,"|",true);
    getabb=strlist.takeFirst();
    int i1,i2;
    for(i2=strlist.length()-1;i2>=0;i2--){
        bklist.prepend(strlist.at(i2));
        if(strlist.at(i2).startsWith(property2prefix(Description))){break;}
    }
    for(i1=0;i1<Description;i1++){
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
    return myobj::enumstrlist(metaObject(),enumname_btype(getType()).toUtf8());
}
/*
QString mysk::bstr2abb(QString getstr){
    if(myobj::enumcontains(metaObject(),enumname_btype(getType()).toUtf8(),getstr)){
        return myobj::enumstr(metaObject(),enumname_babb(getType()).toUtf8(),myobj::enumint(metaObject(),enumname_btype(getType()).toUtf8(),getstr));
    }
    else{return QString();}
}
*/
void mysk::iniObj(){
    const QMetaObject *mob=metaObject();
    QList<int> list;
    QString btype=enumname_btype(getType());
    myobj::enumintlist(mob,btype.toUtf8(),list);
    foreach(int geti,list){
        QString abb=myobj::enumstr(mob,enumname_babb(getType()).toUtf8(),geti);
        if(abb==""){continue;}
        QList<myobj *> tobjlist;
        myobj::newConst(tobjlist,parent(),getsys()->qsv,abb,true);
        foreach(myobj *ip,tobjlist){ip->blockstr=myobj::enumstr(mob,btype.toUtf8(),geti);}
        avlobjlist<<tobjlist;
    }
    myobj::newConst(avlobjlist,parent(),getsys()->qsv,"");
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
    const QMetaObject *mob=metaObject();
    int btype=myobj::enumint(mob,enumname_btype(getType()).toUtf8(),getstr);
    QString abbstr=myobj::enumstr(mob,enumname_babb(getType()).toUtf8(),btype);
    return trans4avlobjlist(getstr,abbstr);
}
QStringList mysk::trans4avlobjlist(QString getstr, QString abbstr){
    QStringList strlist;
    foreach(myobj *ip,avlobjlist){
        if(ip->blockstr!=getstr){continue;}
        if(ip->noDeclaration){continue;}
        QString getconststr=myobj::isConst(ip->name,abbstr);
        if(getconststr!=""){
            strlist<<myobj::transConst(getconststr);
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
                       QStringList &getrtrmlist, QStringList &getblrmlist,
                       bool b4redo, QString dotag){
    myfunction *pfunc=new myfunction(this);
    pfunc->funname=getfunstr;
    pfunc->objlist<<getobjlist;
    //pfunc->inilist<<QVariant(geteventstr)<<QVariant(getblockstr);
    pfunc->myini(geteventstr,getblockstr,getrtrmlist,getblrmlist);
    foreach(myobj *ip,pfunc->rtobjlist){avlobjlist.prepend(ip);}
    iniBlock(geteventstr)->addBlock(pfunc,getblockstr);

    mydo *pdo=new mydo(this);
    pdo->psktgt=this;
    pdo->tagstr=dotag;
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
QString mysk::findRemarkByName_block(QString getname){
    if(eventstrlist().contains(getname)){return tr("Default Block");}
    QString tstr="";
    myblock *pt=findBlockByName(getname);
    if(pt){tstr+=pt->getRemark();}
    else{qWarning()<<"findremarkbyname_block"<<getname;}
    return tstr;
}
QString mysk::findRemarkByName_obj(QString getname,QString getstr,bool b4func){
    myobj *pt=findObjByName(getname,getstr);
    if(!pt){qWarning()<<"findremarkbyname_obj"<<getname;return QString();}
    myfunction *pfunc=getsys()->findFuncByObj(pt);
    return findRemarkByName_obj(pt,pfunc,b4func);
}
QString mysk::findRemarkByName_obj(myobj *tgtobj, myfunction *pfunc, bool b4func){
    QString tstr=tgtobj->remark;
    if(!pfunc){return tstr;}
    if(!pfunc->rtobjlist.contains(tgtobj)){qWarning()<<"findremarkbyname_obj"<<tgtobj->name<<pfunc->name;}
    if(tstr==""){
        tstr=pfunc->funname+myfunction::str4returnvalue()+QString::number(pfunc->rtobjlist.indexOf(tgtobj)+1);
    }
    if(b4func){
        tstr+="  /from: "+pfunc->getRemark();
    }
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
void mysk::dotrans(QString getstr, QString dotag){
    qWarning()<<"dotrans"<<name<<getstr<<dotag;
    //QString tstr=getstr;
    //tstr.replace("\\|","\\\\");
    //QStringList strlist=tstr.split("|");
    //strlist.replaceInStrings("\\\\","|");
    QStringList strlist=mycode::mysplit(getstr,"|");
    QString geteventstr=strlist.first();
    QString getblockstr,tstr;
    int index=-1;
    mydo *tdo=NULL;
    if(eventstrlist().contains(strlist.at(1))){getblockstr=strlist.at(1);}
    else{
        tstr=strlist.at(1);
        index=tstr.indexOf("->");
        if(index==-1){qWarning()<<"dotrans0727"<<getstr;}
        tdo=getsys()->findDoByTag(tstr.mid(0,index));
        if(!tdo){qWarning()<<"dotrans0727"<<getstr;}
        int index2=tstr.mid(index+2).toInt();
        getblockstr=tdo->getBlockName(index2);
    }
    QString getfunstr=strlist.at(2);
    QList<myobj *> getobjlist;
    QStringList tstrlist=mycode::mysplit(strlist.at(3),",");
    myobj *pobj=NULL;
    if(strlist.at(3).isEmpty()){tstrlist.clear();}
    for(int i=0;i<tstrlist.length();i++){        
        tstr=tstrlist.at(i);
        if(tstr.contains("->")){
            index=tstr.indexOf("->");
            tdo=getsys()->findDoByTag(tstr.mid(0,index));
            if(!tdo){qWarning()<<"dotrans0727"<<getstr;}
            int index2=tstr.mid(index+2).toInt();
            pobj=tdo->getObj(index2);
        }
        else{
            pobj=findObjByName(tstr,geteventstr);
            if(!pobj){
                pobj=new myobj(getsys());
                pobj->name=tstr;
                int gettype=myobj::str2type(myfun::intypestrlist(getfunstr,getsys()->qsv).at(i));
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
    addFunction(geteventstr,getblockstr,getfunstr,getobjlist,getrtrmlist,getblrmlist,true,dotag);
}
mysys *mysk::getsys(){
    return static_cast<mysys *>(parent());
}
void mysk::sig_update(){
    getsys()->sig_update();
}

void mytrs::iniObj(){
    //QStringList strlist;
    //strlist<<"trs";
    myobj::newConst(avlobjlist,parent(),getsys()->qsv,"trs");
    foreach(QString stri,myevent::geteventstrlist(getsys()->qsv)){
        myevent::getavlobjlist(stri,avlobjlist,parent());
    }
}
QStringList mytrs::trans(QStringList &back){
    QStringList strlist;
    strlist<<name+"=sgs.CreateTriggerSkill{"<<QString("name=\"%1\",").arg(name);
    QStringList tstrlist;
    foreach(myblock *ip,blocklist){tstrlist<<myevent::trans(ip->name);}
    strlist<<QString("events={%1},").arg(tstrlist.join(","));
    strlist<<QString("frequency=%1,").arg(type2trans(subtype));
    strlist<<"on_trigger=function(self,event,player,data)";
    strlist<<"local room=player:getRoom()";
    strlist<<default4skname();
    strlist<<trans4avlobjlist("");
    foreach(myblock *ip,blocklist){
        QStringList blockstrlist;
        blockstrlist<<QString("if event==%1 then").arg(myevent::trans(ip->name));
        blockstrlist<<mycode::myindent(myevent::trans4eventdata(ip->name,true));
        blockstrlist<<mycode::myindent(trans4avlobjlist(ip->name));
        blockstrlist<<ip->trans(back);
        blockstrlist<<mycode::myindent(myevent::trans4eventdata(ip->name,false));
        blockstrlist<<"end";
        strlist<<mycode::myindent(blockstrlist);
    }
    strlist<<"end,";
    strlist<<"can_trigger=function(self,player)";
    strlist<<"local room=player:getRoom()";
    switch(cantrigger){
    case SkillOwnerAlive:
        strlist<<"local selfplayer=room:findPlayerBySkillName(self:objectName())";
        strlist<<"return selfplayer and selfplayer:isAlive()";
        break;
    case TriggerAlive:strlist<<"return player and player:isAlive()";break;
    case Always:strlist<<"return true";break;
    default:qWarning()<<"trs cantrigger";
    }
    strlist<<"end,\n}";
    return strlist;
}
QStringList mytrs::trans4avlobjlist(QString getstr){
    return mysk::trans4avlobjlist(getstr,"trs");
}
QStringList mytrs::eventstrlist(){
    QStringList strlist=myevent::geteventstrlist(getsys()->qsv);
    //if(cantrigger==UserDefined){strlist.prepend("CanTrigger");}
    return strlist;
}
QString mytrs::findRemarkByName_event(QString getname){
/*
    if(getname=="CanTrigger"){
        return tr("We use this function to check if the triggerskill can be triggered.")
                +tr("Return true means it can be triggered.")
                +tr("For example, Fankui can be triggered when the trigger is the skill owner and he is alive;")
                +tr("Guicai can be triggered when the skill owner is alive but the trigger may be someone else.");
    }
*/
    return myevent::findRemarkByName(getname);
}
QString mytrs::getSubtypeProperty(){
    QStringList strlist=subtypestrlist();
    QString str=subtype2str(subtype);
    str2first(strlist,str);
    return strlist.join("|");
}
void mytrs::setSubtypeProperty(QString getstr){subtype=str2subtype(getstr);}
QString mytrs::getCanTriggerProperty(){
    QStringList strlist=cantriggerstrlist();
    QString str=cantrigger2str(cantrigger);
    str2first(strlist,str);
    return strlist.join("|");
}
void mytrs::setCanTriggerProperty(QString getstr){cantrigger=str2cantrigger(getstr);}
QString mytrs::getCanTriggerPropertyRemark(){
    return getCanTriggerProperty();
}
void mytrs::setCanTriggerPropertyRemark(QString getstr){setCanTriggerProperty(getstr);}

/*
void mytrs::propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap,bool b4remark){
    mysk::propertymap_get(strmap,b4remark);
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
*/
QStringList myvs::trans(QStringList &back){
    const QMetaObject *mob=metaObject();
    QStringList strlist,tstrlist;
    myblock *pt;
    QString bname;
    if(objname_viewas=="SkillCard"){
        strlist<<name+"_card=sgs.CreateSkillCard{";
        strlist<<QString("name=\"%1\",").arg(name);
        strlist<<"target_fixed=false,";
        strlist<<"will_throw=false,";
        bname=myobj::enumstr(mob,"vsbType",PlayerFilter);
        pt=findBlockByName(bname);
        if(pt&&!pt->blocklist.isEmpty()){
            strlist<<"filter=function(self,targets,to_select)";
            tstrlist.clear();
            tstrlist<<QString("local selfskname=\"%1\"").arg(name);
            tstrlist<<trans4avlobjlist(bname);
            strlist<<mycode::myindent(tstrlist);
            strlist<<pt->trans(back);
            strlist<<"return false end,";
        }
        else{}
        if(tgtn!=1){
            strlist<<QString("feasible=function(self,targets) return (#targets>=%1) end,").arg(tgtn);
        }
        bname=myobj::enumstr(mob,"vsbType",SkillCardUse);
        pt=findBlockByName(bname);
        if(pt&&!pt->blocklist.isEmpty()){
            strlist<<"on_use=function(self,room,source,targets)";
            tstrlist.clear();
            tstrlist<<QString("local selfskname=\"%1\"").arg(name);
            tstrlist<<trans4avlobjlist(bname);
            strlist<<mycode::myindent(tstrlist);
            strlist<<pt->trans(back);
            strlist<<"end,";
        }
        else{}
        strlist<<"}";
    }
    strlist<<name+"=sgs.CreateViewAsSkill{";
    strlist<<QString("name=\"%1\",").arg(name);
    strlist<<QString("n=%1,").arg(vsn);
    bname=myobj::enumstr(mob,"vsbType",ViewFilter);
    pt=findBlockByName(bname);
    if(pt&&!pt->blocklist.isEmpty()){
        strlist<<"view_filter=function(self,selected,to_select)";
        tstrlist.clear();
        tstrlist<<default4skname();
        tstrlist<<trans4avlobjlist(bname);
        strlist<<mycode::myindent(tstrlist);
        strlist<<pt->trans(back);
        strlist<<"return false end,";
    }
    strlist<<"view_as=function(self,cards)";
    if(vsn<10){strlist<<"if #cards~="+QString::number(vsn)+" then return nil end";}
    QString tstr;
    if(objname_viewas=="SkillCard"){tstr=QString("local a_card=%1_card:clone()").arg(name);}
    else{tstr=QString("local a_card=sgs.Sanguosha:cloneCard(\"%1\",sgs.Card_SuitToBeDecided,0)").arg(objname_viewas);}
    strlist<<tstr;
    strlist<<"for var=1,#cards,1 do a_card:addSubcard(cards[var]) end";
    strlist<<"a_card:setSkillName(self:objectName())";
    strlist<<"return a_card";
    strlist<<"end,";
    bname=myobj::enumstr(mob,"vsbType",EnabledAtPlay);
    pt=findBlockByName(bname);
    if(pt&&!pt->blocklist.isEmpty()){
        strlist<<"enabled_at_play=function(self,player)";
        tstrlist.clear();
        tstrlist<<default4skname();
        tstrlist<<trans4avlobjlist(bname);
        strlist<<mycode::myindent(tstrlist);
        strlist<<pt->trans(back);
        strlist<<"return false end,";
    }
    bname=myobj::enumstr(mob,"vsbType",EnabledAtResponse);
    pt=findBlockByName(bname);
    if(pt&&!pt->blocklist.isEmpty()){
        strlist<<"enabled_at_response=function(self,player,pattern)";
        tstrlist.clear();
        tstrlist<<default4skname();
        tstrlist<<trans4avlobjlist(bname);
        strlist<<mycode::myindent(tstrlist);
        strlist<<pt->trans(back);
        strlist<<"return false end,";
    }
    if(objname_viewas==myobj::objname_nullification()){
        strlist<<"enabled_at_nullification=function(self,player) return true end,";
        //strlist<<"return true";
        //strlist<<"end,";
    }
    strlist<<"}";
    return strlist;
}

myblock *myvs::iniBlock(QString getstr){
    return mysk::iniBlock(getstr);
}

QString myvs::getCardsNumProperty(){return QString::number(vsn);}
void myvs::setCardsNumProperty(QString getstr){
    bool b;
    int getn=getstr.toInt(&b);
    if(b){vsn=getn;}
}
QString myvs::getTargetPlayersNumProperty(){return QString::number(tgtn);}
void myvs::setTargetPlayersNumProperty(QString getstr){
    bool b;
    int getn=getstr.toInt(&b);
    if(b){tgtn=getn;}
}
QString myvs::getCardViewAsProperty(){
    QStringList strlist=myobj::getconstlist_tag("ob");
    strlist.prepend("SkillCard");
    str2first(strlist,objname_viewas);
    return strlist.join("|");
}
void myvs::setCardViewAsProperty(QString getstr){objname_viewas=getstr;}
QString myvs::getCardViewAsPropertyRemark(){
    QStringList strlist=myobj::getconstrmlist_tag("ob");
    strlist.prepend(tr("SkillCard"));
    QString tstr;
    if(objname_viewas=="SkillCard"){tstr=tr("SkillCard");}
    else{tstr=myobj::name2remark(objname_viewas);}
    str2first(strlist,tstr);
    return strlist.join("|");
}
void myvs::setCardViewAsPropertyRemark(QString getstr){
    if(getstr==tr("SkillCard")){objname_viewas="SkillCard";}
    else{objname_viewas=myobj::remark2name(getstr);}
}
/*
void myvs::propertymap_get(QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap,bool b4remark){
    mysk::propertymap_get(strmap,b4remark);
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
*/
QStringList mydts::trans(QStringList &back){
    QStringList strlist,tstrlist;
    strlist<<name+"=sgs.CreateDistanceSkill{";
    strlist<<QString("name=\"%1\",").arg(name);
    strlist<<"correct_func=function(self,from,to)";
    QString bname=myobj::enumstr(metaObject(),"dtsbType",CorrectFunc);
    myblock *pt=findBlockByName(bname);
    if(pt&&!pt->blocklist.isEmpty()){
        tstrlist.clear();
        tstrlist<<default4skname();
        tstrlist<<trans4avlobjlist(bname);
        strlist<<mycode::myindent(tstrlist);
        strlist<<pt->trans(back);
    }
    strlist<<"return 0";
    strlist<<"end,";
    strlist<<"}";
    return strlist;
}

QStringList myfts::trans(QStringList &back){
    QStringList strlist;
    strlist<<name+"=sgs.CreateFilterSkill{";
    strlist<<QString("name=\"%1\",").arg(name);
    strlist<<"view_filter=function(self,to_select)";
    QString bname=myobj::enumstr(metaObject(),"ftsbType",ViewFilter);
    myblock *pt=findBlockByName(bname);
    if(pt&&!pt->blocklist.isEmpty()){
        QStringList tstrlist;
        tstrlist<<"local room=sgs.Sanguosha:currentRoom()";
        tstrlist<<default4skname();
        tstrlist<<trans4avlobjlist(bname);
        strlist<<mycode::myindent(tstrlist);
        strlist<<pt->trans(back);
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
QString myfts::getCardViewAsProperty(){
    QStringList strlist=myobj::getconstlist_tag("ob");
    str2first(strlist,objname_viewas);
    return strlist.join("|");
}
void myfts::setCardViewAsProperty(QString getstr){objname_viewas=getstr;}
QString myfts::getCardViewAsPropertyRemark(){
    QStringList strlist=myobj::getconstrmlist_tag("ob");
    str2first(strlist,myobj::name2remark(objname_viewas));
    return strlist.join("|");
}
void myfts::setCardViewAsPropertyRemark(QString getstr){objname_viewas=myobj::remark2name(getstr);}

QStringList myprs::trans(QStringList &back){
    QStringList strlist,tstrlist;
    strlist<<name+"=sgs.CreateProhibitSkill{";
    strlist<<QString("name=\"%1\",").arg(name);
    strlist<<"is_prohibited=function(self,from,to,card)";
    QString bname=myobj::enumstr(metaObject(),"prsbType",IsProhibited);
    myblock *pt=findBlockByName(bname);
    if(pt&&!pt->blocklist.isEmpty()){
        tstrlist.clear();
        tstrlist<<default4skname();
        tstrlist<<trans4avlobjlist(bname);
        strlist<<mycode::myindent(tstrlist);
        strlist<<pt->trans(back);
    }
    else{strlist<<"return false";}
    strlist<<"end,";
    strlist<<"}";
    return strlist;
}

QStringList mymcs::trans(QStringList &back){
    QStringList strlist,tstrlist;
    strlist<<name+"=sgs.CreateMaxCardsSkill{";
    strlist<<QString("name=\"%1\",").arg(name);
    strlist<<"extra_func=function(self,target)";
    QString bname=myobj::enumstr(metaObject(),"mcsbType",ExtraFunc);
    myblock *pt=findBlockByName(bname);
    if(pt&&!pt->blocklist.isEmpty()){
        tstrlist.clear();
        tstrlist<<default4skname();
        tstrlist<<trans4avlobjlist(bname);
        strlist<<mycode::myindent(tstrlist);
        strlist<<pt->trans(back);
    }
    else{strlist<<"return 0";}
    strlist<<"end,";
    strlist<<"}";
    return strlist;
}

QStringList mytms::trans(QStringList &back){
    const QMetaObject *mob=metaObject();
    QStringList strlist,tstrlist;
    strlist<<name+"=sgs.CreateTargetModSkill{";
    strlist<<QString("name=\"%1\",").arg(name);
    strlist<<QString("pattern=\"%1\",").arg(pattern);
    myblock *pt;
    QString bname;
    bname=myobj::enumstr(mob,"tmsbType",ExtraTarget);
    pt=findBlockByName(bname);
    if(pt&&!pt->blocklist.isEmpty()){
        strlist<<"extra_target_func=function(self,player)";
        tstrlist.clear();
        tstrlist<<default4skname();
        tstrlist<<trans4avlobjlist(bname);
        strlist<<mycode::myindent(tstrlist);
        strlist<<pt->trans(back);
        strlist<<"end,";
    }
    bname=myobj::enumstr(mob,"tmsbType",DistanceLimit);
    pt=findBlockByName(bname);
    if(pt&&!pt->blocklist.isEmpty()){
        strlist<<"distance_limit_func=function(self,player)";
        tstrlist.clear();
        tstrlist<<default4skname();
        tstrlist<<trans4avlobjlist(bname);
        strlist<<mycode::myindent(tstrlist);
        strlist<<pt->trans(back);
        strlist<<"end,";
    }
    bname=myobj::enumstr(mob,"tmsbType",Residue);
    pt=findBlockByName(bname);
    if(pt&&!pt->blocklist.isEmpty()){
        strlist<<"residue_func=function(self,player)";
        tstrlist.clear();
        tstrlist<<default4skname();
        tstrlist<<trans4avlobjlist(bname);
        strlist<<mycode::myindent(tstrlist);
        strlist<<pt->trans(back);
        strlist<<"end,";
    }
    strlist<<"}";
    return strlist;
}

QString mytms::getPatternProperty(){
    QString tstr=pattern;
    tstr.replace("|","\\|");
    QStringList tstrlist=myobj::getconstlist_tag("pt");
    tstrlist.removeOne(tstr);
    tstrlist.prepend("");
    tstrlist.prepend(tstr);
    return tstrlist.join("|");
    //return pattern;
}
void mytms::setPatternProperty(QString getstr){
    QString tstr=getstr;
    pattern=tstr.replace("\\|","|");
}
QString mytms::getPatternPropertyRemark(){
    QString tstr=pattern;
    tstr.replace("|","\\|");
    QString trm=myobj::name2remark(tstr);
    if(trm!=""){tstr=trm;}
    QStringList tstrlist=myobj::getconstrmlist_tag("pt");
    tstrlist.removeOne(tstr);
    tstrlist.prepend("");
    tstrlist.prepend(tstr);
    return tstrlist.join("|");
}
void mytms::setPatternPropertyRemark(QString getstr){
    QString tstr=getstr;
    tstr.replace("\\|","|");
    QString tname=myobj::remark2name(tstr);
    if(tname!=""){tstr=tname;}
    pattern=tstr;
}

QString myexs::getskname(){
    if(tgtsk){return (skname=tgtsk->name);}
    else{return skname;}
}
void myexs::setskname(QString getstr){
    tgtsk=getsys()->findSkillByName(getstr);
    if(tgtsk==this){tgtsk=NULL;}
    skname=getstr;
}
QString myexs::getSKNameProperty(){
    QStringList tstrlist;
    QList<mysk *> tsklist;
    getsys()->getsklist_noexs(tsklist);
    foreach(mysk *ip,tsklist){tstrlist<<ip->name;}
    foreach(QString stri,myobj::myconstskstrlist){tstrlist<<stri.split("|").first();}
    str2first(tstrlist,getskname());
    return tstrlist.join("|");
}
void myexs::setSKNameProperty(QString getstr){setskname(getstr);}
QString myexs::getSKNamePropertyRemark(){
    QStringList tstrlist;
    QString tstr;
    if(tgtsk){tstr=tgtsk->name+"\\|"+tgtsk->translation;}
    QList<mysk *> tsklist;
    getsys()->getsklist_noexs(tsklist);
    foreach(mysk *ip,tsklist){tstrlist<<ip->name+"\\|"+ip->translation;}
    foreach(QString stri,myobj::myconstskstrlist){
        QString getname=stri.split("|").first();
        QString gettrans=stri.split("|").at(1);
        tstrlist<<getname+"\\|"+gettrans;
        if(getname==skname){tstr=getname+"\\|"+gettrans;}
    }
    str2first(tstrlist,tstr);
    return tstrlist.join("|");
}
void myexs::setSKNamePropertyRemark(QString getstr){
    QList<mysk *> tsklist;
    getsys()->getsklist_noexs(tsklist);
    foreach(mysk *ip,tsklist){
        if(getstr.split("|").first()==ip->name){setskname(ip->name);return;}
    }
    foreach(QString stri,myobj::myconstskstrlist){
        QString getname=stri.split("|").first();
        if(getstr.split("|").first()==getname){setskname(getname);return;}
    }
    qWarning()<<"setsknamepropertyremark"<<getstr;
}
QString myexs::propertystr_get(){
    QStringList strlist;
    strlist<<type2abb(getType());
    strlist<<getName();
    strlist<<getOwnerProperty().split("|").first();
    strlist<<getSKNameProperty().split("|").first();
    return strlist.join("|");
}

void myexs::propertystr_set(QString getstr){
    QStringList tstrlist=getstr.split("|");
    if((tstrlist.length()==4)&&(tstrlist.first()==type2abb(getType()))){
        setName(tstrlist.at(1));
        setOwnerProperty(tstrlist.at(2));
        setSKNameProperty(tstrlist.at(3));
    }
    else{qWarning()<<"propertystr_set"<<getstr;return;}
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
