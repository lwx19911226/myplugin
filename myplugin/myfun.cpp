#include "myfun.h"
#include "mycode.h"
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
    QString str;
    for(;;){
        if(cin.atEnd()){break;}
        str=cin.readLine();
        if(str==""){continue;}
        if(str.startsWith("//")){continue;}
        if(str.startsWith(">>")){myfuntaglist<<str.mid(2);continue;}
        if(str.startsWith(">")&&str.count("|")==Parts-1){myfunlist<<str.mid(1);continue;}
        qWarning()<<"myfunini:"<<str<<",";
    }
    fin.close();
}
QStringList myfun::need(QString getstr){
    myini();
    foreach(QString stri,myfunlist){
        if(stri.startsWith(getstr+"|")){
            //myappend(list,stri.split("|").at(In_Type).split(","));
            if(stri.split("|").at(In_Type)==""){return QStringList();}
            return stri.split("|").at(In_Type).split(",");
        }
    }
    return QStringList();
}
QStringList myfun::get(QString getstr){
    myini();
    foreach(QString stri,myfunlist){
        if(stri.startsWith(getstr+"|")){
            //myappend(list,stri.split("|").at(Out_Type).split(","));
            if(stri.split("|").at(Out_Type)==""){return QStringList();}
            return stri.split("|").at(Out_Type).split(",");
        }
    }
    return QStringList();
}

int myfun::getBlock_cnt(QString getstr){
    myini();
    foreach(QString stri,myfunlist){
        if(stri.startsWith(getstr+"|")){
            return stri.split("|").at(Trans).count("%block");
        }
    }
    return 0;
}
bool myfun::notnil(QString getstr, int geti){
    myini();
    return myfun::getTrans(getstr).contains("<"+QString::number(geti+1)+":");
}

QString myfun::getTrans(QString getstr){
    myini();
    foreach(QString stri,myfunlist){
        if(stri.startsWith(getstr+"|")){
            return stri.split("|").at(Trans);
        }
    }
    return QString();
}

QStringList myfun::getfunstrlist(QStringList taglist){
    myini();
    QStringList strlist;
    foreach(QString stri,myfunlist){
        QString getname=stri.split("|").at(Name);
        if(matchTaglist(getname,taglist)){strlist<<getname;}
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
bool myfun::matchTaglist(QString getname, QStringList gettaglist){
    myini();
    foreach(QString stri,myfunlist){
        if(stri.startsWith(getname+"|")){
            QStringList taglist=stri.split("|").at(Tag).split(",");
            foreach(QString tagstri,taglist.filter(QRegExp("\\$$"))){
                if(gettaglist.contains(tagstri)||gettaglist.contains(tagstri.mid(0,tagstri.length()-1))){}
                else{return false;}
            }
            foreach(QString tagstri,gettaglist.filter(QRegExp("[^\\$]$"))){
                if(taglist.contains(tagstri)||taglist.contains(tagstri+"$")){}
                else{return false;}
            }
            return true;
        }
    }
    return true;
}

QString myfun::findRemarkByName(QString getname){
    myini();
    foreach(QString stri,myfunlist){
        if(stri.startsWith(getname+"|")){
            return stri.split("|").at(Remark);
        }
    }
    return "";
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
