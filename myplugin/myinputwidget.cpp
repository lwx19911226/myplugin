#include <QPushButton>
#include <qmath.h>
//#include <QStyleOption>
#include "myinputwidget.h"
#include "myinputmodel.h"

myinputwidget::myinputwidget(MainWindow *getpmain,QWidget *parent):QWidget(parent){
    pmain=getpmain;seltype=0;b4filter=true;

    p_lineedit_filter=new QLineEdit(this);
    QObject::connect(p_lineedit_filter,SIGNAL(textEdited(QString)),this,SLOT(filterItems()));
    QLabel *p_label0=new QLabel(tr("Current Skill:")+" "+pmain->psys->psk0->name+" | "+pmain->psys->psk0->translation,this);
    QFormLayout *p_formlayout=new QFormLayout;
    p_formlayout->addRow(tr("Filter:"),p_lineedit_filter);

    p_combobox_filter=new QComboBox(this);
    p_combobox_filter->hide();    
    QObject::connect(p_combobox_filter,SIGNAL(currentIndexChanged(QString)),this,SLOT(filterItems_obj()));

    QHBoxLayout *p_hboxlayout=new QHBoxLayout;    
    p_hboxlayout->addLayout(p_formlayout);
    p_hboxlayout->addWidget(p_combobox_filter);

    p_inputmodel=new myinputmodel(pmain);    

    p_columnview=new QColumnView(this);
    //p_columnview->setDragDropMode(QAbstractItemView::DragDrop);
    p_columnview->setModel(p_inputmodel);
    QList<int> wlist;
    for(int i=0;i<10;i++){wlist<<160;}
    //wlist<<200<<200<<200<<200<<200<<200<<200<<200<<200<<200;
    p_columnview->setColumnWidths(wlist);
    p_columnview->setSelectionMode(QAbstractItemView::SingleSelection);
    QObject::connect(p_columnview,SIGNAL(clicked(QModelIndex)),this,SLOT(changeColumnView()));
    QObject::connect(p_columnview,SIGNAL(clicked(QModelIndex)),this,SLOT(mytest(QModelIndex)));

    p_textedit=new QTextEdit(this);
    p_textedit->setLineWrapMode(QTextEdit::NoWrap);
    p_textedit->setReadOnly(true);

    QVBoxLayout *p_vboxlayout=new QVBoxLayout(this);
    p_vboxlayout->addWidget(p_label0);
    p_vboxlayout->addLayout(p_hboxlayout);
    p_vboxlayout->addWidget(p_columnview);
    QWidget *p_widget1=new QWidget(this);
    p_widget1->setLayout(p_vboxlayout);
    QSplitter *p_splitter=new QSplitter(this);
    p_splitter->setOrientation(Qt::Vertical);
    p_splitter->addWidget(p_widget1);
    p_splitter->addWidget(p_textedit);

    QVBoxLayout *p_vboxlayout2=new QVBoxLayout(this);
    p_vboxlayout2->addWidget(p_splitter);
    p_formlayout=new QFormLayout();
    QLabel *p_label;
    QLineEdit *p_lineedit;
    for(int i=0;i<10;i++){
        p_label=new QLabel(this);
        p_lineedit=new QLineEdit(this);
        p_label_list.append(p_label);
        p_lineedit_list.append(p_lineedit);
        p_label->hide();p_lineedit->hide();
        p_formlayout->addRow(p_label,p_lineedit);
    }
    p_vboxlayout2->addLayout(p_formlayout);

    QPushButton *p_pushbutton_add=new QPushButton(tr("Skill_Add"),this);
    QObject::connect(p_pushbutton_add,SIGNAL(clicked()),this,SLOT(myadd()));
    //QPushButton *p_pushbutton_ptg=new QPushButton(tr("Pattern Generator"),this);
    //QObject::connect(p_pushbutton_ptg,SIGNAL(clicked()),this,SLOT(myptg()));
    QHBoxLayout *p_hboxlayout2=new QHBoxLayout;
    p_hboxlayout2->addWidget(p_pushbutton_add);
    //p_hboxlayout2->addWidget(p_pushbutton_ptg);

    p_vboxlayout2->addLayout(p_hboxlayout2);
    setLayout(p_vboxlayout2);
    setAutoFillBackground(true);

    QObject::connect(p_inputmodel,SIGNAL(myscroll(QModelIndex)),this,SLOT(myscroll(QModelIndex)));
}

