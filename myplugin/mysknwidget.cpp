#include "mysknwidget.h"

QTableWidgetItem *mynewitem(QString getstr){
    QTableWidgetItem *pitem=new QTableWidgetItem(getstr);
    pitem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    return pitem;
}

mysknwidget::mysknwidget(MainWindow *getMain,QWidget *parent):QWidget(parent){
    pmain=getMain;
    setWindowTitle(tr("Skill Name List"));

    p_tablewidget=new QTableWidget(this);
    QList<mysk *> tsklist;
    pmain->psys->getsklist_noexs(tsklist);
    p_tablewidget->setRowCount(myobj::myconstskstrlist.length()+tsklist.length());
    p_tablewidget->setColumnCount(4);
    p_tablewidget->setSelectionMode(QAbstractItemView::SingleSelection);
    p_tablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList tstrlist;
    tstrlist<<tr("Name")<<tr("Translation")<<tr("Description")<<"";
    p_tablewidget->setHorizontalHeaderLabels(tstrlist);
    int rowi=0;
    foreach(mysk *ip,tsklist){
        p_tablewidget->setItem(rowi,0,mynewitem(ip->name));
        p_tablewidget->setItem(rowi,1,mynewitem(ip->translation));
        p_tablewidget->setItem(rowi,2,mynewitem(ip->description));
        rowi++;
    }
    //QStringList tskstrlist(myobj::myconstskstrlist);
    //qSort(tskstrlist.begin(),tskstrlist.end(),mycmp);
    foreach(QString stri,myobj::myconstskstrlist){
        QString tstri=stri;
        for(int j=0;j<3;j++){
            p_tablewidget->setItem(rowi,j,mynewitem(tstri.split("|").at(j)));
        }
        rowi++;
    }
    p_tablewidget->resizeColumnsToContents();

    QPushButton *p_pushbutton_cpy=new QPushButton(tr("Copy"),this);
    QObject::connect(p_pushbutton_cpy,SIGNAL(clicked()),this,SLOT(mycopy()));
    QVBoxLayout *p_vboxlayout=new QVBoxLayout(this);
    p_vboxlayout->addWidget(p_tablewidget);
    p_vboxlayout->addWidget(p_pushbutton_cpy);
}
void mysknwidget::mycopy(){
    QTableWidgetItem *pitem=p_tablewidget->item(p_tablewidget->currentRow(),0);
    if(!pitem){return;}
    QApplication::clipboard()->setText(pitem->text());
    QMessageBox::information(this,"COPY",pitem->text()+" has been copied to the clipboard");
}
