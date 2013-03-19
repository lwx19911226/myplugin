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
void mygeneral::propertymap_get(QMap<QString,QString> &strmap,QMap<QString,QStringList> &strlistmap){
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
void mygeneral::propertymap_set(QMap<QString,QString> &strmap){
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
