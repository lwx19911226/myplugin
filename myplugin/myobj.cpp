#include "myobj.h"

QStringList myobj::myconstlist;
QStringList myobj::myconsttaglist;
void myobj::myini(){
    if(!myconstlist.isEmpty()){return;}
    QFile fin("myconst.txt");
    if(!fin.open(QFile::ReadOnly)){
        QMessageBox::warning(NULL,"FILE ERROR","No myconst.txt");
        qWarning("no myconst.txt\n");
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
        if(str.startsWith(">")&&(str.count("|")==Parts-1)){myconstlist<<str.mid(1);continue;}
        qWarning()<<"myconstini:"<<str<<",";
    }
    fin.close();
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
