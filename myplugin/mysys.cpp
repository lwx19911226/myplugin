#include "mysys.h"
int mysys::globalint=0;
void mysys::myini(QString getpath){    
    //funstrlist=myfun::getfunstrlist();
    //eventstrlist=myevent::geteventstrlist();
    myini_design(getpath);
}

void mysys::myini_design(QString path){
    qWarning()<<path;
    QFile fin(path);
    if(!fin.open(QFile::ReadOnly)){
        QMessageBox::warning(NULL,"FILE ERROR","No design");
        qWarning()<<"no"<<path;
        return;
    }
    QTextStream cin(&fin);
    //cin.setCodec("UTF-8");
    QString str;
    QStringList strlist;
    for(;;){
        if(cin.atEnd()){break;}
        str=cin.readLine();
        if(!str.startsWith(">")){continue;}
        strlist<<str;
    }
    QStringList strlist_package=strlist.filter(QRegExp("^>[^>]"));
    QStringList strlist_general=strlist.filter(QRegExp("^>>[^>]"));
    QStringList strlist_trs=strlist.filter(QRegExp("^>>>[^>]"));
    QStringList strlist_do=strlist.filter(QRegExp("^>>>>[^>]"));
    if(strlist_package.length()==1&&strlist_package.at(0).count("|")==1){
        packagename=strlist_package.at(0).mid(1).split("|").at(0);
        package_trans=strlist_package.at(0).mid(1).split("|").at(1);
    }
    else{
        qWarning()<<strlist_package;
        return;
    }
    foreach(QString stri,strlist_general){
        if(stri.count("|")>=4){
            QString tstr=stri.mid(2);
            QString getname=tstr.split("|").at(0);
            mygeneral *pg=findGeneralByName(getname);
            if(!pg){pg=newGeneral(getname);}
            pg->propertystr_set(tstr);
            /*
            QString gettranslation=tstrlist.at(1);
            int getkingdom=mygeneral::str2kingdom(tstrlist.at(2));
            bool getsex=mygeneral::str2sex(tstrlist.at(3));
            int gethp=tstrlist.at(4).toInt();
            if(pg){
                pg->translation=gettranslation;
                pg->kingdom=getkingdom;
                pg->sex=getsex;
                pg->hp=gethp;
            }
            else{
                pg=newGeneral(getname,getkingdom,gethp,getsex);
                pg->translation=gettranslation;
            }
            for(int i=5;i<tstrlist.length();i++){
                if(tstrlist.at(i).startsWith("#")){
                    pg->title=tstrlist.at(i).mid(1);
                }
                else if(tstrlist.at(i).startsWith("~")){
                    pg->word=tstrlist.at(i).mid(1);
                }
                else if(tstrlist.at(i).startsWith("cv:")){
                    pg->cv=tstrlist.at(i).mid(3);
                }
            }
            */
        }
        else{
            qWarning()<<stri;
        }
    }
    foreach(QString stri,strlist_trs){
        if(stri.count("|")>=3){
            QString tstr=stri.mid(3);
            QString getabb=tstr.split("|").at(0);
            QString getname=tstr.split("|").at(1);
            mysk *psk=findSkillByName(getname);
            if(!psk){psk=newSkill(getname,mysk::abb2type(getabb));}
            psk->propertystr_set(tstr);
            /*
            QString gettranslation=tstrlist.at(1);
            int gettype=mytrs::str2type(tstrlist.at(2));
            mygeneral *pg=findGeneralByName(tstrlist.at(3));
            mytrs *psk=static_cast<mytrs *>(findSkillByName(getname));
            if(psk){
                psk->setOwner(pg);
                psk->translation=gettranslation;
                psk->subtype=gettype;
            }
            else{
                psk=newTrs(getname,gettype);
                psk->setOwner(pg);
                psk->translation=gettranslation;
            }
            for(int i=4;i<tstrlist.length();i++){
                if(tstrlist.at(i).startsWith(":")){
                    psk->description=tstrlist.at(i).mid(1);
                }
                else if(tstrlist.at(i).startsWith("$")){
                    psk->wordslist<<tstrlist.at(i).mid(1);
                }
            }
            */
        }
        else{
            qWarning()<<stri;
        }
    }
    foreach(QString stri,strlist_do){
        QString skname=stri.mid(4,stri.indexOf("::")-4);
        QString getstr=stri.mid(stri.indexOf("::")+2);
        //qWarning()<<trsname<<getstr;
        mysk *psk=findSkillByName(skname);
        if(!psk){qWarning()<<"myini_design_psk"<<skname;}
        else{psk->dotrans(getstr);}
    }
    qWarning()<<"design success";
    fin.close();
    sig_update();
}
void mysys::getsklist(QList<mysk *> &rsklist,int gettype){
    foreach(mysk *ip,sklist){
        if(ip->getType()==gettype){rsklist<<ip;}
    }
}
void mysys::getavlobjlist_global(QList<myobj *> &list){
    foreach(mysk *ip,sklist){
        foreach(myobj *pobj,ip->avlobjlist){
            if(pobj->isGlobal){list<<pobj;}
        }
    }
}
mygeneral *mysys::newGeneral(QString getname, int getkingdom, int gethp, bool getsex){
    pg0=new mygeneral(this);
    pg0->setName(getname);
    pg0->kingdom=getkingdom;
    pg0->hp=gethp;
    pg0->sex=getsex;    
    glist.append(pg0);
    sig_update();
    return pg0;
}
mysk *mysys::newsk(int gettype){
    qRegisterMetaType<mytrs>("mytrs");
    qRegisterMetaType<myvs>("myvs");
    qRegisterMetaType<mydts>("mydts");
    qRegisterMetaType<myfts>("myfts");
    qRegisterMetaType<myprs>("myprs");
    qRegisterMetaType<mymcs>("mymcs");
    qRegisterMetaType<mytms>("mytms");
    foreach(QString stri,mysk::typeclasslist()){
        if(QMetaType::type(stri.toUtf8())==0){qWarning()<<"newsk"<<stri;}
    }
    mysk *psk=NULL;
    int id=QMetaType::type(mysk::type2class(gettype).toUtf8());
    if(id!=0){
        psk=static_cast<mysk *>(QMetaType::construct(id));
        psk->setParent(this);
    }
    return psk;
}
mysk *mysys::newSkill(QString getname, int gettype){
    //if(gettype==mysk::TriggerSkill){return newTrs(getname);}
    //if(gettype==mysk::ViewAsSkill){return newVs(getname);}
    mysk *psk=newsk(gettype);
    if(!psk){qWarning()<<"newskill"<<getname<<gettype;}
    psk0=psk;
    psk0->setName(getname);
    psk0->iniObj();
    sklist.append(psk0);
    sig_update();
    return psk0;
}
/*
mytrs *mysys::newTrs(QString getname){
    mytrs *ptrs=new mytrs(this);    
    ptrs->setName(getname);
    ptrs->iniObj();
    psk0=ptrs;
    sklist.append(psk0);
    sig_update();
    return ptrs;
}
myvs *mysys::newVs(QString getname){
    myvs *pvs=new myvs(this);
    pvs->setName(getname);
    pvs->iniObj();
    psk0=pvs;
    sklist.append(psk0);
    sig_update();
    return pvs;
}
*/
mygeneral *mysys::findGeneralByName(QString getname){
    if(getname==mygeneral::nullname()){return NULL;}
    foreach(mygeneral *ip,glist){
        if(ip->name==getname){return ip;}
    }
    return NULL;
}
mysk *mysys::findSkillByName(QString getname){
    foreach(mysk *ip,sklist){
        if(ip->name==getname){return ip;}
    }
    return NULL;
}
bool mysys::delGeneral(mygeneral *getp){
    if(!getp){return false;}
    if(!glist.contains(getp)){return false;}
    foreach(mysk *ip,getp->sklist){ip->owner=NULL;}
    glist.removeOne(getp);
    glist_r.append(getp);
    if(getp==pg0){pg0=(glist.isEmpty()?NULL:glist.first());}
    sig_update();
    return true;
}
bool mysys::delSkill(mysk *getp){
    if(!getp){return false;}
    if(!sklist.contains(getp)){return false;}
    if(getp->owner){getp->owner->sklist.removeOne(getp);}
    sklist.removeOne(getp);
    sklist_r.append(getp);
    QList<mydo *> tdolist;
    foreach(mydo *ip,dolist){
        if(ip->psktgt!=getp){tdolist<<ip;}
        else{dolist_r<<ip;}
    }
    dolist.clear();
    dolist<<tdolist;
    undostrlist.clear();
    if(getp==psk0){        
        QList<mysk *> rsklist;
        getsklist(rsklist,getp->getType());
        psk0=(rsklist.isEmpty()?NULL:rsklist.first());        
    }
    sig_update();
    return true;
}

