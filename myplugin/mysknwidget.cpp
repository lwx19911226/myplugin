#include "mysknwidget.h"

bool myshorter(const QString &s1,const QString &s2){
    return s1.split("|").last().length()<s2.split("|").last().length();
}
QTableWidgetItem *mynewitem(QString getstr){
    QTableWidgetItem *pitem=new QTableWidgetItem(getstr);
    pitem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    return pitem;
}

mysknwidget::mysknwidget(MainWindow *getMain,QWidget *parent):QWidget(parent){
    pmain=getMain;
    setWindowTitle(tr("Skill Name List"));

    p_tablewidget=new QTableWidget(this);
    p_tablewidget->setRowCount(myobj::myconstskstrlist.length()+pmain->psys->sklist.length());
    p_tablewidget->setColumnCount(3);
    p_tablewidget->setSelectionMode(QAbstractItemView::SingleSelection);
    QStringList tstrlist;
    tstrlist<<tr("Name")<<tr("Translation")<<tr("Description");
    p_tablewidget->setHorizontalHeaderLabels(tstrlist);
    int rowi=0;
    foreach(mysk *ip,pmain->psys->sklist){
        p_tablewidget->setItem(rowi,0,mynewitem(ip->name));
        p_tablewidget->setItem(rowi,1,mynewitem(ip->translation));
        p_tablewidget->setItem(rowi,2,mynewitem(ip->description));
        rowi++;
    }
    QStringList tskstrlist(myobj::myconstskstrlist);
    qSort(tskstrlist.begin(),tskstrlist.end(),myshorter);
    foreach(QString stri,tskstrlist){
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