myinputwidget::~myinputwidget(){
    delete p_columnview;
    delete p_inputmodel;    
    delete p_textedit;
    foreach(QLineEdit *ip,p_lineedit_list){delete ip;}
    foreach(QLabel *ip,p_label_list){delete ip;}
}

QStringList myinputwidget::getSel(){
    QStringList list;
    myinputitem *pi=p_inputmodel->getItem(p_columnview->currentIndex());
    while(pi&&(pi->getstr()!="root")){
        list.prepend(pi->getstr());
        pi=pi->pf;
    }
    return list;
}

void myinputwidget::myadd(){
    //qWarning()<<p_inputmodel->getItem(p_columnview->currentIndex())->getstr();
    QStringList list=getSel();
    if(list.length()<3){return;}
    if(list.last()==myinputitem::str4input()){return;}
    QString geteventstr=list.at(0);
    QString getblockstr=list.at(1).split("|").at(0);
    QString getfunstr=list.at(2);
    if(myfun::need(getfunstr).length()!=list.length()-3){return;}
    QList<myobj *> getobjlist;
    QStringList getrtrmlist;
    QStringList getblrmlist;
    for(int i=3;i<list.length();i++){
        myobj *pobj=pmain->psys->psk0->findObjByName(list.at(i),geteventstr);
        if(!pobj){
            int gettype=myobj::str2type(myfun::need(getfunstr).at(i-3));
            if(!myobj::b4input(gettype)){qWarning()<<"myadd"<<gettype;}
            pobj=new myobj(pmain->psys);
            pobj->name=list.at(i);
            pobj->type=gettype;
            pobj->isVerified=true;
            //pobj->isDynamic=false;
        }
        getobjlist.append(pobj);
    }
    int n=myfun::get(getfunstr).length();
    for(int i=1;i<=n;i++){getrtrmlist<<p_lineedit_list.at(i)->text();}
    for(int i=1;i<=myfun::getBlock_cnt(getfunstr);i++){getblrmlist<<p_lineedit_list.at(n+i+1)->text();}
    pmain->psys->psk0->addFunction(geteventstr,getblockstr,getfunstr,getobjlist,getrtrmlist,getblrmlist);
    pmain->myexport_design("tmp4design.txt");
    close();
}

QString myinputwidget::getRemark(myinputitem *pi){
    int i=pi->type;
    if(i==myinputitem::func_Event){
        return pmain->psys->psk0->findRemarkByName_event(pi->getstr());
    }
    else if(i==myinputitem::func_Block){
        return pmain->psys->psk0->findRemarkByName_block(pi->getstr());
    }
    else if(i==myinputitem::func_Fun){
        return myfun::findRemarkByName(pi->getstr());
    }
    else if(i>=myinputitem::func_Obj){
        return pmain->psys->psk0->findRemarkByName_obj(pi->getstr(),pi->getParent(myinputitem::func_Event)->getstr(),true);
        /*
        QString tstr;
        myobj *pt=pmain->psys->psk0->findObjByName(pi->getstr(),pi->getParent(myinputitem::func_Event)->getstr());
        if(pt){tstr+=pt->remark;}
        myfunction *pfunc=pmain->psys->psk0->findFuncByObj(pt);
        if(pfunc){tstr+="  /from:"+myfun::findRemarkByName(pfunc->funname);}
        return tstr;
        */
    }
    return "";
}

void myinputwidget::showRemark(){
    QStringList strlist;
    myinputitem *pi=p_inputmodel->getItem(p_columnview->currentIndex());
    if(!pi){return;}
    if(pi&&(pi->type==0||myinputitem::spstr(pi->getstr()))){return;}
    while(pi->pf){
        strlist.prepend(myinputitem::type2str(pi->type)+tr(": ")+"<"+pi->getstr()+"> "+getRemark(pi));
        //strlist.prepend(QString("%1 : %2").arg(pi->getstr()).arg(getRemark(pi)));
        pi=pi->pf;
    }
    strlist.prepend(tr("Remarks: "));
    p_textedit->setText(strlist.join("\n"));
    //qWarning()<<"showremark";
}

