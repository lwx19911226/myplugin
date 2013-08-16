#include "myfun.h"
#include "mycode.h"
#include "mysys.h"
/*
void myappend(QList<QString> &list,QString str){
    if(str==""){return;}
    list.append(str);
}
void myappend(QList<QString> &list,QStringList getlist){
    foreach(QString stri,getlist){myappend(list,stri);}
}
*/
QStringList myfun::myfunlist;
QStringList myfun::myfuntaglist;
int myfun::globalint=0;
myfun::myfun(QObject *parent) :
    QObject(parent)
{
}

void myfun::myini(){
    if(!myfunlist.isEmpty()){return;}
    QFile fin("myfun.txt");
    if(!fin.open(QFile::ReadOnly)){
        QMessageBox::warning(NULL,"FILE ERROR","No myfun.txt");
        qWarning("no myfun.txt\n");
        return;
    }
    QTextStream cin(&fin);
    cin.setCodec("UTF-8");
    QString str,tstr;
    QList<int> tlist0,tlist;
    for(;;){
        if(cin.atEnd()){break;}
        str=cin.readLine();
        if(str==""){continue;}
        if(str.startsWith("//")){continue;}
        if(str.startsWith(">>")){myfuntaglist<<str.mid(2);continue;}
        if(str.startsWith(">")){
            tstr=str.mid(1);
            if(str.replace("\\|","\\\\").count("|")==myobj::enumcnt(&staticMetaObject,"iniFormat")-1){
                tlist0=getqsvlist_str(tstr);
                tlist.clear();
                foreach(QString stri,name2strlist(tstr.split("|").at(Name))){
                    tlist<<getqsvlist_str(stri);
                }
                foreach(int i,tlist0){
                    foreach(int j,tlist){
                        if((i==mysys::VersionUnknown)||(j==mysys::VersionUnknown)||(i==j)){qWarning()<<"myfunini"<<tstr;}
                    }
                }
                myfunlist<<tstr;
                continue;
            }
        }
        qWarning()<<"myfunini:"<<str<<",";
    }
    fin.close();
}
QString myfun::extract_str(QString getstr, int gettype){
    return mycode::mysplit(getstr,"|").at(gettype);
    //return getstr.replace("\\|","\\\\").split("|").replaceInStrings("\\\\","|").at(gettype);
}
QStringList myfun::name2strlist(QString getname){
    return mycode::myfilter_sw(myfunlist,getname+"|");
    //return myfunlist.filter(QRegExp("^"+getname+"\\|"));
}
QString myfun::name2str(QString getname, int getqsv){
    foreach(QString stri,name2strlist(getname)){
        if(matchqsv_str(stri,getqsv)){return stri;}
    }
    return QString();
}
QStringList myfun::intypestrlist(QString getname,int getqsv){
    myini();
    QString tstr=name2str(getname,getqsv);
    if(!tstr.isEmpty()&&!tstr.split("|").at(In_Type).isEmpty()){
        return tstr.split("|").at(In_Type).split(",");
    }
    return QStringList();
}
QStringList myfun::outtypestrlist(QString getname,int getqsv){
    myini();
    QString tstr=name2str(getname,getqsv);
    if(!tstr.isEmpty()&&!tstr.split("|").at(Out_Type).isEmpty()){
        return tstr.split("|").at(Out_Type).split(",");
    }
    return QStringList();
}
QStringList myfun::inouttypestrlist(QString getname, int getqsv){
    myini();
    QString tstr=name2str(getname,getqsv);
    QStringList rlist;
    if(!tstr.isEmpty()){
        QStringList tstrlist=tstr.split("|");
        QString tstr1=tstrlist.at(In_Type);
        QString tstr2=tstrlist.at(Out_Type);
        if(!tstr1.isEmpty()){rlist<<tstr1.split(",");}
        if(!tstr2.isEmpty()){rlist<<tstr2.split(",");}
    }
    return rlist;
}

int myfun::name2blockcnt(QString getname, int getqsv){
    myini();
    QString tstr=name2str(getname,getqsv);
    if(!tstr.isEmpty()){return extract_str(tstr,Trans).count("%block");}
    return 0;
}
bool myfun::notnil(QString getname,int getqsv,int geti){
    myini();
    //QStringList strlist;
    //strlist<<myfun::intypestrlist(getname,getqsv)<<myfun::outtypestrlist(getname,getqsv);
    //return myfun::getTrans(getstr).contains("<"+QString::number(geti+1)+":");
    return myobj::gettypesuffix(inouttypestrlist(getname,getqsv).at(geti)).contains("~");
}

QString myfun::name2trans(QString getname,int getqsv){
    myini();
    QString tstr=name2str(getname,getqsv);
    if(!tstr.isEmpty()){
        return extract_str(tstr,Trans);
    }
    return QString();
}

