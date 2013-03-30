#include "myevent.h"

//extern void myappend(QList<QString> &list,QString str);
//extern void myappend(QList<QString> &list,QStringList getlist);

QStringList myevent::myeventlist;
myevent::myevent(QObject *parent) :
    QObject(parent)
{
}

void myevent::myini(){
    if(!myeventlist.isEmpty()){return;}
    QFile fin("myevent.txt");
    if(!fin.open(QFile::ReadOnly)){
        QMessageBox::warning(NULL,"FILE ERROR","No myevent.txt");
        qWarning("no myevent.txt\n");
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
        if(str.startsWith(">")&&(str.count("|")==myobj::enumcnt(&staticMetaObject,"iniFormat")-1)){myeventlist.append(str.mid(1));continue;}
        qWarning()<<"myeventini:"<<str<<",";
    }
    fin.close();
}
QStringList myevent::geteventstrlist(){
    myini();
    QStringList strlist;
    foreach(QString stri,myeventlist){
        strlist<<stri.split("|").at(Name);
    }
    return strlist;
}
void myevent::getavlobjlist(QString getstr,QList<myobj *> &list, QObject *getpf){
    myini();
    foreach(QString stri,myeventlist){
        if(stri.startsWith(getstr+"|")){
            QStringList typelist=stri.split("|").at(Data_Type).split(",");
            QStringList namelist=stri.split("|").at(Data_Name).split(",");
            QStringList remarklist=stri.split("|").at(Data_Remark).split(",");
            if(typelist.length()!=namelist.length()){qWarning()<<"myevent,"<<getstr;}
            for(int i=0;i<typelist.length();i++){
                if(typelist.at(i)==""){return;}
                bool b=false;
                foreach(myobj *ip,list){
                    if((ip->name==namelist[i])&&(ip->blockstr==getstr)){b=true;break;}
                }
                if(b){continue;}
                myobj *p=new myobj(getpf);
                if(typelist[i]==""){qWarning()<<"0227here3?";}
                p->type=myobj::str2type(typelist[i]);
                p->name=namelist[i];
                if(i<remarklist.length()){p->remark=remarklist[i];}
                p->blockstr=getstr;
                p->noDeclaration=true;
                //p->isDynamic=false;
                list.prepend(p);
            }
        }
    }
}
QStringList myevent::trans4eventdata(QString getstr,bool front){
    myini();    
    foreach(QString stri,myeventlist){
        if(stri.startsWith(getstr+"|")){
            QStringList namelist=stri.split("|").at(Data_Name).split(",");
            QString str=stri.split("|").at(Data_Trans);
            if(!front){
                if(str.contains(";;")){str=str.split(";;").at(1);}
                else{str="";}
            }
            else{
                str=str.split(";;").at(0);
            }            
            for(int i=0;i<namelist.length();i++){
                /*
                bool b=false;
                foreach(myobj *ip,usdlist){
                    if(ip->name==namelist.at(i)){b=true;break;}
                }
                QString tstr="";
                if(b){tstr=QString("\\1");}
                str.replace(QRegExp("<"+QString::number(i+1)+":([^>]*)>"),tstr);
                */
                str.replace(QRegExp("([^\\\\]?)%"+QString::number(i+1)),"\\1"+namelist[i]);
            }
            if(str==""){return QStringList();}
            return str.split(";");
        }
    }
    qWarning()<<"trans:"<<getstr;
    return QStringList();
}
QString myevent::findRemarkByName(QString getname){
    myini();
    foreach(QString stri,myeventlist){
        if(stri.startsWith(getname+"|")){
            return stri.split("|").at(Remark);
        }
    }
    return "";
}
bool myevent::isEvent(QString getname){
    myini();
    foreach(QString stri,myeventlist){
        if(stri.startsWith(getname+"|")){
            return true;
        }
    }
    return false;
}
