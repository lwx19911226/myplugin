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
    p_tablewidget->setColumnCount(6);
    QStringList headerlist;
    headerlist<<tr("")<<tr("Class")<<tr("")<<tr("Suit")<<tr("Number")<<tr("Place");
    p_tablewidget->setHorizontalHeaderLabels(headerlist);
    QCheckBox *pcheckbox=NULL;
    for(int i=0;i<cllist.length();i++){
        pcheckbox=new QCheckBox(p_tablewidget);
        p_tablewidget->setCellWidget(i,0,pcheckbox);
        p_tablewidget->setItem(i,1,new QTableWidgetItem(cllist.at(i)));
    }
    for(int i=0;i<suitlist.length();i++){
        pcheckbox=new QCheckBox(p_tablewidget);
        p_tablewidget->setCellWidget(i,2,pcheckbox);
        p_tablewidget->setItem(i,3,new QTableWidgetItem(suitlist.at(i)));
    }
    for(int i=0;i<13;i++){
        p_tablewidget->setItem(i,4,new QTableWidgetItem(QString::number(i+1)));
    }
    p_tablewidget->setItem(0,5,new QTableWidgetItem("hand"));
    p_tablewidget->setItem(1,5,new QTableWidgetItem("equipped"));
    p_tablewidget->setSelectionMode(QAbstractItemView::MultiSelection);
    p_tablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    p_tablewidget->resizeColumnsToContents();

    QLabel *plabel=new QLabel(tr("Note: if the checkbox before Slash is checked, it means non-Slash."),this);

    p_checkbox_judge=new QCheckBox(tr("Judge"),this);
    p_checkbox_judge->hide();
    p_checkbox_or=new QCheckBox(tr("Or"),this);
    //QObject::connect(p_checkbox_judge,SIGNAL(clicked(bool)),p_checkbox_or,SLOT(setHidden(bool)));
    //QObject::connect(p_checkbox_judge,SIGNAL(clicked(bool)),this,SLOT(myjudge(bool)));
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
    p_vboxlayout->addWidget(plabel);
    p_vboxlayout->addLayout(p_hboxlayout);
    p_vboxlayout->addLayout(p_formlayout);
}
void myptgwidget::mygenerate(){
    QList<QTableWidgetItem *> list=p_tablewidget->selectedItems();
    QList<QTableWidgetItem *> pcllist,psuitlist,pnumlist,pplacelist;
    foreach(QTableWidgetItem *ip,list){
        switch(ip->column()){
        case 1:pcllist<<ip;break;
        case 3:psuitlist<<ip;break;
        case 4:pnumlist<<ip;break;
        case 5:pplacelist<<ip;break;
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
                QCheckBox *pcheckbox=static_cast<QCheckBox *>(p_tablewidget->cellWidget(ip->row(),0));
                QString str=ip->text();
                if(pcheckbox->isChecked()){
                    str="^"+str;
                }
                tstrlist<<str;
            }
            strlist<<tstrlist.join(",");
        }
        if(psuitlist.isEmpty()){strlist<<".";}
        else{
            QStringList tstrlist;
            foreach(QTableWidgetItem *ip,psuitlist){
                QCheckBox *pcheckbox=static_cast<QCheckBox *>(p_tablewidget->cellWidget(ip->row(),2));
                QString str=myobj::suitstr(ip->text());
                if(pcheckbox->isChecked()){
                    str="^"+str;
                }
                tstrlist<<str;
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
void myptgwidget::myjudge(bool checked){
    p_tablewidget->setColumnHidden(0,checked);
    p_tablewidget->setColumnHidden(1,checked);
    p_tablewidget->setColumnHidden(2,checked);
    p_tablewidget->setColumnHidden(5,checked);
    p_checkbox_or->setHidden(checked);
    mygenerate();
}
