#include "myinputmodel.h"

#define VISIBILITY true

myinputmodel::myinputmodel(QObject *parent) :
    QAbstractItemModel(parent)
{
    rootitem0=NULL;
    rootitem=NULL;    
    myini();
}

myinputmodel::~myinputmodel(){
    if(rootitem){delete rootitem;}
    if(rootitem0){delete rootitem0;}
}

void myinputmodel::myini(){
    rootitem0=new myinputitem;
    rootitem0->pf=NULL;
    rootitem0->mydata=QVariant("root");
    rootitem0->type=myinputitem::func_Start;
    rootitem=new myinputitem;
    rootitem->pf=NULL;
    rootitem->mydata=QVariant("root");
    rootitem->type=myinputitem::func_Start;

    rootitem0->newChild(QVariant(">EVENT:"));
    if(getsk0()){
        foreach(QString stri,getsk0()->eventstrlist()){
            rootitem0->newChild(QVariant(stri),true);
        }
    }
    myinputitem::mycpy(rootitem0,rootitem,true);
}

QModelIndex myinputmodel::index(int row, int column, const QModelIndex &parent) const{
    //qWarning()<<"index";
    if(!hasIndex(row,column,parent)){return QModelIndex();}
    myinputitem *pf=getItem(parent);
    if(!pf){return QModelIndex();}
    return (pf->getChildren_cnt(VISIBILITY)>row)?createIndex(row,column,pf->getChild(row,VISIBILITY)):QModelIndex();
    //return hasIndex(row, column, parent) ? createIndex(row, column, 0) : QModelIndex();
}

QModelIndex myinputmodel::parent(const QModelIndex &child) const{
    //qWarning()<<"parent";
    if (!child.isValid()){return QModelIndex();}
    myinputitem *pfitem = getItem(child)->pf;
    if(pfitem==rootitem){return QModelIndex();}
    return createIndex(pfitem->pf?pfitem->getRank(VISIBILITY):0,0,pfitem);
    //return parentItem==rootitem?QModelIndex():createIndex(parentItem->getchnum(),0,parentItem);
    //return QModelIndex();
}

int myinputmodel::columnCount(const QModelIndex &parent) const{
    //qWarning()<<"columncount";    
    return getItem(parent)->getcnt();
}

int myinputmodel::rowCount(const QModelIndex &parent) const{
    //qWarning()<<"rowcount";
    return getItem(parent)?getItem(parent)->getChildren_cnt(VISIBILITY):0;
}

QVariant myinputmodel::data(const QModelIndex &index, int role) const{
    //qWarning()<<"data";
    if (!index.isValid()){return QVariant();}
    if (role != Qt::DisplayRole && role != Qt::EditRole){return QVariant();}
    if(!getItem(index)->visible){qWarning()<<"visible";}
    return getItem(index)->mydata;
    //return QVariant();
}

myinputitem *myinputmodel::getItem(const QModelIndex &index) const{
    //qWarning()<<"getitem";
    if (index.isValid()) {
        myinputitem *p = static_cast<myinputitem *>(index.internalPointer());
        if (p) return p;
    }
    return rootitem;
}
myinputitem *myinputmodel::getItem0(const QModelIndex &index) const{
    myinputitem *pi=getItem(index);
    return rootitem0->search(pi);
}

bool myinputmodel::canFetchMore(const QModelIndex &parent) const{
    if(getItem0(parent)&&getItem0(parent)->pchlist.length()==1){return true;}
    return false;
}

void myinputmodel::fetchMore(const QModelIndex &parent){
    //qWarning()<<"fetchmore0";
    myinputitem *pp=getItem0(parent);
    myinputitem *p=getItem(parent);
    if(pp->type+1==myinputitem::func_Block){
        pp->newChild(QVariant(">BLOCK:"),false,true);
        QString geteventstr=pp->getstr();
        QStringList list;
        list=getsys()->psk0->need4block(geteventstr);
        if(list.isEmpty()){list.append(geteventstr);}
        foreach(QString stri,list){
            pp->newChild(QVariant(stri),true);
        }
    }
    else if(pp->type+1==myinputitem::func_Fun){
        QString geteventstr=pp->getParent(myinputitem::func_Event)->getstr();
        pp->newChild(QVariant(">FUNCTION:"),false,true);
        foreach(QString stri,myfun::getfunstrlist(getsys()->psk0->funtaglist(geteventstr))){
            pp->newChild(QVariant(stri),!myfun::need(stri).isEmpty());
        }
    }
    else if(pp->type+1>=myinputitem::func_Obj){
        //if(!pp->getParent(myinputitem::stc_Fun,true)){return;}
        QString geteventstr=pp->getParent(myinputitem::func_Event)->getstr();
        QString getfunstr=pp->getParent(myinputitem::func_Fun,true)->getstr();
        QStringList strlist;
        strlist=myfun::need(getfunstr);
        if(!strlist.isEmpty()){
            QList<myobj *> objlist;
            QString gettypestr=strlist.at(pp->type+1-myinputitem::func_Obj);
            pp->newChild(QVariant(">OBJECT("+gettypestr+"):"),false,true);
            foreach(QString stri,gettypestr.split("+")){
                getsys()->psk0->getavlobjlist(myobj::str2type(stri),objlist,geteventstr);
            }
            foreach(myobj *ip,objlist){
                pp->newChild(QVariant(ip->name),pp->type+1-myinputitem::func_Obj<strlist.length()-1);
            }
        }
    }
    p->delAllChildren();
    myinputitem::mycpy(pp,p,true);
    //qWarning()<<"fetchmore";
}

