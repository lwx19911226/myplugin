#include "myobj.h"
#include "mysys.h"

QStringList myobj::myconstlist;
QStringList myobj::myconsttaglist;
QStringList myobj::myconstskstrlist;
void myobj::myini(){
    if(myconstlist.isEmpty()){
        QFile fin("myconst.txt");
        if(!fin.open(QFile::ReadOnly)){
            QMessageBox::warning(NULL,"FILE ERROR","No myconst.txt");
            return;
        }
        QTextStream cin(&fin);
        cin.setCodec("UTF-8");
        QString str,tstr;
        QStringList tstrlist;
        for(;;){
            if(cin.atEnd()){break;}
            str=cin.readLine();
            if(str==""){continue;}
            if(str.startsWith("//")){continue;}
            if(str.startsWith(">>")){myconsttaglist<<str.mid(2);continue;}
            if(str.startsWith(">")&&(str.count("|")==enumcnt(&staticMetaObject,"iniFormat")-1)){
                tstr=str.mid(1);
                tstrlist.clear();
                foreach(QString stri,myconstlist.filter("^"+tstr.split("|").at(Name)+"\\|")){
                    tstrlist<<getblabb_str(stri);
                }
                if(tstrlist.contains(getblabb_str(tstr))){qWarning()<<"myconstini"<<tstr;}
                myconstlist<<tstr;
                continue;
            }
            qWarning()<<"myconstini:"<<str<<",";
        }
        fin.close();
        myini_lang();
        myini_cl();

        tstrlist<<tr("TriggerSkill")<<tr("ViewAsSkill")<<tr("DistanceSkill")<<tr("FilterSkill")<<tr("ProhibitSkill")
               <<tr("MaxCardsSkill")<<tr("TargetModSkill")<<tr("ExistingSkill")<<tr("Name")<<tr("Translation")<<tr("Kingdom")<<tr("Sex")
              <<tr("HP")<<tr("Title")<<tr("Word")<<tr("Owner")<<tr("Description")<<tr("Words")<<tr("Subtype")<<tr("CardsNum")
             <<tr("CardViewAs")<<tr("Pattern")<<tr("SKName")<<tr("TargetPlayersNum")<<tr("Block")<<tr("Function");
    }
}
void myobj::myini_lang(){
    QDir dir("../lang/zh_CN");
    if(!dir.exists()){
        QMessageBox::warning(NULL,"FILE ERROR","No lang/zh_CN");
        myconstskstrlist<<"NULL";
        return;
    }
    QRegExp rx1("\\[\"(\\w+.*)\"\\].*=.*\"(.*)[\"\\\\]");
    QRegExp rx2("\\[\":(\\w+.*)\"\\].*=.*\"(.*)[\"\\\\]");
    QStringList cdstrlist;
    cdstrlist<<tr("BasicCard")<<tr("EquipCard")<<tr("TrickCard")<<tr("DelayedTrickCard");
    QString str;
    QStringList tstrlist;
    QStringList ttstrlist;
    QStringList flist;
    QStringList tdirlist;
    tdirlist<<dir.path();
    while(!tdirlist.isEmpty()){
        str=tdirlist.takeFirst();
        QDir tdir(str);
        foreach(QString stri,tdir.entryList(QStringList("*.lua"))){flist<<str+"/"+stri;}
        foreach(QString stri,tdir.entryList(QStringList("*"),QDir::Dirs|QDir::NoDotAndDotDot)){tdirlist<<str+"/"+stri;}
    }
    foreach(QString stri,flist){
        QFile tfin(stri);
        if(!tfin.open(QFile::ReadOnly)){
            QMessageBox::warning(NULL,"FILE ERROR","No "+stri);
            continue;
            return;
        }
        QTextStream tcin(&tfin);
        tcin.setCodec("UTF-8");
        for(;;){
            if(tcin.atEnd()){break;}
            str=tcin.readLine();
            if(str.contains(rx2)){
                if(ttstrlist.contains(rx2.cap(1))){                    
                    int index=tstrlist.indexOf(QRegExp("^"+rx2.cap(1)+"\\|.*"));
                    if(index!=-1){
                        QString getname=tstrlist.at(index).split("|").first();
                        QString gettranslation=tstrlist.at(index).split("|").last();
                        if(!rx2.cap(2).contains(QRegExp(mycode::mymdf(cdstrlist,"^").join("|")))){myconstskstrlist<<tstrlist.at(index)+"|"+rx2.cap(2);}
                        else{
                            bool b=false;
                            foreach(QString tstri,myconstlist){
                                if(tstri.startsWith(getname+"|")){b=true;break;}
                            }
                            if(name2str(getname,QString()).isEmpty()){
                                QStringList obstrlist;
                                obstrlist<<getname<<"mystrc"<<tr("Object Name")+tr("[")+gettranslation+tr("]")<<"ob";
                                myconstlist<<obstrlist.join("|");
                            }
                        }

                    }
                }
                else{qWarning()<<"130609"<<str;}
            }
            else if(str.contains(rx1)){
                ttstrlist<<rx1.cap(1);
                tstrlist<<rx1.cap(1)+"|"+rx1.cap(2);
            }
        }
        tfin.close();
    }
    qSort(myconstskstrlist.begin(),myconstskstrlist.end(),mycmp);
}
void myobj::myini_cl(){
    QStringList oblist=getconstlist_tag("ob");
    foreach(QString stri,oblist){
        QRegExp rxt(".*"+tr("[")+"(.*)"+tr("]")+".*");
        QString trm=name2remark(stri,QString());
        if(rxt.exactMatch(trm)){
            trm=tr("Class Name")+tr("[")+rxt.cap(1)+tr("]")+tr(", ")+tr("Pattern for any ")+rxt.cap(1);
        }
        QString tstr;
        foreach(QString tstri,stri.split("_")){tstr+=tstri.mid(0,1).toUpper()+tstri.mid(1);}
        bool b=false;
        QStringList::Iterator iter;
        for(iter=myconstlist.begin();iter!=myconstlist.end();iter++){
            if(iter->startsWith(tstr+"|")){
                QStringList exstrlist=iter->split("|");
                QStringList::Iterator iter2;
                int cnt=0;
                for(iter2=exstrlist.begin(),cnt=0;iter2!=exstrlist.end();iter2++,cnt++){
                    if(cnt==Remark){*iter2=*iter2+tr(", ")+trm;}
                    if(cnt==Extra){*iter2=*iter2+",,cl,,pt";}
                }
                *iter=exstrlist.join("|");
                b=true;
                break;
            }
        }
        if(b){continue;}
        QStringList clstrlist;
        clstrlist<<tstr<<"mystrc"<<trm<<"cl,,pt";
        myconstlist<<clstrlist.join("|");
    }
}
/*
QString myobj::isConst(QString getstr, QString abbstr){
    myini();
    foreach(QString stri,myconstlist){
        if(stri.startsWith(getstr+"|")){
            bool b=true;
            foreach(QString tstri,stri.split("|").at(Extra).split(",,")){
                if(tstri.startsWith("bl:")&&tstri!="bl:"+abbstr){b=false;break;}
            }
            if(b){return stri;}
        }
    }
    return QString();
}
*/
QStringList myobj::transConst(QString getstr, QString abbstr){
    myini();
    foreach(QString stri,myconstlist){
        if(stri.startsWith(getstr+"|")){
            bool b=true;
            foreach(QString tstri,stri.split("|").at(Extra).split(",,")){
                if(tstri.startsWith("bl:")&&tstri!="bl:"+abbstr){b=false;break;}
            }
            if(b){return transConst(stri);}
        }
    }
    return QStringList();
}
QStringList myobj::transConst(QString getconststr){
    QStringList extrastrlist=getconststr.split("|").at(Extra).split(",,");
    foreach(QString extrastri,extrastrlist){
        if(!extrastri.startsWith("tr:")){continue;}
        QString transstr=extrastri.mid(3);
        if(transstr==""){return QStringList();}
        QString namestr=getconststr.split("|").at(Name);
        transstr.replace(QRegExp("([^\\\\]?)%1"),"\\1"+namestr);
        return transstr.split(";");
    }
    return QStringList();
}

