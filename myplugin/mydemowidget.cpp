#include "mydemowidget.h"
#include "myinputitem.h"

mydemowidget::mydemowidget(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("Demo"));
    psys=NULL;
    p_combobox_flist=new QComboBox(this);
    QDir dir("demo");
    foreach(QString stri,dir.entryList(QStringList("*.txt"))){
        flist<<"demo/"+stri;
        p_combobox_flist->addItem(stri.mid(0,stri.length()-4));
    }
    QObject::connect(p_combobox_flist,SIGNAL(currentIndexChanged(QString)),this,SLOT(myload_cbb()));
    QFormLayout *p_formlayout_load=new QFormLayout;
    p_formlayout_load->addRow(tr("Design")+tr(": "),p_combobox_flist);
    QPushButton *p_pushbutton_load=new QPushButton(tr("Load Other Design"),this);
    QObject::connect(p_pushbutton_load,SIGNAL(clicked()),this,SLOT(myload_btn()));
    QHBoxLayout *p_hboxlayout_load=new QHBoxLayout;
    p_hboxlayout_load->addLayout(p_formlayout_load);
    p_hboxlayout_load->addWidget(p_pushbutton_load);
    p_label_qsv=new QLabel(this);
    QFormLayout *p_formlayout_qsv=new QFormLayout;
    p_formlayout_qsv->addRow(tr("Version")+tr(": "),p_label_qsv);
    p_combobox_sklist=new QComboBox(this);
    QObject::connect(p_combobox_sklist,SIGNAL(currentIndexChanged(int)),this,SLOT(changeSkill()));
    QFormLayout *p_formlayout_sk=new QFormLayout;
    p_formlayout_sk->addRow(tr("Skill")+tr(": "),p_combobox_sklist);
    //p_label_sk=new QLabel(this);
    //p_label_sk->setWordWrap(true);
    p_textedit_sk=new QTextEdit(this);
    p_textedit_sk->setReadOnly(true);
    p_textedit_sk->setWordWrapMode(QTextOption::NoWrap);
    QLabel *p_label=new QLabel(tr("Procedure")+tr(": "),this);
    p_listwidget=new QListWidget(this);
    QObject::connect(p_listwidget,SIGNAL(currentRowChanged(int)),this,SLOT(myrfr_do()));
    p_textedit_do=new QTextEdit(this);
    p_textedit_do->setReadOnly(true);
    p_textedit_do->setWordWrapMode(QTextOption::NoWrap);
    QVBoxLayout *p_vboxlayout=new QVBoxLayout(this);
    p_vboxlayout->addLayout(p_hboxlayout_load);
    p_vboxlayout->addLayout(p_formlayout_qsv);
    p_vboxlayout->addLayout(p_formlayout_sk);
    p_vboxlayout->addWidget(p_textedit_sk);
    p_vboxlayout->addWidget(p_label);
    p_vboxlayout->addWidget(p_listwidget);
    p_vboxlayout->addWidget(p_textedit_do);

    if(p_combobox_flist->count()>0){myload_cbb();}
}

void mydemowidget::myload_cbb(){
    QString tstr=p_combobox_flist->currentText();
    if(tstr!=""){
        //qWarning()<<"130709here";
        myload(flist.at(p_combobox_flist->currentIndex()));
    }
}
void mydemowidget::myload_btn(){
    QString path=QFileDialog::getOpenFileName(this,"DESIGN FILE",".","design files (*.txt)");
    if(path!=""){
        flist<<path;
        QString tstr=path.split("/").last();
        p_combobox_flist->addItem(tstr.mid(0,tstr.length()-4));
        p_combobox_flist->setCurrentIndex(p_combobox_flist->count()-1);
    }
}
void mydemowidget::myload(QString getpath){
    if(psys){delete psys;}
    psys=new mysys(this);
    QObject::connect(psys,SIGNAL(update()),this,SLOT(myrfr()));
    psys->myini(getpath,mysys::VersionUnknown);
}