QStringList mysys::trans(){
    //QString str="";
    QStringList strlist;
    strlist<<QString("module(\"extensions.%1\",package.seeall)\nextension=sgs.Package(\"%1\")").arg(packagename);
    foreach(mygeneral *ip,glist){
        strlist<<ip->trans();
    }
    strlist<<"dofile \"lua/sgs_ex.lua\"";
    foreach(mysk *ip,sklist){
        strlist<<ip->trans();
    }    
    strlist<<"function addsk(sk)"<<"if not sgs.Sanguosha:getSkill(sk:objectName()) then"<<"local sklist=sgs.SkillList()"
          <<"sklist:append(sk)"<<"sgs.Sanguosha:addSkills(sklist)"<<"end"<<"end";
    foreach(mysk *ip,sklist){
        strlist<<QString("addsk(%1)").arg(ip->name);
    }
    foreach(mysk *ip,sklist){
        if(ip->owner){
            strlist<<QString("%1:addSkill(\"%2\")").arg(ip->owner->name).arg(ip->name);
        }
    }    
    strlist<<packagename+"_trans={}";
    strlist<<"function addsktrans(t,name,trans,dscrpt,wds)"<<"t[name]=trans"<<"t[\":\"..name]=dscrpt"
          <<"if wds then"<<"if #wds==1 then t[\"$\"..name]=wds[1]"
         <<"else table.foreach(wds,function(i,v) t[string.format(\"$%s%d\",name,i)]=v end)"<<"end"<<"end"<<"end";
    strlist<<"function addgtrans(t,name,trans,title,wdf,cv)"<<"t[name]=trans"<<"t[\"#\"..name]=title"
          <<"t[\"~\"..name]=wdf"<<"t[\"cv:\"..name]=cv"<<"end";
    strlist<<"function cmpltrans(t)"<<"local key,value"<<"local tmplist4here={}"<<"for key,value in pairs(t) do"
          <<"tmplist4here[key]=value"<<"if string.sub(key,1,1)==\":\" and not t[\"~\"..string.sub(key,2)] then"
         <<"tmplist4here[\"~\"..string.sub(key,2)]=value"<<"end"<<"if string.sub(key,1,1)==\":\" and not t[\"@\"..string.sub(key,2)] then"
        <<"tmplist4here[\"@\"..string.sub(key,2)]=t[string.sub(key,2)]"<<"end"<<"if string.sub(key,1,1)~=\":\" and not t[\":\"..key] then"
       <<"tmplist4here[\"designer:\"..key]=tmplist4here[\"designer:\"..key] or \"\""<<"tmplist4here[\"illustrator:\"..key]=tmplist4here[\"illustrator:\"..key] or \"Internet\""
      <<"end"<<"end"<<"return tmplist4here"<<"end";
    strlist<<QString("%1_trans[\"%1\"]=\"%2\"").arg(packagename).arg(package_trans);
    foreach(mygeneral *ip,glist){
        strlist<<QString("addgtrans(%1_trans,\"%2\",\"%3\",\"%4\",\"%5\",\"%6\")").arg(packagename,ip->name,ip->translation,ip->title,ip->word,ip->cv);
    }
    foreach(mysk *ip,sklist){
        strlist<<QString("addsktrans(%1_trans,\"%2\",\"%3\",\"%4\",{%5})").arg(packagename,ip->name,ip->translation,ip->description,mycode::mymdf(ip->wordslist,QString("\""),QString("\"")).join(","));
    }
    strlist<<QString("sgs.LoadTranslationTable(cmpltrans(%1_trans))").arg(packagename);

    return strlist;
}
QStringList mysys::trans4design(){
    //QString str="";
    QStringList strlist;
    strlist<<QString(">%1|%2").arg(packagename,package_trans);
    foreach(mygeneral *ip,glist){
        strlist<<">>"+ip->propertystr_get();
        //strlist<<QString(">>%1|%2|%3|%4|%5|#%6|~%7|cv:%8").arg(ip->name,ip->translation,mygeneral::kingdom2str(ip->kingdom),mygeneral::sex2str(ip->sex)).arg(ip->hp).arg(ip->title,ip->word,ip->cv);
    }
    QList<mysk *> rsklist;
    getsklist(rsklist,mysk::TriggerSkill);
    foreach(mysk *ip,sklist){
        //mytrs *pt=static_cast<mytrs *>(ip);
        strlist<<">>>"+ip->propertystr_get();
        //QString tstr="";
        //tstr+=QString(">>>%1|%2|%3|%4|:%5").arg(pt->name,pt->translation,mytrs::type2str(pt->subtype),pt->owner?pt->owner->name:"",pt->description);
        //tstr+=mycode::mymdf(pt->wordslist,"|$").join("");
        //strlist<<tstr;
    }
    foreach(mydo *ip,dolist){
        strlist<<">>>>"+ip->trans();
    }
    return strlist;
}