QStringList myfun::getfunstrlist(int getqsv, QStringList taglist){
    myini();
    QStringList strlist;
    foreach(QString stri,myfunlist){
        if(!matchqsv_str(stri,getqsv)){continue;}
        if(!matchtaglist_str(stri,taglist)){continue;}
        strlist<<stri.split("|").at(Name);
    }
    return strlist;
}
QStringList myfun::getfuntagrmlist(bool visibility){
    myini();
    QStringList strlist;
    foreach(QString stri,myfuntaglist){
        if(visibility&&stri.split("|").first().endsWith("$")){}
        else{strlist<<stri.split("|").last();}
    }
    return strlist;
}
QString myfun::remark2tag(QString getrm){
    myini();
    foreach(QString stri,myfuntaglist){
        if(stri.split("|").last()==getrm){return stri.split("|").first();}
    }
    return QString();
}
bool myfun::matchtaglist(QString getname,int getqsv, QStringList gettaglist){
    myini();
    QString tstr=name2str(getname,getqsv);
    if(!tstr.isEmpty()){return matchtaglist_str(tstr,gettaglist);}
    return true;
}
bool myfun::matchtaglist_str(QString getstr, QStringList gettaglist){
    QStringList taglist=getstr.split("|").at(Tag).split(",");
    foreach(QString tagstri,taglist.filter(QRegExp("\\$$"))){
        bool b=false;
        foreach(QString tstri,tagstri.mid(0,tagstri.length()-1).split("+")){
            if(gettaglist.contains(tstri)||gettaglist.contains(tstri+"$")){b=true;break;}
            //else{return false;}
        }
        if(!b){return false;}
    }
    foreach(QString tagstri,gettaglist.filter(QRegExp("[^\\$]$"))){
        if(taglist.contains(tagstri)||taglist.contains(tagstri+"$")){}
        else{return false;}
    }
    return true;
}
bool myfun::matchqsv_str(QString getstr, int getqsv){
    if(getqsv==mysys::VersionUnknown){return true;}
    QStringList tstrlist=getstr.split("|").at(Tag).split(",").filter(QRegExp("^V"));
    if(tstrlist.isEmpty()){return true;}
    else{return !tstrlist.filter(mysys::qsv2str(getqsv)).isEmpty();}
}
QList<int> myfun::getqsvlist_str(QString getstr){
    QList<int> tlist;
    QStringList tstrlist=getstr.split("|").at(Tag).split(",").filter(QRegExp("^V"));
    if(tstrlist.isEmpty()){tlist<<mysys::VersionUnknown;}
    else if(tstrlist.length()==1){
        QString tstr=tstrlist.first();
        if(tstr.endsWith("$")){tstr=tstr.mid(0,tstr.length()-1);}
        foreach(QString stri,tstr.split("+")){
            int tqsv=mysys::str2qsv(stri);
            if(!tlist.contains(tqsv)){tlist<<tqsv;}
        }
    }
    else{qWarning()<<"getqsvlist_str"<<getstr;}
    return tlist;
}

QString myfun::name2remark(QString getname,int getqsv){
    myini();
    QString tstr=name2str(getname,getqsv);
    if(!tstr.isEmpty()){return extract_str(tstr,Remark);}
    return QString();
}


/*
void myfun::setrt(QList<QString> &rtnamelist,QList<QString> &rtrmlist,QString geteventstr){
    QList<QString> list;
    get(name,list);
    if(list.length()!=rtnamelist.length()){qWarning()<<"setrt,"<<name;}
    for(int i=0;i<list.length();i++){        
        myobj *p=new myobj(this);
        p->type=myobj::str2type(list[i]);
        if(rtnamelist[i].endsWith("$")){
            p->name=rtnamelist[i].mid(0,rtnamelist[i].length()-1);
            p->noDeclaration=true;
            //p->isDynamic=false;
        }
        else{
            p->name=rtnamelist[i];
            p->eventstr=geteventstr;
        }
        p->remark=rtrmlist[i];
        rtobjlist.append(p);
    }
}
void myfun::setBlock(QList<QString> &blrmlist){
    int cnt=getBlock_cnt(name);
    if(blrmlist.length()!=cnt){qWarning()<<"setblock,"<<name;}
    for(int i=0;i<cnt;i++){
        myblock *p=new myblock(parent());
        p->name=static_cast<myblock *>(parent())->name+","+QString::number(i+1);
        p->remark=blrmlist.at(i);
        blocklist<<p;
    }
}

QStringList myfun::trans(){
    foreach(QString stri,myfunlist){
        if(stri.startsWith(name+"|")){
            QList<myobj *> list;
            list.append(objlist);
            list.append(rtobjlist);
            QString str=stri.split("|").at(Trans);
            if(str.contains("%%")){str.replace(QRegExp("%%"),QString::number(++globalint));}
            for(int i=0;i<list.length();i++){
                str.replace(QRegExp("([^\\\\]?)%"+QString::number(i+1)),"\\1"+list[i]->name);
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
    }
    qWarning()<<"trans:"<<name;
    return QStringList();
}

void myfun::setrtrm(QList<QString> &rtrmlist){
    //if(rtobjlist.isEmpty()){qDebug()<<"setrtrm,"<<name;}
    //if(rtobjlist.length()!=rtrmlist.length()){qDebug()<<"setrtrm2,"<<name;}
    QList<QString> list;
    get(name,list);
    if(list.length()!=rtrmlist.length()){qWarning()<<"setrtrm,"<<name;}
    for(int i=0;i<list.length();i++){
        rtobjlist[i]->remark=rtrmlist[i];
    }
}
void myfun::setrtevent(QString getstr){
    foreach(myobj *ip,rtobjlist){
        if(ip->noDeclaration){}
        else{ip->eventstr=getstr;}
    }
}

QString myfun_drawCards::trans(){
    return QString("%1:drawCards(%2)\n").arg(objlist[0]->name).arg(objlist[1]->name);
}
void myfun_drawCards::need(QList<QString> &list){
    list.append("myplayer");
    list.append("mynum");
}
void myfun_drawCards::get(QList<QString> &list){

}

void myfun_getHp::setrt(QList<QString> &rtnamelist){
    myobj *rtobj=new myobj;
    rtobj->type="mynum";
    rtobj->name=rtnamelist.first();
    rtobjlist.append(rtobj);
}
QString myfun_getHp::trans(){
    return QString("%1=%2:getHp()\n").arg(rtobjlist.first()->name).arg(objlist[0]->name);
}
void myfun_getHp::need(QList<QString> &list){
    list.append("myplayer");
}
void myfun_getHp::get(QList<QString> &list){
    list.append("mynum");
}
*/