void mydemowidget::myrfr(){
    b4rfr=true;
    p_label_qsv->setText(mysys::qsv2str(psys->qsv));
    p_combobox_sklist->clear();
    foreach(mysk *ip,psys->sklist){
        p_combobox_sklist->addItem(ip->name+"|"+ip->translation);
    }
    myrfr_sk();
    b4rfr=false;
}
void mydemowidget::myrfr_sk(){
    p_textedit_sk->setText(QString());
    p_listwidget->clear();
    dolist4show.clear();
    if(psys&&psys->psk0){
        QStringList tstrlist;
        QMap<QString,QString> strmap;
        psys->psk0->propertymap_get(strmap,true);
        foreach(QString stri,psys->psk0->propertystrlist()){
            tstrlist<<stri+tr(": ")+mycode::mysplit(strmap.value(stri)).first();
        }
        tstrlist.prepend(tr("Skill Category")+tr(": ")+mysk::type2str(psys->psk0->getType()));
        p_textedit_sk->setText(tstrlist.join("\n"));
        foreach(mydo *ip,psys->dolist){
            if(ip->psktgt==psys->psk0){
                dolist4show<<ip;
                p_listwidget->addItem(ip->blocklist.first()->getRemark());
            }
        }
    }
    myrfr_do();
}
void mydemowidget::myrfr_do(){
    p_textedit_do->setText(QString());
    QListWidgetItem *pitem=p_listwidget->currentItem();
    if(pitem&&pitem->text()!=""){
        mydo *pdo=dolist4show.at(p_listwidget->currentRow());
        myfunction *pfunc=static_cast<myfunction *>(pdo->blocklist.first());
        QStringList tstrlist;
        QString eventstr=pfunc->getEvent();
        QString blockstr=pfunc->upperLayer->name;
        tstrlist<<myinputitem::type2str(myinputitem::func_Event)+tr(": ")+"<"+eventstr+"> "+psys->psk0->findRemarkByName_event(eventstr);
        tstrlist<<myinputitem::type2str(myinputitem::func_Block)+tr(": ")+"<"+blockstr+"> "+psys->psk0->findRemarkByName_block(blockstr);
        tstrlist<<myinputitem::type2str(myinputitem::func_Fun)+tr(": ")+"<"+pfunc->funname+"> "+myfun::name2remark(pfunc->funname,psys->qsv);
        for(int i=0;i<pfunc->objlist.length();i++){
            myobj *pobj=pfunc->objlist.at(i);
            tstrlist<<myinputitem::type2str(myinputitem::func_Obj)+QString::number(i+1)+" "+myobj::type2str(pobj->type)+tr(": ")+"<"+pobj->name+"> "+psys->psk0->findRemarkByName_obj(pobj,psys->findFuncByObj(pobj),true);
        }
        for(int i=0;i<pfunc->rtobjlist.length();i++){
            myobj *pobj=pfunc->rtobjlist.at(i);
            tstrlist<<tr("ReturnValue")+QString::number(i+1)+tr(": ")+pobj->remark;
        }
        for(int i=0;i<pfunc->blocklist.length();i++){
            myblock *pblock=pfunc->blocklist.at(i);
            tstrlist<<tr("Block")+QString::number(i+1)+tr(": ")+pblock->remark;
        }
        p_textedit_do->setText(tstrlist.join("\n"));
    }
}

void mydemowidget::changeSkill(){
    if(b4rfr){return;}
    QString tstr=p_combobox_sklist->currentText();
    if(tstr==""){return;}
    tstr=tstr.split("|").first();
    if(psys->psk0&&psys->psk0->name==tstr){return;}
    mysk *psk=psys->findSkillByName(tstr);
    if(!psk){qWarning()<<"changeSkill";}
    else{
        psys->psk0=psk;
        myrfr_sk();
    }
}