void mysys::undo(){
    if(dolist.isEmpty()){return;}
    undostrlist<<dolist.last()->trans();
    mydo *pdo=dolist.takeLast();
    foreach(myblock *ip,pdo->blocklist){
        bool b=false;
        foreach(myblock *pt,pdo->psktgt->blocklist){
            if(pt->removeBlock(ip)){b=true;break;}
        }
        if(!b){qWarning()<<"removecode:"<<ip->name;}
    }
    foreach(myobj *ip,pdo->objlist){
        if(!pdo->psktgt->avlobjlist.removeOne(ip)){qWarning()<<"removeobj:"<<ip->name;}
    }
    dolist_r.append(pdo);
    psk0=pdo->psktgt;
    sig_update();
}
void mysys::redo(){
    //if(undolist.isEmpty()){return;}
    if(undostrlist.isEmpty()){return;}
    QString str=undostrlist.takeLast();
    QRegExp rx("([^:]*)::([^:]*)");
    if(rx.indexIn(str)!=-1){
        QString skname=rx.cap(1);
        QString tstr=rx.cap(2);qWarning()<<"redo:"<<skname<<tstr;
        mysk *psk=findSkillByName(skname);
        if(!psk){qWarning()<<"redo_psk"<<str;}
        else{
            psk0=psk;
            psk->dotrans(tstr);
        }
    }
    //mydo::dotrans(this,);
    sig_update();
/*
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
*/
}