void myobj::newConst(QList<myobj *> &list,QObject *getpf,int getqsv,QString getbl,bool onlybl){
    myini();
    myobj *pnew;
    foreach(QString stri,myconstlist){
        QStringList extrastrlist=stri.split("|").at(Extra).split(",,");
        if(onlybl&&!extrastrlist.contains("bl:"+getbl)){continue;}
        bool b=true;
        foreach(QString extrastri,extrastrlist){
            if(extrastri.startsWith("bl:")&&(extrastri!="bl:"+getbl)){b=false;break;}
        }
        if(!b){continue;}
        if(getqsv!=mysys::VersionUnknown){
            QStringList tstrlist=extrastrlist.filter(QRegExp("^V"));
            if(!tstrlist.isEmpty()&&!tstrlist.contains(mysys::qsv2str(getqsv))){continue;}
        }
        pnew=new myobj(getpf);
        pnew->name=stri.split("|").at(Name);
        if(stri.split("|").at(Type)==""){qWarning()<<"0227here4?";}
        pnew->type=myobj::str2type(stri.split("|").at(Type));
        pnew->remark=stri.split("|").at(Remark);
        //pnew->isDynamic=false;
        pnew->isVerified=true;
        list.append(pnew);
    }
}
QStringList myobj::getconstlist_tag(QString gettag){
    myini();
    QStringList strlist;
    foreach(QString stri,myconstlist){
        if(matchtag_str(stri,gettag)){strlist<<stri.split("|").at(Name);}
    }
    return strlist;
}
QStringList myobj::getconstrmlist_tag(QString gettag){
    myini();
    QStringList strlist;
    foreach(QString stri,myconstlist){
        if(matchtag_str(stri,gettag)){strlist<<stri.split("|").at(Remark);}
    }
    return strlist;
}
QStringList myobj::getconsttaglist(){
    myini();
    QStringList strlist;
    foreach(QString stri,myconsttaglist){
        strlist<<stri.split("|").last();
    }
    return strlist;
}
QString myobj::getblabb_str(QString getstr){
    foreach(QString stri,getstr.split("|").at(Extra).split(",,")){
        if(stri.startsWith("bl:")){return stri.mid(3);}
    }
    return QString();
}