void myinputwidget::changeRTR(){
    QStringList list=getSel();
    if(!list.isEmpty()&&list.last()==myinputitem::str4input()){return;}
    foreach(QLabel *ip,p_label_list){ip->hide();}
    foreach(QLineEdit *ip,p_lineedit_list){ip->hide();}
    if(list.isEmpty()){return;}
    if(list.length()<myinputitem::func_Fun){return;}
    QString getfunstr=list.at(myinputitem::func_Fun-1);
    QStringList rtlist;
    rtlist=myfun::get(getfunstr);
    if(!rtlist.isEmpty()){
        p_label_list.at(0)->setText(tr("Remarks for return values: "));
        p_label_list.at(0)->show();
        for(int i=0;i<rtlist.length();i++){
            p_label_list.at(i+1)->setText(tr("ReturnValue")+QString::number(i+1)+" "+myobj::gettypestr(rtlist.at(i))+tr(": "));
            p_label_list.at(i+1)->show();
            p_lineedit_list.at(i+1)->show();
        }
    }
    int n=myfun::getBlock_cnt(getfunstr);
    if(n>0){
        int nn=rtlist.length()+1;
        p_label_list.at(nn)->setText(tr("Remarks for blocks: "));
        p_label_list.at(nn)->show();
        for(int i=0;i<n;i++){
            p_label_list.at(nn+i+1)->setText(tr("Block")+QString::number(i+1)+tr(": "));
            p_label_list.at(nn+i+1)->show();
            p_lineedit_list.at(nn+i+1)->show();
        }
    }
    //qWarning()<<"changertr";    
}
void myinputwidget::changeFilter(){
    //qWarning()<<"0228changefilter0";
    myinputitem *pi=p_inputmodel->getItem(p_columnview->currentIndex());
    int gettype=pi->type;
    if(pi->type!=seltype){
        b4filter=false;
        p_lineedit_filter->setText("");
        if(!pi->pchlist.isEmpty()){gettype++;}        
        if(gettype==myinputitem::func_Fun){
            p_combobox_filter->clear();
            p_combobox_filter->addItem(tr("ALL"));
            p_combobox_filter->addItems(myfun::getfuntagrmlist());
            p_combobox_filter->setCurrentIndex(0);
            p_combobox_filter->setMaxVisibleItems(p_combobox_filter->count());
            p_combobox_filter->show();
        }
        else if(gettype>=myinputitem::func_Obj){
            if(seltype>=myinputitem::func_Obj){
                p_combobox_filter->setCurrentIndex(0);
            }
            else{
                p_combobox_filter->clear();
                p_combobox_filter->addItem(tr("ALL"));
                p_combobox_filter->addItem(tr("Const"));
                p_combobox_filter->addItem(tr("Variable"));
                p_combobox_filter->addItems(myobj::getconsttaglist());
                p_combobox_filter->setCurrentIndex(0);
                p_combobox_filter->setMaxVisibleItems(p_combobox_filter->count());
            }
            p_combobox_filter->show();
        }
        else{
            p_combobox_filter->hide();
        }
        b4filter=true;
    }
    seltype=pi->type;    
}
void myinputwidget::changeColumnView(){
    showRemark();//qWarning()<<"showRemark";
    changeRTR();//qWarning()<<"changeRTR";
    changeFilter();//qWarning()<<"changeFilter";
    //qWarning()<<"changecolumnview";
}
/*
void myinputwidget::changeColumnViewWidths(){
    myinputitem *pi=p_inputmodel->getItem(p_columnview->currentIndex());
    if(!pi){return;}
    if(pi->type==0){return;}
    qWarning()<<"changewidth "<<pi->getstr();
    QList<int> widthlist;
    while(pi->pf){
        int max=128;
        foreach(myinputitem *ip,pi->pchlist){
            max=qMax(max,(int)(ip->getstr().length()*6.8));
        }
        widthlist.prepend(max);
        pi=pi->pf;
    }
    qWarning()<<widthlist;
    p_columnview->setColumnWidths(widthlist);
}
*/
void myinputwidget::filterItems(){
    if(!b4filter){return;}
    QRegExp reg(".*"+p_lineedit_filter->text()+".*",Qt::CaseInsensitive);
    if(!reg.isValid()){return;}
    myinputitem *pi0=p_inputmodel->getItem0(p_columnview->currentIndex());
    myinputitem *pi=p_inputmodel->getItem(p_columnview->currentIndex());
    //if(pi0->type==myinputitem::func_Start){return;}

    myinputitem *p0,*p;
    if(pi->pchlist.isEmpty()){p0=pi0->pf;p=pi->pf;}
    else{p0=pi0;p=pi;}
    p_columnview->hide();
    //qWarning()<<"filteritem?";
    if(p->type>myinputitem::func_Start){p_columnview->setCurrentIndex(p_inputmodel->getIndex(p,true));}
    else{p_columnview->setCurrentIndex(QModelIndex());}
    //qWarning()<<"filteritem";
    foreach(myinputitem *ip,p0->pchlist){
        if(myinputitem::spstr(ip->getstr())){continue;}
        ip->visible=reg.exactMatch(ip->getstr())||reg.exactMatch(getRemark(ip));
    }
    p->delAllChildren();
    myinputitem::mycpy(p0,p,true);
    if(!p->pchlist.isEmpty()){myscroll(p_inputmodel->getIndex(p->pchlist.first()));}
    p_columnview->show();
}
void myinputwidget::filterItems_obj(){
    if(!b4filter){return;}
    myinputitem *pi0=p_inputmodel->getItem0(p_columnview->currentIndex());
    myinputitem *pi=p_inputmodel->getItem(p_columnview->currentIndex());
    if(pi0->type==0){return;}
    myinputitem *p0,*p;
    if(pi->pchlist.isEmpty()){p0=pi0->pf;p=pi->pf;}
    else{p0=pi0;p=pi;}
    if(p0->type+1<myinputitem::func_Fun){return;}    
    p_columnview->hide();
    p_columnview->setCurrentIndex(p_inputmodel->getIndex(p,true));
    //qWarning()<<"filteritem_const";
    foreach(myinputitem *ip,p0->pchlist){
        if(myinputitem::spstr(ip->getstr())){continue;}
        if(p_combobox_filter->currentText()==tr("ALL")){ip->visible=true;continue;}
        QStringList gettaglist=pmain->psys->psk0->funtaglist(ip->getParent(myinputitem::func_Event)->getstr());
        gettaglist<<myfun::remark2tag(p_combobox_filter->currentText());
        if(p0->type+1==myinputitem::func_Fun){
            ip->visible=myfun::matchTaglist(ip->getstr(),gettaglist);
        }
        else if(p0->type+1>=myinputitem::func_Obj){
            myobj *pt=pmain->psys->psk0->findObjByName(ip->getstr(),ip->getParent(myinputitem::func_Event)->getstr());
            if(pt){
                bool b=false;
                foreach(QString stri,myobj::myconstlist){
                    if(stri.startsWith(pt->name+"|")){b=true;break;}
                }
                if(p_combobox_filter->currentText()==tr("Const")){ip->visible=b;}
                else if(p_combobox_filter->currentText()==tr("Variable")){ip->visible=!b;}
                else{ip->visible=myobj::matchTag(ip->getstr(),myobj::remark2tag(p_combobox_filter->currentText()));}
            }
        }
    }    
    p->delAllChildren();
    myinputitem::mycpy(p0,p,true);
    if(!p->pchlist.isEmpty()){myscroll(p_inputmodel->getIndex(p->pchlist.first()));}
    p_columnview->show();
}

void myinputwidget::myscroll(const QModelIndex &index){
    p_columnview->scrollTo(index);
}

void myinputwidget::mytest(const QModelIndex &index){
    qWarning()<<p_inputmodel->getItem0(index)->getstr();
}
