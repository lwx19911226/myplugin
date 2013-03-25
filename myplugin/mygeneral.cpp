#include "mygeneral.h"
#include "mysys.h"
int mygeneral::globalint=0;
mysys *mygeneral::getsys(){
    return static_cast<mysys *>(parent());
}
void mygeneral::setDefaultName(){
    setName("g"+QString::number(globalint++));
}
void mygeneral::setName(QString getname){
    if(getname==""){setDefaultName();return;}
    foreach(mygeneral *ip,getsys()->glist){
        if(ip==this){continue;}
        if(ip->name==getname){setDefaultName();return;}
    }
    name=getname;
}
void mygeneral::propertymap_get(QMap<QString,QString> &strmap,QMap<QString,QStringList> &strlistmap,bool b4remark){
    if(b4remark){}
    strmap.insert(property2str(Name),name);
    strmap.insert(property2str(Translation),translation);
    strmap.insert(property2str(Kingdom),kingdom2str(kingdom));
    strlistmap.insert(property2str(Kingdom),kingdomstrlist());
    strmap.insert(property2str(Sex),sex2str(sex));
    strlistmap.insert(property2str(Sex),sexstrlist());
    strmap.insert(property2str(HP),QString::number(hp));
    strmap.insert(property2str(Title),title);
    strmap.insert(property2str(Word),word);
    strmap.insert(property2str(CV),cv);
}
void mygeneral::propertymap_set(QMap<QString,QString> &strmap,bool b4remark){
    if(b4remark){}
    if(strmap.contains(property2str(Name))){setName(strmap.value(property2str(Name)));}
    if(strmap.contains(property2str(Translation))){translation=strmap.value(property2str(Translation));}
    if(strmap.contains(property2str(Kingdom))){kingdom=str2kingdom(strmap.value(property2str(Kingdom)));}
    if(strmap.contains(property2str(Sex))){sex=str2sex(strmap.value(property2str(Sex)));}
    if(strmap.contains(property2str(HP))){
        bool b;
        int gethp=strmap.value(property2str(HP)).toInt(&b);
        if(b){hp=gethp;}
    }
    if(strmap.contains(property2str(Title))){title=strmap.value(property2str(Title));}
    if(strmap.contains(property2str(Word))){word=strmap.value(property2str(Word));}
    if(strmap.contains(property2str(CV))){cv=strmap.value(property2str(CV));}
    getsys()->sig_update();
}
QString mygeneral::propertystr_get(){
    QStringList strlist;
    strlist<<name;
    strlist<<translation;
    strlist<<kingdom2str(kingdom);
    strlist<<sex2str(sex);
    strlist<<QString::number(hp);
    if(title!=""){strlist<<property2prefix(Title)+title;}
    if(word!=""){strlist<<property2prefix(Word)+word;}
    if(cv!=""){strlist<<property2prefix(CV)+cv;}
    return strlist.join("|");
}
void mygeneral::propertystr_set(QString getstr){
    QStringList strlist=getstr.split("|");
    if(strlist.length()<5){qWarning()<<"propertystr_set"<<getstr;}
    QMap<QString,QString> strmap;
    strmap.insert(property2str(Name),strlist.at(0));
    strmap.insert(property2str(Translation),strlist.at(1));
    strmap.insert(property2str(Kingdom),strlist.at(2));
    strmap.insert(property2str(Sex),strlist.at(3));
    strmap.insert(property2str(HP),strlist.at(4));
    for(int i=5;i<strlist.length();i++){
        if(strlist.at(i).startsWith(property2prefix(Title))){
            strmap.insert(property2str(Title),strlist.at(i).mid(property2prefix(Title).length()));
        }
        if(strlist.at(i).startsWith(property2prefix(Word))){
            strmap.insert(property2str(Word),strlist.at(i).mid(property2prefix(Word).length()));
        }
        if(strlist.at(i).startsWith(property2prefix(CV))){
            strmap.insert(property2str(CV),strlist.at(i).mid(property2prefix(CV).length()));
        }
    }
    propertymap_set(strmap,false);
}