/*
void mysys::myrun(){
    QTextStream cout(stdout,QIODevice::WriteOnly);
    QTextStream cin(stdin,QIODevice::ReadOnly);
    cout<<"start"<<endl;
    QList<QString> list;
    psk0->need(list);
    QString ch="n";
    QString ch2="y";
    foreach(QString stri,list){
        if(stri=="opr"){
            for(;;){
                cout<<"stc(y) or condition(n)?"<<endl;
                cin>>ch2;
                if(ch2=="y"){
                    QString geteventstr;
                    QString getblockstr;
                    QString getfunstr;
                    QList<myobj *> getobjlist;
                    QList<QString> getrtrmlist;
                    input_opr(geteventstr,getblockstr,getfunstr,getobjlist,getrtrmlist);
                    psk0->addOpr(geteventstr,getblockstr,getfunstr,getobjlist,getrtrmlist);
                }
                else{
                    QString geteventstr;
                    QString getblockstr;
                    QList<myobj *> getobjlist;
                    QString getrm;
                    input_condition(geteventstr,getblockstr,getobjlist,getrm);
                    psk0->addCondition(geteventstr,getblockstr,getobjlist,getrm);
                }
                cout<<"result:"<<endl;
                cout<<psk0->trans()<<endl;
                cout<<"anything else? y/n"<<endl;
                cin>>ch;
                if(ch=="n"){break;}
            }
        }
    }
    cout<<"result:"<<endl;
    cout<<psk0->trans()<<endl;
}

void mysys::input_opr(QString &geteventstr,QString &getblockstr, QString &getfunstr, QList<myobj *> &getobjlist, QList<QString> &getrtrmlist){
    QTextStream cout(stdout,QIODevice::WriteOnly);
    QTextStream cin(stdin,QIODevice::ReadOnly);
    input_event(geteventstr);
    input_block(geteventstr,getblockstr);
    input_fun(getfunstr);
    QList<QString> list;
    list.clear();
    need4fun(getfunstr,list);
    foreach(QString stri,list){
        getobjlist.append(input_obj(stri));
    }
    list.clear();
    get4fun(getfunstr,list);
    foreach(QString stri,list){
       QString str;
       cout<<"remark "<<stri<<":"<<endl;
       cin>>str;
       getrtrmlist.append(str);
    }
}
void mysys::input_condition(QString &geteventstr, QString &getblockstr, QList<myobj *> &getobjlist,QString &getrm){
    QTextStream cout(stdout,QIODevice::WriteOnly);
    QTextStream cin(stdin,QIODevice::ReadOnly);
    input_event(geteventstr);
    input_block(geteventstr,getblockstr);
    cout<<"please input the tgtobj:"<<endl;
    getobjlist.append(input_obj("alltype"));
    QString ch="n";
    for(;;){
        cout<<"please input the caseobj:"<<endl;
        getobjlist.append(input_obj(getobjlist.first()->type));
        cout<<"again? y/n"<<endl;
        cin>>ch;
        if(ch!="y"){break;}
    }
    cout<<"please input the remark:"<<endl;
    cin>>getrm;
}

void mysys::input_event(QString &geteventstr){
    QTextStream cout(stdout,QIODevice::WriteOnly);
    QTextStream cin(stdin,QIODevice::ReadOnly);
    cout<<"please input the event:"<<endl;
    int i;
    i=0;
    foreach(QString stri,eventstrlist){
        cout<<i<<". "<<stri<<endl;
        i++;
    }
    cout<<i<<". input others"<<endl;
    int getinput;
    cin>>getinput;
    if(getinput==i){
        cin>>geteventstr;
    }
    else{
        geteventstr=eventstrlist[getinput];
    }
    if(psk0->newEvent(geteventstr)){
        psk0->newObj(geteventstr);
    }
}
void mysys::input_block(QString geteventstr,QString &getblockstr){
    QTextStream cout(stdout,QIODevice::WriteOnly);
    QTextStream cin(stdin,QIODevice::ReadOnly);
    cout<<"please input the block:"<<endl;
    QList<QString> list;
    need4block(geteventstr,list);
    if(list.isEmpty()){list.append(geteventstr);}
    int i;
    i=0;
    foreach(QString stri,list){
        cout<<i<<". "<<stri<<endl;
        i++;
    }
    cout<<i<<". input others"<<endl;
    int getinput;
    cin>>getinput;
    if(getinput==i){
        cin>>getblockstr;
    }
    else{
        getblockstr=list[getinput].split("|").at(0);
    }
}

void mysys::input_fun(QString &getfunstr){
    QTextStream cout(stdout,QIODevice::WriteOnly);
    QTextStream cin(stdin,QIODevice::ReadOnly);
    cout<<"please input the function:"<<endl;
    int i;
    i=0;
    foreach(QString stri,funstrlist){
        cout<<i<<". "<<stri<<endl;
        i++;
    }
    cout<<i<<". input others"<<endl;
    int getinput;
    cin>>getinput;
    if(getinput==i){
        cin>>getfunstr;
    }
    else{
        getfunstr=funstrlist[getinput];
    }
}

myobj *mysys::input_obj(QString gettype){
    QTextStream cout(stdout,QIODevice::WriteOnly);
    QTextStream cin(stdin,QIODevice::ReadOnly);
    cout<<"please input the "<<gettype<<":"<<endl;
    QList<myobj *> list;
    psk0->getavlobjlist(gettype,list);
    int i;
    QString str;
    i=0;
    foreach(myobj *ip,list){
        cout<<i<<". "<<ip->name<<"|"<<ip->remark<<endl;
        i++;
    }
    cout<<i<<". input others"<<endl;
    int getinput;
    cin>>getinput;
    if(getinput==i){
        if(gettype=="mynum"){
            int getnum;
            cin>>getnum;
            cin>>str;
            myobj *pnum=new myobj;
            pnum->name=QString("%1").arg(getnum);
            pnum->type="mynum";
            pnum->remark=str;
            return pnum;
        }
        else{

        }
    }
    else{
        return list[getinput];
    }
}
*/
