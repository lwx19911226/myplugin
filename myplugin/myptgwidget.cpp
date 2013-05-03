#include "myptgwidget.h"

myptgwidget::myptgwidget(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("Pattern Generator"));
    QStringList cllist=myobj::getconstlist_tag("cl");
    QStringList suitlist=myobj::getconstlist_tag("suit");
    p_tablewidget=new QTableWidget(this);
    int rowmax=qMax(qMax(13,cllist.length()),suitlist.length());
    p_tablewidget->setRowCount(rowmax);
    p_tablewidget->setColumnCount(4);
    QStringList headerlist;
    headerlist<<tr("Class")<<tr("Suit")<<tr("Number")<<tr("Place");
    p_tablewidget->setHorizontalHeaderLabels(headerlist);

    for(int i=0;i<cllist.length();i++){
        p_tablewidget->setItem(i,0,new QTableWidgetItem(cllist.at(i)));
    }
    for(int i=0;i<suitlist.length();i++){
        p_tablewidget->setItem(i,1,new QTableWidgetItem(suitlist.at(i)));
    }
    for(int i=0;i<13;i++){
        p_tablewidget->setItem(i,2,new QTableWidgetItem(QString::number(i+1)));
    }
    p_tablewidget->setItem(0,3,new QTableWidgetItem("hand"));
    p_tablewidget->setItem(1,3,new QTableWidgetItem("equipped"));
    p_tablewidget->setSelectionMode(QAbstractItemView::MultiSelection);
    p_tablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    p_checkbox_judge=new QCheckBox(tr("Judge"),this);
    p_checkbox_or=new QCheckBox(tr("Or"),this);
    QObject::connect(p_checkbox_judge,SIGNAL(clicked(bool)),p_checkbox_or,SLOT(setHidden(bool)));
    QPushButton *p_pushbutton_generate=new QPushButton(tr("Generate"),this);
    QObject::connect(p_pushbutton_generate,SIGNAL(clicked()),this,SLOT(mygenerate()));
    QPushButton *p_pushbutton_copy=new QPushButton(tr("Copy"),this);
    QObject::connect(p_pushbutton_copy,SIGNAL(clicked()),this,SLOT(mycopy()));
    QPushButton *p_pushbutton_del=new QPushButton(tr("Delete"),this);
    QObject::connect(p_pushbutton_del,SIGNAL(clicked()),this,SLOT(mydel()));
    QHBoxLayout *p_hboxlayout=new QHBoxLayout;
    p_hboxlayout->addWidget(p_checkbox_judge);
    p_hboxlayout->addWidget(p_checkbox_or);
    p_hboxlayout->addWidget(p_pushbutton_generate);
    p_hboxlayout->addWidget(p_pushbutton_del);
    p_hboxlayout->addWidget(p_pushbutton_copy);

    p_lineedit=new QLineEdit(this);
    p_lineedit->setReadOnly(true);
    QFormLayout *p_formlayout=new QFormLayout;
    p_formlayout->addRow(tr("Result:"),p_lineedit);

    QVBoxLayout *p_vboxlayout=new QVBoxLayout(this);
    p_vboxlayout->addWidget(p_tablewidget);
    p_vboxlayout->addLayout(p_hboxlayout);
    p_vboxlayout->addLayout(p_formlayout);
}
void myptgwidget::mygenerate(){
    QList<QTableWidgetItem *> list=p_tablewidget->selectedItems();
    QList<QTableWidgetItem *> pcllist,psuitlist,pnumlist,pplacelist;
    foreach(QTableWidgetItem *ip,list){
        switch(ip->column()){
        case 0:pcllist<<ip;break;
        case 1:psuitlist<<ip;break;
        case 2:pnumlist<<ip;break;
        case 3:pplacelist<<ip;break;
        default:;
        }
    }

    if(p_checkbox_judge->isChecked()){
        QStringList suitstrlist,numstrlist;
        foreach(QTableWidgetItem *ip,psuitlist){suitstrlist<<myobj::suitstr(ip->text());}
        foreach(QTableWidgetItem *ip,pnumlist){numstrlist<<myobj::num2str(ip->text().toInt());}
        QString str4suit,str4num;
        if(suitstrlist.isEmpty()){str4suit=".*";}
        else{str4suit=suitstrlist.join("|");}
        if(numstrlist.isEmpty()){str4num=".*";}
        else{str4num=numstrlist.join("|");}
        ptstrlist.clear();
        ptstrlist<<QString("(.*):(%1):(%2)").arg(str4suit,str4num);
    }
    else{
        QStringList strlist;
        if(pcllist.isEmpty()){strlist<<".";}
        else{
            QStringList tstrlist;
            foreach(QTableWidgetItem *ip,pcllist){
                tstrlist<<ip->text();
            }
            strlist<<tstrlist.join(",");
        }
        if(psuitlist.isEmpty()){strlist<<".";}
        else{
            QStringList tstrlist;
            foreach(QTableWidgetItem *ip,psuitlist){
                tstrlist<<myobj::suitstr(ip->text());
            }
            strlist<<tstrlist.join(",");
        }
        if(pnumlist.isEmpty()){strlist<<".";}
        else{
            QStringList tstrlist;
            foreach(QTableWidgetItem *ip,pnumlist){
                tstrlist<<ip->text();
            }
            strlist<<tstrlist.join(",");
        }
        if(pplacelist.isEmpty()){}
        else{
            QStringList tstrlist;
            foreach(QTableWidgetItem *ip,pplacelist){
                tstrlist<<ip->text();
            }
            strlist<<tstrlist.join(",");
        }
        if(!p_checkbox_or->isChecked()){
            ptstrlist.clear();
            ptstrlist<<strlist.join("|");
        }
        else{
            if(!ptstrlist.isEmpty()&&ptstrlist.first().startsWith("(")){ptstrlist.clear();}
            ptstrlist<<strlist.join("|");
        }
    }


    p_lineedit->setText(ptstrlist.join("#"));

}
void myptgwidget::mycopy(){
    p_lineedit->copy();
    QApplication::clipboard()->setText(p_lineedit->text());
    QMessageBox::information(this,"COPY",p_lineedit->text()+" has been copied to the clipboard");
}
void myptgwidget::mydel(){
    if(!ptstrlist.isEmpty()){
        ptstrlist.takeLast();
        p_lineedit->setText(ptstrlist.join("#"));
    }
}