QString myobj::name2str(QString getname, QString abbstr){
    myini();
    QStringList tstrlist=myconstlist.filter(QRegExp("^"+getname+"\\|"));
    foreach(QString stri,tstrlist){
        if(getblabb_str(stri)==abbstr){return stri;}
    }
    foreach(QString stri,tstrlist){
        if(getblabb_str(stri).isEmpty()){return stri;}
    }
    return QString();
}

QString myobj::remark2name(QString getrm){
    myini();
    foreach(QString stri,myconstlist){
        if(stri.split("|").at(Remark)==getrm){return stri.split("|").at(Name);}
    }
    return QString();
}
QString myobj::name2remark(QString getname,QString abbstr){
    myini();
    return name2str(getname,abbstr).split("|").at(Remark);
}
QString myobj::remark2tag(QString getrm){
    myini();
    foreach(QString stri,myconsttaglist){
        if(stri.split("|").last()==getrm){return stri.split("|").first();}
    }
    return QString();
}
bool myobj::matchtag_str(QString getstr, QString gettag){
    return getstr.split("|").at(Extra).split(",,").contains(gettag);
}

QString myobj::enumstr(const QMetaObject *mob,QByteArray getname,int getint){
    return tr(mob->enumerator(mob->indexOfEnumerator(getname)).valueToKey(getint));
}
int myobj::enumint(const QMetaObject *mob,QByteArray getname,QString getstr,int defaultint){
    QMetaEnum myenum=mob->enumerator(mob->indexOfEnumerator(getname));
    for(int i=0;i<myenum.keyCount();i++){
        if((getstr==myenum.key(i))||(getstr==tr(myenum.key(i)))){return myenum.value(i);}
    }
    qWarning()<<"enumint"<<getname<<getstr;
    return defaultint;
}
QStringList myobj::enumstrlist(const QMetaObject *mob,QByteArray getname){
    QStringList strlist;
    QMetaEnum myenum=mob->enumerator(mob->indexOfEnumerator(getname));
    for(int i=0;i<myenum.keyCount();i++){
        strlist<<enumstr(mob,getname,myenum.value(i));
    }
    return strlist;
}
void myobj::enumintlist(const QMetaObject *mob,QByteArray getname,QList<int> &list){
    QMetaEnum myenum=mob->enumerator(mob->indexOfEnumerator(getname));
    for(int i=0;i<myenum.keyCount();i++){
        list<<myenum.value(i);
    }
}
bool myobj::enumcontains(const QMetaObject *mob,QByteArray getname,QString getstr){
    return enumstrlist(mob,getname).contains(getstr)||enumstrlist(mob,getname).contains(tr(getstr.toUtf8()));
}
bool myobj::enumcontains(const QMetaObject *mob,QByteArray getname,int getint){
    return enumcontains(mob,getname,enumstr(mob,getname,getint));
}
int myobj::enumcnt(const QMetaObject *mob,QByteArray getname){
    return mob->enumerator(mob->indexOfEnumerator(getname)).keyCount();
}
