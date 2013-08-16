#include "myobj.h"
#include "mysys.h"

QStringList myobj::myconstlist;
QStringList myobj::myconsttaglist;
QStringList myobj::myconstskstrlist;
void myobj::myini(int getqsv){
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
        myini_lang(getqsv);
        myini_cl(getqsv);
        if(myconstskstrlist.isEmpty()){qWarning()<<"myconstskstrlist"<<"empty";}
        tstrlist<<tr("TriggerSkill")<<tr("ViewAsSkill")<<tr("DistanceSkill")<<tr("FilterSkill")<<tr("ProhibitSkill")
               <<tr("MaxCardsSkill")<<tr("TargetModSkill")<<tr("ExistingSkill")<<tr("Name")<<tr("Translation")<<tr("Kingdom")<<tr("Sex")
              <<tr("HP")<<tr("Title")<<tr("Word")<<tr("Owner")<<tr("Description")<<tr("Words")<<tr("Subtype")<<tr("CardsNum")
             <<tr("CardViewAs")<<tr("Pattern")<<tr("SKName")<<tr("TargetPlayersNum")<<tr("Block")<<tr("Function")<<tr("SkillCard");
    }
}
void myobj::myini_lang(int getqsv){
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
    QStringList tstrlist1;
    QStringList tstrlist2;
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
                if(tstrlist2.contains(rx2.cap(1))){
                    ttstrlist=mycode::myfilter_sw(tstrlist1,rx2.cap(1)+"|");
                    //int index=tstrlist.indexOf(QRegExp("^"+rx2.cap(1)+"\\|.*"));
                    if(!ttstrlist.isEmpty()){
                        QString getname=ttstrlist.first().split("|").first();
                        QString gettranslation=ttstrlist.first().split("|").last();
                        if(!rx2.cap(2).contains(QRegExp(mycode::mymdf(cdstrlist,"^").join("|")))){myconstskstrlist<<ttstrlist.first()+"|"+rx2.cap(2);}
                        else{
                            bool b=false;
                            foreach(QString tstri,myconstlist){
                                if(tstri.startsWith(getname+"|")){b=true;break;}
                            }
                            if(name2str(getname,mysys::VersionUnknown,QString()).isEmpty()){
                                QStringList obstrlist;
                                obstrlist<<getname<<"mystrc"<<tr("Object Name")+tr("[")+gettranslation+tr("]");
                                QString extrastr="ob";
                                if(getqsv!=mysys::VersionUnknown){extrastr+=",,"+mysys::qsv2str(getqsv);}
                                obstrlist<<extrastr;
                                myconstlist<<obstrlist.join("|");
                            }
                        }

                    }
                }
                else{qWarning()<<"130609"<<str;}
            }
            else if(str.contains(rx1)){
                tstrlist2<<rx1.cap(1);
                tstrlist1<<rx1.cap(1)+"|"+rx1.cap(2);
            }
        }
        tfin.close();
    }
    qSort(myconstskstrlist.begin(),myconstskstrlist.end(),mycmp);
}
void myobj::myini_cl(int getqsv){
    QStringList oblist=getconstlist_tag(mysys::VersionUnknown,"ob");
    foreach(QString stri,oblist){
        QRegExp rxt(".*"+tr("[")+"(.*)"+tr("]")+".*");
        QString trm=name2remark(stri,mysys::VersionUnknown,QString());
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
        clstrlist<<tstr<<"mystrc"<<trm;
        QString extrastr="cl,,pt";
        if(getqsv!=mysys::VersionUnknown){extrastr+=",,"+mysys::qsv2str(getqsv);}
        clstrlist<<extrastr;
        myconstlist<<clstrlist.join("|");
    }
}
void myobj::iniwarning(){if(myconstlist.isEmpty()){qWarning()<<"iniwarning"<<"myconst";}}
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
QStringList myobj::transConst(QString getname, QString abbstr, int getqsv){
    iniwarning();    
    foreach(QString stri,mycode::myfilter_sw(myconstlist,getname+"|")){
        if(matchblabb_str(stri,abbstr,false)&&matchqsv_str(stri,getqsv)){
            return transconst_str(stri);
        }
    }
    return QStringList();
}
QStringList myobj::transconst_str(QString getconststr){
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
bool myobj::needtransconst_str(QString getconststr){
    return getconststr.split("|").at(Extra).split(",,").contains("tr");
}

void myobj::newConst(QList<myobj *> &list,QObject *getpf,int getqsv,QString abbstr,bool only){
    iniwarning();
    myobj *pnew=NULL;
    foreach(QString stri,myconstlist){
        if(matchblabb_str(stri,abbstr,only)&&matchqsv_str(stri,getqsv)){
            pnew=new myobj(getpf);
            pnew->name=stri.split("|").at(Name);
            pnew->type=myobj::str2type(stri.split("|").at(Type));
            pnew->remark=stri.split("|").at(Remark);
            //pnew->isDynamic=false;
            pnew->isVerified=true;
            list.append(pnew);
        }
    }
}
QStringList myobj::getconstlist_tag(int getqsv, QString gettag){
    iniwarning();
    QStringList strlist;
    foreach(QString stri,myconstlist){
        if(matchtag_str(stri,gettag)&&matchqsv_str(stri,getqsv)){strlist<<stri.split("|").at(Name);}
    }
    return strlist;
}
QStringList myobj::getconstrmlist_tag(int getqsv,QString gettag){
    iniwarning();
    QStringList strlist;
    foreach(QString stri,myconstlist){
        if(matchtag_str(stri,gettag)&&matchqsv_str(stri,getqsv)){strlist<<stri.split("|").at(Remark);}
    }
    return strlist;
}
QStringList myobj::getconsttaglist(){
    iniwarning();
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

QString myobj::name2str(QString getname,int getqsv, QString abbstr){
    iniwarning();    
    QStringList tstrlist=mycode::myfilter_sw(myconstlist,getname+"|");
    QStringList ttstrlist;
    foreach(QString stri,tstrlist){
        if(matchqsv_str(stri,getqsv)){ttstrlist<<stri;}
    }
    foreach(QString stri,ttstrlist){
        if(matchblabb_str(stri,abbstr,true)){return stri;}
    }
    foreach(QString stri,ttstrlist){
        if(matchblabb_str(stri,abbstr,false)){return stri;}
    }
    return QString();
}

QString myobj::remark2name(QString getrm){
    iniwarning();
    foreach(QString stri,myconstlist){
        if(stri.split("|").at(Remark)==getrm){return stri.split("|").at(Name);}
    }
    return QString();
}
QString myobj::name2remark(QString getname,int getqsv,QString abbstr){
    iniwarning();
    return name2str(getname,getqsv,abbstr).split("|").at(Remark);
}
QString myobj::remark2tag(QString getrm){
    iniwarning();
    foreach(QString stri,myconsttaglist){
        if(stri.split("|").last()==getrm){return stri.split("|").first();}
    }
    return QString();
}
bool myobj::matchtag_str(QString getstr, QString gettag){
    return getstr.split("|").at(Extra).split(",,").contains(gettag);
}
bool myobj::matchqsv_str(QString getstr, int getqsv){
    if(getqsv!=mysys::VersionUnknown){
        QStringList tstrlist=getstr.split("|").at(Extra).split(",,").filter(QRegExp("^V"));
        if(!tstrlist.isEmpty()&&!tstrlist.contains(mysys::qsv2str(getqsv))){return false;}
    }
    return true;
}
bool myobj::matchqsv0_str(QString getstr){
    return getstr.split("|").at(Extra).split(",,").filter(QRegExp("^V")).isEmpty();
}
bool myobj::matchblabb_str(QString getstr, QString abbstr, bool only){
    QString tstr=getblabb_str(getstr);
    if(abbstr==tstr){return true;}
    if(!only&&(tstr.isEmpty()||abbstr.isEmpty())){return true;}
    return false;
}
QString myobj::objname_default(){
    iniwarning();
    foreach(QString stri,myconstlist){
        if(matchtag_str(stri,"ob")&&matchqsv0_str(stri)){
            return stri.split("|").at(Name);
        }
    }
    qWarning()<<"objname_default";
    return QString();
}
QString myobj::ptname_default(){
    iniwarning();
    foreach(QString stri,myconstlist){
        if(matchtag_str(stri,"pt")&&matchqsv0_str(stri)){
            return stri.split("|").at(Name);
        }
    }
    qWarning()<<"ptname_default";
    return QString();
}
QString myobj::skname_default(){
    iniwarning();
    return myconstskstrlist.first().split("|").first();
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
