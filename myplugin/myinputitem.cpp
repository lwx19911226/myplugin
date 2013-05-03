#include "myinputitem.h"
myinputitem *myinputitem::newChild(QVariant getdata,bool extra,bool front){
    myinputitem *p=new myinputitem;
    p->mydata=getdata;
    p->pf=this;
    p->type=type+1;
    if(!front){pchlist.append(p);}
    else if(spstr(p->getstr())){pchlist.prepend(p);}
    else{
        int geti=0;
        for(geti=0;geti<pchlist.length();geti++){
            if(!spstr(pchlist.at(geti)->getstr())){break;}
        }
        pchlist.insert(geti,p);
    }
    if(extra){pchlist.last()->newChild(QVariant(str4input()));}
    return p;
}
myinputitem *myinputitem::getChild(int i,bool visibility){
    //if(i>=0&&i<=pchlist.length()-1){return pchlist.at(i);}
    int cnt=0;
    foreach(myinputitem *ip,pchlist){
        if(visibility&&!ip->visible){continue;}
        cnt++;
        if(cnt==i+1){return ip;}
    }
    return NULL;
}
int myinputitem::getChildren_cnt(bool visibility){
    int cnt=0;
    foreach(myinputitem *ip,pchlist){
        if(visibility&&!ip->visible){continue;}
        cnt++;
    }
    return cnt;
}
int myinputitem::getRank(bool visibility){
    if(!pf){return 0;}
    int cnt=0;
    foreach(myinputitem *ip,pf->pchlist){
        if(visibility&&!ip->visible){continue;}
        cnt++;
        if(ip==this){break;}
    }
    return cnt-1;
}
myinputitem *myinputitem::getParent(int gettype,bool self){
    if(self&&(gettype==type)){return this;}
    if(gettype>type){return NULL;}
    if(!pf){return NULL;}
    return pf->getParent(gettype,true);
    //if(gettype>(type-(self?0:1))){return NULL;}
    //if(gettype==(type-(self?0:1))){return self?this:pf;}
    //return pf->getParent(gettype,self);
}
myinputitem *myinputitem::getParent(QString str, bool self){
    if(self&&(getstr()==str)){return this;}
    if(!pf){return NULL;}
    return pf->getParent(str,true);
}
myinputitem *myinputitem::search(myinputitem *getp){
    if(!getp){return NULL;}
    if(getp->type<type){return NULL;}
    if(sameValue(getp)){return this;}
    myinputitem *pt0=this,*pt=getp;
    QList<myinputitem *> plist;
    while(1){
        if(pt->type==type){break;}
        plist.prepend(pt);
        pt=pt->pf;
    }
    while(1){
        if(plist.isEmpty()){return pt0;}
        pt=plist.takeFirst();
        bool b=false;
        foreach(myinputitem *ip,pt0->pchlist){
            if(ip->sameValue(pt)){b=true;pt0=ip;break;}
        }
        if(!b){return NULL;}
    }
}

void myinputitem::mycpy(myinputitem *root0,myinputitem *root1,bool visibility){
    root1->mydata=root0->mydata;
    root1->type=root0->type;
    root1->visible=root0->visible;
    foreach(myinputitem *ip,root0->pchlist){
        if(visibility&&!ip->visible){continue;}
        root1->newChild(ip->mydata);
        mycpy(ip,root1->pchlist.last(),visibility);
    }
}
