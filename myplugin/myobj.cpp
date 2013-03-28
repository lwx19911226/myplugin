#include "myobj.h"

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
        QString str;
        for(;;){
            if(cin.atEnd()){break;}
            str=cin.readLine();
            if(str==""){continue;}
            if(str.startsWith("//")){continue;}
            if(str.startsWith(">>")){myconsttaglist<<str.mid(2);continue;}
            if(str.startsWith(">")&&(str.count("|")==enumcnt(&staticMetaObject,"iniFormat")-1)){myconstlist<<str.mid(1);continue;}
            qWarning()<<"myconstini:"<<str<<",";
        }
        fin.close();
        myini_lang();
        myini_cl();
    }
}
void myobj::myini_lang(){
    QDir dir("../lang/zh_CN");
    if(!dir.exists()){
        QMessageBox::warning(NULL,"FILE ERROR","No lang/zh_CN");
        myconstskstrlist<<"NULL";
        return;
    }
    QRegExp rx1(".*\\[\"(.*)\"\\].*=.*\"(.*)[\"|\\\\].*");
    QRegExp rx2(".*\\[\":(.*)\"\\].*=.*\"(.*)[\"|\\\\].*");
    //QRegExp rxt(".*\\[::.*\\].*");
    //if(rxt.exactMatch("[::1]")){qWarning()<<"130327-0";}
    QString str;
    QStringList tstrlist;
    QStringList ttstrlist;
    QStringList namefilter;
    namefilter<<"*.lua";
    foreach(QString stri,dir.entryList(namefilter)){
        QFile tfin(dir.path()+"/"+stri);
        if(!tfin.open(QFile::ReadOnly)){
            QMessageBox::warning(NULL,"FILE ERROR","No "+dir.path()+"/"+stri);
            return;
        }
        QTextStream tcin(&tfin);
        tcin.setCodec("UTF-8");
        for(;;){
            if(tcin.atEnd()){break;}
            str=tcin.readLine();
            if(rx2.exactMatch(str)){
                if(ttstrlist.contains(rx2.cap(1))){
                    int index=tstrlist.indexOf(QRegExp("^"+rx2.cap(1)+"\\|.*"));
                    if(index!=-1){
                        QString getname=tstrlist.at(index).split("|").first();
                        QString gettranslation=tstrlist.at(index).split("|").last();
                        if(rx2.cap(2).length()>10){myconstskstrlist<<tstrlist.at(index)+"|"+rx2.cap(2);}
                        else if(!isConst(getname)){
                            QStringList obstrlist;
                            obstrlist<<getname<<"mystrc"<<tr("Object Name")+tr("[")+gettranslation+tr("]")<<"ob";
                            //qWarning()<<"130328"<<obstrlist.join("|");
                            myconstlist<<obstrlist.join("|");
                        }

                    }
                }
            }
            else if(rx1.exactMatch(str)){
                ttstrlist<<rx1.cap(1);
                tstrlist<<rx1.cap(1)+"|"+rx1.cap(2);
            }
        }
        tfin.close();
    }
}
void myobj::myini_cl(){
    QStringList oblist=getconstlist_tag("ob");
    foreach(QString stri,oblist){
        QRegExp rxt(".*"+tr("[")+"(.*)"+tr("]")+".*");
        QString trm=name2remark(stri);
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

bool myobj::isConst(QString getstr){
    myini();
    foreach(QString stri,myconstlist){
        if(stri.startsWith(getstr+"|")){return true;}
    }
    return false;
}
QStringList myobj::transConst(QString getstr){
    myini();
    foreach(QString stri,myconstlist){
        if(stri.startsWith(getstr+"|")){
            QStringList extrastrlist=stri.split("|").at(Extra).split(",,");
            foreach(QString extrastri,extrastrlist){
                if(!extrastri.startsWith("tr:")){continue;}
                QString transstr=extrastri.mid(3);
                if(transstr==""){return QStringList();}
                QString namestr=stri.split("|").at(Name);
                transstr.replace(QRegExp("([^\\\\]?)%1"),"\\1"+namestr);
                return transstr.split(";");
            }
        }
    }
    return QStringList();
}
void myobj::newConst(QList<myobj *> &list,QString getbl,QObject *getpf,bool only){
    myini();
    myobj *pnew;
    foreach(QString stri,myconstlist){
        QStringList extrastrlist=stri.split("|").at(Extra).split(",,");
        if(only&&!extrastrlist.contains("bl:"+getbl)){continue;}
        bool b=true;
        foreach(QString extrastri,extrastrlist){
            if(extrastri.startsWith("bl:")&&(extrastri!="bl:"+getbl)){b=false;break;}
        }
        if(!b){continue;}
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
        QString getname=stri.split("|").at(Name);
        if(matchTag(getname,gettag)){
            strlist<<getname;
        }
    }
    return strlist;
}
QStringList myobj::getconstrmlist_tag(QString gettag){
    myini();
    QStringList strlist;
    foreach(QString stri,myconstlist){
        QString getrm=stri.split("|").at(Remark);
        if(matchTag(stri.split("|").at(Name),gettag)){
            strlist<<getrm;
        }
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
QString myobj::remark2name(QString getrm){
    myini();
    foreach(QString stri,myconstlist){
        if(stri.split("|").at(Remark)==getrm){return stri.split("|").at(Name);}
    }
    return QString();
}
QString myobj::name2remark(QString getname){
    myini();
    foreach(QString stri,myconstlist){
        if(stri.split("|").at(Name)==getname){return stri.split("|").at(Remark);}
    }
    return QString();
}
QString myobj::remark2tag(QString getrm){
    myini();
    foreach(QString stri,myconsttaglist){
        if(stri.split("|").last()==getrm){return stri.split("|").first();}
    }
    return QString();
}
bool myobj::matchTag(QString getname, QString gettag){
    myini();
    foreach(QString stri,myconstlist){
        if(stri.startsWith(getname+"|")){
            return stri.split("|").at(Extra).split(",,").contains(gettag);
        }
    }
    return true;
}
