#include "myinputmodel.h"
//#include "mainwindow.h"

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

    rootitem0->newChild(QVariant(">"+myinputitem::type2str(myinputitem::func_Event)+tr(": ")));
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
    myinputitem *p0=getItem0(parent);
    if(!p0){return false;}
    if(!p0->pf){return false;}
    //if(p0&&!p0->pchlist.isEmpty()){return true;}
    if(p0->pchlist.length()==1){return true;}
    return false;
}

void myinputmodel::fetchMore(const QModelIndex &parent){
    //qWarning()<<"fetchmore0";
    myinputitem *pp=getItem0(parent);
    myinputitem *p=getItem(parent);
    if(pp->type+1==myinputitem::func_Block){
        pp->newChild(QVariant(">"+myinputitem::type2str(myinputitem::func_Block)+tr(": ")),false,true);
        QString geteventstr=pp->getstr();
        //QStringList list;
        //list=getsys()->psk0->need4block(geteventstr);
        //if(list.isEmpty()){list.append(geteventstr);}
        foreach(QString stri,getsys()->psk0->blockstrlist(geteventstr)){
            pp->newChild(QVariant(stri),true);
        }
    }
    else if(pp->type+1==myinputitem::func_Fun){
        QString geteventstr=pp->getParent(myinputitem::func_Event)->getstr();
        pp->newChild(QVariant(">"+myinputitem::type2str(myinputitem::func_Fun)+tr(": ")),false,true);
        QStringList tstrlist=getsys()->psk0->funtaglist(geteventstr);
        tstrlist<<mysys::qsv2str(getsys()->qsv)+"$";
        foreach(QString stri,myfun::getfunstrlist(getsys()->qsv,tstrlist)){
            pp->newChild(QVariant(stri),!myfun::intypestrlist(stri,getsys()->qsv).isEmpty());
        }
    }
    else if(pp->type+1>=myinputitem::func_Obj){
        //if(!pp->getParent(myinputitem::stc_Fun,true)){return;}
        QString geteventstr=pp->getParent(myinputitem::func_Event)->getstr();
        QString getfunstr=pp->getParent(myinputitem::func_Fun,true)->getstr();
        QStringList strlist;
        strlist=myfun::intypestrlist(getfunstr,getsys()->qsv);
        if(!strlist.isEmpty()){
            QList<myobj *> objlist;
            int geti=pp->type+1-myinputitem::func_Obj;
            QString gettypestr=myobj::gettypestr(strlist.at(geti));
            pp->newChild(QVariant(">"+myinputitem::type2str(myinputitem::func_Obj)+QString::number(geti+1)+" "+gettypestr+tr(": ")),false,true);
            foreach(QString stri,gettypestr.split("+")){
                getsys()->psk0->getavlobjlist(myobj::str2type(stri),objlist,geteventstr,!myfun::notnil(getfunstr,getsys()->qsv,geti));
            }
            foreach(myobj *ip,objlist){
                pp->newChild(QVariant(ip->name),geti<strlist.length()-1);
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
    strlist=myfun::intypestrlist(pi->getParent(myinputitem::func_Fun)->getstr(),getsys()->qsv);
    if(!strlist.isEmpty()){
        QString getstr=strlist.at(pi->type-myinputitem::func_Obj);
        int gettype=myobj::str2type(getstr);
        if(!myobj::b4input(gettype)){return false;}
        if(gettype==myobj::mynum){
            bool b;
            value.toString().toInt(&b);
            if(!b){return false;}
        }
        pi0->pf->newChild(value,pi->type-myinputitem::func_Obj<strlist.length()-1,false);
        myinputitem *pt=pi->pf->newChild(value,pi->type-myinputitem::func_Obj<strlist.length()-1,false);
        QModelIndex ited=createIndex(pi0->pf->pchlist.length()-1,0,pi0->pf->pchlist.last());
        QModelIndex itst=createIndex(0,0,pi0->pf->pchlist.first());
        QModelIndex itt=createIndex(pt->getRank(true),0,pt);
        emit dataChanged(itst,ited);
        emit myscroll(itt);
        return true;
    }
    return false;
}

Qt::ItemFlags myinputmodel::flags(const QModelIndex &index) const{
    if (!index.isValid()){return 0;}
    if(getItem(index)->getstr().startsWith(">")){return Qt::NoItemFlags;}
    Qt::ItemFlags r=Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(getItem(index)->getstr()==myinputitem::str4input()){
        myinputitem *pi=getItem(index);
        if(pi->type-myinputitem::func_Obj<0){return false;}
        QStringList strlist;
        strlist=myfun::intypestrlist(pi->getParent(myinputitem::func_Fun)->getstr(),getsys()->qsv);
        if(!strlist.isEmpty()){
            QString getstr=strlist.at(pi->type-myinputitem::func_Obj);
            int gettype=myobj::str2type(getstr);
            if(!myobj::b4input(gettype)){return Qt::NoItemFlags;}
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