bool myinputmodel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(role!=Qt::EditRole){return false;}
    myinputitem *pi=getItem(index);
    myinputitem *pi0=getItem0(index);
    //if(!pi){return false;}
    if(pi->type-myinputitem::func_Obj<0){return false;}
    //if(!pi->getParent(myinputitem::stc_Fun)){return false;}
    QStringList strlist;
    strlist=myfun::need(pi->getParent(myinputitem::func_Fun)->getstr());
    if(!strlist.isEmpty()){
        QString getstr=strlist.at(pi->type-myinputitem::func_Obj);
        if(!myobj::b4input(myobj::str2type(getstr))){return false;}
        if(myobj::str2type(getstr)==myobj::Mynum){
            bool b;
            value.toString().toInt(&b);
            if(!b){return false;}
        }
        pi0->pf->newChild(value,pi->type-myinputitem::func_Obj<strlist.length()-1);
        pi->pf->newChild(value,pi->type-myinputitem::func_Obj<strlist.length()-1);
        QModelIndex it=createIndex(pi->pf->pchlist.length()-1,0,pi->pf->pchlist.last());
        emit dataChanged(it,it);
        return true;
    }
    return false;
    /*
    if(getType()==mydo::Sentence){
        if(pi->type-myinputitem::func_Obj<0){return false;}
        //if(!pi->getParent(myinputitem::stc_Fun)){return false;}
        QStringList strlist;
        strlist=myfun::need(pi->getParent(myinputitem::func_Fun)->getstr());
        if(!strlist.isEmpty()){
            QString getstr=strlist.at(pi->type-myinputitem::func_Obj);
            if(!myobj::b4input(getstr)){return false;}
            if(myobj::str2type(getstr)==myobj::Mynum){
                bool b;
                value.toString().toInt(&b);
                if(!b){return false;}
            }
            pi0->pf->newChild(value,pi->type-myinputitem::func_Obj<strlist.length()-1);
            pi->pf->newChild(value,pi->type-myinputitem::func_Obj<strlist.length()-1);
            QModelIndex it=createIndex(pi->pf->pchlist.length()-1,0,pi->pf->pchlist.last());
            emit dataChanged(it,it);
            return true;
        }
    }
    else if(getType()==mydo::Condition){
        if(pi->type-myinputitem::condition_Obj<0){return false;}
        QString geteventstr=pi->getParent(myinputitem::condition_Event)->getstr();
        myobj *pobj=getsys()->psk0->findObjByName(pi->getParent(myinputitem::condition_Obj0)->getstr(),geteventstr);
        if(!pobj){return false;}
        if(myobj::b4input(pobj->type)){
            if(pobj->type==myobj::Mynum){
                bool b;
                value.toString().toInt(&b);
                if(!b){return false;}
            }
            pi0->pf->newChild(value,true);
            pi->pf->newChild(value,true);
            QModelIndex it=createIndex(pi->pf->pchlist.length()-1,0,pi->pf->pchlist.last());
            emit dataChanged(it,it);
            return true;
        }
    }
    */
}

Qt::ItemFlags myinputmodel::flags(const QModelIndex &index) const{
    if (!index.isValid()){return 0;}
    if(getItem(index)->getstr().startsWith(">")){return Qt::NoItemFlags;}
    Qt::ItemFlags r=Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(getItem(index)->getstr()=="input others"){
        myinputitem *pi=getItem(index);
        if(pi->type-myinputitem::func_Obj<0){return false;}
        QStringList strlist;
        strlist=myfun::need(pi->getParent(myinputitem::func_Fun)->getstr());
        if(!strlist.isEmpty()){
            QString getstr=strlist.at(pi->type-myinputitem::func_Obj);
            if(!myobj::b4input(myobj::str2type(getstr))){return Qt::NoItemFlags;}
            else{r=r|Qt::ItemIsEditable;}
        }
    }
    return r;
}

QVariant myinputmodel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(QString("header%1").arg(section));

    return QVariant();
}
