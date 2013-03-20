#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myinputwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("myplugin"));
    qsrand(QTime::currentTime().msec());
    myini_tips();
    tipbox();
    b4rfr=true;

    psys=new mysys(this);
    QString getpath;
    QFile ft("tmp4design.txt");
    if(ft.exists()&&(QMessageBox::question(this,"DESIGN FILE",
                                           "There exists temp design files left before, do you want to import it?",
                                          QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)){
        getpath="tmp4design.txt";
    }
    else{
        getpath=QFileDialog::getOpenFileName(NULL,"DESIGN FILE",".","design files (*.txt)");
    }
    psys->myini(getpath);

    p_lineedit_packagename=new QLineEdit(ui->centralWidget);
    QObject::connect(p_lineedit_packagename,SIGNAL(returnPressed()),this,SLOT(changePackageName()));
    QFormLayout *p_formlayout4=new QFormLayout;    
    p_formlayout4->addRow(tr("Package Name:"),p_lineedit_packagename);
    p_lineedit_packagetrans=new QLineEdit(ui->centralWidget);
    QObject::connect(p_lineedit_packagetrans,SIGNAL(returnPressed()),this,SLOT(changePackageTranslation()));
    QFormLayout *p_formlayout5=new QFormLayout;
    p_formlayout5->addRow(tr("Package Translation:"),p_lineedit_packagetrans);
    QHBoxLayout *p_hboxlayout1=new QHBoxLayout;
    p_hboxlayout1->addLayout(p_formlayout4);
    p_hboxlayout1->addLayout(p_formlayout5);

    QTableWidget *p_tablewidget_g=new QTableWidget;
    p_tablewidget_g->setColumnCount(mygeneral::propertystrlist().length());
    p_tablewidget_g->setSelectionMode(QTableWidget::SingleSelection);    
    p_tablewidget_g->setHorizontalHeaderLabels(mygeneral::propertystrlist());
    QObject::connect(p_tablewidget_g,SIGNAL(itemSelectionChanged()),this,SLOT(itemsel()));
    QObject::connect(p_tablewidget_g,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(itemchanged_g(QTableWidgetItem*)));
    //QObject::connect(p_tablewidget_g,SIGNAL(cellChanged(int,int)),this,SLOT(cellchanged_g(int,int)));
    QTableWidget *p_tablewidget_trs=new QTableWidget;
    p_tablewidget_trs->setColumnCount(mytrs::propertystrlist(true).length());
    p_tablewidget_trs->setSelectionMode(QTableWidget::SingleSelection);    
    p_tablewidget_trs->setHorizontalHeaderLabels(mytrs::propertystrlist(true));
    QObject::connect(p_tablewidget_trs,SIGNAL(itemSelectionChanged()),this,SLOT(itemsel()));
    QObject::connect(p_tablewidget_trs,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(itemchanged_sk(QTableWidgetItem*)));
    //QObject::connect(p_tablewidget_trs,SIGNAL(cellChanged(int,int)),this,SLOT(cellchanged_trs(int,int)));
    QTableWidget *p_tablewidget_vs=new QTableWidget;
    p_tablewidget_vs->setColumnCount(myvs::propertystrlist(true).length());
    p_tablewidget_vs->setSelectionMode(QTableWidget::SingleSelection);
    p_tablewidget_vs->setHorizontalHeaderLabels(myvs::propertystrlist(true));
    QObject::connect(p_tablewidget_vs,SIGNAL(itemSelectionChanged()),this,SLOT(itemsel()));
    QObject::connect(p_tablewidget_vs,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(itemchanged_sk(QTableWidgetItem*)));

    p_tabwidget1=new QTabWidget(ui->centralWidget);
    p_tabwidget1->setTabPosition(QTabWidget::West);
    p_tabwidget1->addTab(p_tablewidget_g,mygeneral::tabstr());
    p_tabwidget1->addTab(p_tablewidget_trs,mysk::type2str(mysk::TriggerSkill));
    p_tabwidget1->addTab(p_tablewidget_vs,mysk::type2str(mysk::ViewAsSkill));

    p_label_skname=new QLabel(ui->centralWidget);
    QFormLayout *p_formlayout1=new QFormLayout;
    p_formlayout1->addRow(tr("Current Skill:"),p_label_skname);

    QPushButton *p_pushbutton_new=new QPushButton(ui->centralWidget);
    p_pushbutton_new->setText(tr("New"));
    QObject::connect(p_pushbutton_new,SIGNAL(clicked()),this,SLOT(mynew()));
    QPushButton *p_pushbutton_del=new QPushButton(ui->centralWidget);
    p_pushbutton_del->setText(tr("Delete"));
    QObject::connect(p_pushbutton_del,SIGNAL(clicked()),this,SLOT(mydel()));
    QPushButton *p_pushbutton_add=new QPushButton(ui->centralWidget);
    p_pushbutton_add->setText(tr("Skill_Add"));
    QObject::connect(p_pushbutton_add,SIGNAL(clicked()),this,SLOT(myadd()));
    QPushButton *p_pushbutton_undo=new QPushButton(ui->centralWidget);
    p_pushbutton_undo->setText(tr("Skill_Undo"));
    QObject::connect(p_pushbutton_undo,SIGNAL(clicked()),this,SLOT(myundo()));
    QPushButton *p_pushbutton_redo=new QPushButton(ui->centralWidget);
    p_pushbutton_redo->setText(tr("Skill_Redo"));
    QObject::connect(p_pushbutton_redo,SIGNAL(clicked()),this,SLOT(myredo()));
    QPushButton *p_pushbutton_import=new QPushButton(ui->centralWidget);
    p_pushbutton_import->setText(tr("Import"));
    QObject::connect(p_pushbutton_import,SIGNAL(clicked()),this,SLOT(myimport()));
    QPushButton *p_pushbutton_export=new QPushButton(ui->centralWidget);
    p_pushbutton_export->setText(tr("Export"));
    QObject::connect(p_pushbutton_export,SIGNAL(clicked()),this,SLOT(myexport()));
    QHBoxLayout *p_hboxlayout2=new QHBoxLayout;
    p_hboxlayout2->addWidget(p_pushbutton_new);
    p_hboxlayout2->addWidget(p_pushbutton_del);
    p_hboxlayout2->addWidget(p_pushbutton_add);
    p_hboxlayout2->addWidget(p_pushbutton_undo);
    p_hboxlayout2->addWidget(p_pushbutton_redo);
    p_hboxlayout2->addWidget(p_pushbutton_import);
    p_hboxlayout2->addWidget(p_pushbutton_export);

    p_textedit_all=new QTextEdit;
    p_textedit_all->setReadOnly(true);
    p_textedit_all->setLineWrapMode(QTextEdit::NoWrap);
    p_textedit_current=new QTextEdit;
    p_textedit_current->setReadOnly(true);
    p_textedit_current->setLineWrapMode(QTextEdit::NoWrap);
    //mytext();
    p_tabwidget2=new QTabWidget(ui->centralWidget);
    p_tabwidget2->setTabPosition(QTabWidget::West);
    p_tabwidget2->addTab(p_textedit_all,tr("Full"));
    p_tabwidget2->addTab(p_textedit_current,tr("Current Skill"));


    QVBoxLayout *p_vboxlayout=new QVBoxLayout;
    p_vboxlayout->addLayout(p_hboxlayout1);
    p_vboxlayout->addWidget(p_tabwidget1);
    p_vboxlayout->addLayout(p_formlayout1);
    p_vboxlayout->addLayout(p_hboxlayout2);

    QWidget *p_widget1=new QWidget(ui->centralWidget);
    p_widget1->setLayout(p_vboxlayout);
    QSplitter *p_splitter=new QSplitter(ui->centralWidget);
    p_splitter->setOrientation(Qt::Vertical);
    p_splitter->addWidget(p_widget1);
    p_splitter->addWidget(p_tabwidget2);
    QVBoxLayout *p_vboxlayout2=new QVBoxLayout;
    p_vboxlayout2->addWidget(p_splitter);
    //p_vboxlayout->addWidget(p_tabwidget2);
    ui->centralWidget->setLayout(p_vboxlayout2);

    p_inputwidget=NULL;

    myrfr();
    QObject::connect(psys,SIGNAL(update()),this,SLOT(myrfr()));

}

MainWindow::~MainWindow()
{
    delete ui;
    if(p_inputwidget){delete p_inputwidget;}
}
void MainWindow::myini_tips(){
    QFile fin("readme.txt");
    if(!fin.open(QFile::ReadOnly)){
        QMessageBox::warning(this,"FILE ERROR","No readme.txt");
        return;
    }
    QTextStream cin(&fin);
    QString str0,str;
    QStringList strlist;
    bool b=false;
    while(!cin.atEnd()){
        str=cin.readLine();
        if(str.startsWith("===")){b=true;}
        else if(b){
            if(str.startsWith(">")){
                if(strlist.isEmpty()){}
                else{
                    tips.insert(str0,mycode::mymdf(strlist,QString("\n"),false).join(""));
                    strlist.clear();
                    str0=str;
                }
            }
            else{strlist<<str;}
        }
    }
    fin.close();
}
void MainWindow::tipbox(){
    int rnd=qrand()%tips.count();
    qWarning()<<tips.count()<<rnd;
    QMap<QString,QString>::iterator iter;
    int cnt=0;
    for(iter=tips.begin(),cnt=0;iter!=tips.end();iter++,cnt++){
        if(cnt==rnd){
            QMessageBox::information(this,"TIPS",iter.key()+":\n"+iter.value());
            return;
        }
    }
}
void MainWindow::myimport(){    
    int ret=QMessageBox::question(this,"DESIGN FILE","Before you import a new design file, do you want to save the current design file?",
                                  QMessageBox::Yes,QMessageBox::No);
    if(ret==QMessageBox::Yes){myexport_design();}
    QString path=QFileDialog::getOpenFileName(this,"DESIGN FILE",".","design files (*.txt)");
    if(path!=""){
        delete psys;
        psys=new mysys(this);
        psys->myini(path);
        QObject::connect(psys,SIGNAL(update()),this,SLOT(myrfr()));
        psys->sig_update();
    }
}
void MainWindow::myexport(){
    if(psys->packagename==""){return;}
    QDir mydir("../extensions");
    if(!mydir.exists()){mydir.mkpath(".");}
    QString path=mydir.absolutePath()+"/"+psys->packagename+".lua";
    QFile fout("../extensions/"+psys->packagename+".lua");
    if(!fout.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,"SAVE","Fail to write "+path);
        qWarning()<<"save";return;
    }
    QTextStream cout(&fout);
    cout.setCodec("UTF-8");
    cout<<mycode::mymdf(psys->trans(),QString("\n"),false).join("");
    QMessageBox::warning(this,"SAVE","Save the LUA file successfully as "+path);
    fout.close();
    myexport_design();
}
void MainWindow::myexport_design(QString getpath){
    QFile fout2(getpath);
    if(!fout2.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,"SAVE","Fail to write "+getpath);
        qWarning()<<"save2";return;
    }
    QTextStream cout2(&fout2);
    cout2<<mycode::mymdf(psys->trans4design(),QString("\n"),false).join("");
    if(getpath!="tmp4design.txt"){QMessageBox::warning(this,"SAVE","Save the DESIGN file successfully as "+getpath);}
    fout2.close();
}
void MainWindow::myexport_design(){
    QString path2=QFileDialog::getSaveFileName(this,"DESIGN FILE",".","design files (*.txt)");
    myexport_design(path2);
}

QTableWidget *MainWindow::str2tw_tab(QString getstr){
    for(int i=0;i<p_tabwidget1->count();i++){
        if(p_tabwidget1->tabText(i)==getstr){return static_cast<QTableWidget *>(p_tabwidget1->widget(i));}
    }
    return NULL;
}
QString MainWindow::tw2str_tab(QTableWidget *ptw){
    for(int i=0;i<p_tabwidget1->count();i++){
        if(p_tabwidget1->widget(i)==ptw){return p_tabwidget1->tabText(i);}
    }
    return QString();
}
void MainWindow::itemsel_sk(QTableWidget *ptw){
    if(ptw->item(ptw->currentRow(),0)){
        QString getname=ptw->item(ptw->currentRow(),0)->text();
        qWarning()<<"itemsel_sk:"<<psys->psk0->name<<getname;
        if(psys->psk0->name!=getname){
            mysk *pt=psys->findSkillByName(getname);
            if(pt){psys->psk0=pt;}
            else{}
        }
    }
}
void MainWindow::itemsel(){
    if(!b4rfr){return;}
    if(p_tabwidget1->tabText(p_tabwidget1->currentIndex())==mygeneral::tabstr()){
        QTableWidget *ptw=static_cast<QTableWidget *>(p_tabwidget1->currentWidget());
        QString getname=ptw->item(ptw->currentRow(),0)->text();
        qWarning()<<"itemsel:"<<psys->pg0->name<<getname;
        if(psys->pg0->name!=getname){
            mygeneral *pg=psys->findGeneralByName(getname);
            if(pg){psys->pg0=pg;}
            else{}
        }
    }
    if(mysk::str2type(p_tabwidget1->tabText(p_tabwidget1->currentIndex()))!=-1){
        itemsel_sk(static_cast<QTableWidget *>(p_tabwidget1->currentWidget()));
    }
    myrfr();
    /*
    if(p_tablewidget_g->item(p_tablewidget_g->currentRow(),0)){
        QString getname=p_tablewidget_g->item(p_tablewidget_g->currentRow(),0)->text();
        qWarning()<<"itemsel:"<<psys->pg0->name<<getname;
        if(psys->pg0->name!=getname){
            mygeneral *pg=psys->findGeneralByName(getname);
            if(pg){psys->pg0=pg;}
            else{}
        }
    }
    itemsel_sk(p_tablewidget_trs);
    itemsel_sk(p_tablewidget_vs);
    */
}
void MainWindow::itemchanged_g(QTableWidgetItem *getp){
    if(!b4rfr){return;}
    if(!psys->pg0){return;}
    qWarning()<<"itemchanged_g:"<<getp->row()<<getp->column()<<getp->text();
    QString gettext=getp->text();
    b4rfr=false;getp->setText("");b4rfr=true;
    QMap<QString,QString> strmap;
    strmap.insert(getp->tableWidget()->horizontalHeaderItem(getp->column())->text(),gettext);
    psys->pg0->propertymap_set(strmap);    
    //QString hstr=p_tablewidget_g->horizontalHeaderItem(getp->column())->text();
    //if(mygeneral::str2property(hstr)==mygeneral::Name){psys->pg0->name=getp->text();}
    //if(mygeneral::str2property(hstr)==mygeneral::Translation){psys->pg0->translation=getp->text();}
    //if(mygeneral::str2property(hstr)==mygeneral::HP){psys->pg0->hp=getp->text().toInt();}
    //if(mygeneral::str2property(hstr)==mygeneral::Title){psys->pg0->title=getp->text();}
    //if(mygeneral::str2property(hstr)==mygeneral::Word){psys->pg0->word=getp->text();}
    //if(mygeneral::str2property(hstr)==mygeneral::CV){psys->pg0->cv=getp->text();}
}
void MainWindow::itemchanged_sk(QTableWidgetItem *getp){
    if(!b4rfr){return;}
    if(!psys->psk0){return;}
    qWarning()<<"itemchanged_sk:"<<getp->row()<<getp->column()<<getp->text();
    QString hstr=getp->tableWidget()->horizontalHeaderItem(getp->column())->text();
    QMap<QString,QString> strmap;
    int ii;
    for(ii=getp->column();ii>=0;ii--){
        QString hhstr=getp->tableWidget()->horizontalHeaderItem(ii)->text();
        if(mysk::str2property(hhstr,psys->psk0->getType())!=mysk::str2property(hstr,psys->psk0->getType())){break;}
        QTableWidgetItem *getpp=getp->tableWidget()->item(getp->row(),ii);
        if(getpp){
            QString gettext=getpp->text();
            b4rfr=false;getpp->setText("");b4rfr=true;
            if(gettext!=""){strmap.insertMulti(hhstr,gettext);}
        }
    }
    psys->psk0->propertymap_set(strmap);
    /*
    if(mytrs::str2property(hstr)==mytrs::Name){psys->psk0->name=getp->text();}
    if(mytrs::str2property(hstr)==mytrs::Translation){psys->psk0->translation=getp->text();}
    if(mytrs::str2property(hstr)==mytrs::Description){psys->psk0->description=getp->text();}
    if(mytrs::str2property(hstr)==mytrs::Words){
        int ii;
        for(ii=getp->column();ii>=0;ii--){
            if(mytrs::str2property(p_tablewidget_trs->horizontalHeaderItem(ii)->text())!=mytrs::Words){break;}
        }
        QString gettext=getp->text();
        b4rfr=false;getp->setText("");b4rfr=true;
        if(psys->psk0->wordslist.length()>getp->column()-ii-1){
            psys->psk0->wordslist[getp->column()-ii-1]=gettext;
        }
        else{
            psys->psk0->wordslist<<gettext;
        }
    }
    */
}
bool MainWindow::tablewidget_cw(QWidget *getcw, QTableWidget *&ptw, int &ii, int &jj){
    ptw=static_cast<QTableWidget *>(getcw->parent());
    if(!ptw){return false;}
    ptw=static_cast<QTableWidget *>(ptw->parent());
    if(!ptw){return false;}
    for(int i=0;i<ptw->rowCount();i++)
        for(int j=0;j<ptw->columnCount();j++){
            if(ptw->cellWidget(i,j)==getcw){
                ii=i;
                jj=j;
                return true;
            }
        }
    return false;
}
void MainWindow::itemchanged_cbb_sk(QComboBox *pcbb){
    QTableWidget *ptw;
    int ii,jj;
    if(tablewidget_cw(pcbb,ptw,ii,jj)){
        mysk *psk=psys->findSkillByName(ptw->item(ii,0)->text());
        if(!psk){}
        else{
            QMap<QString,QString> strmap;
            strmap.insert(ptw->horizontalHeaderItem(jj)->text(),pcbb->currentText());
            psk->propertymap_set(strmap);
        }
    }
}
void MainWindow::itemchanged_cbb_g(QComboBox *pcbb){
    QTableWidget *ptw;
    int ii,jj;
    if(tablewidget_cw(pcbb,ptw,ii,jj)){
        mygeneral *pg=psys->findGeneralByName(ptw->item(ii,0)->text());
        if(!pg){}
        else{
            QMap<QString,QString> strmap;
            strmap.insert(ptw->horizontalHeaderItem(jj)->text(),pcbb->currentText());
            pg->propertymap_set(strmap);
        }
    }
}
void MainWindow::itemchanged_cbb(){
    if(!b4rfr){return;}
    QComboBox *pcbb=static_cast<QComboBox *>(sender());
    itemchanged_cbb_g(pcbb);
    itemchanged_cbb_sk(pcbb);
}

void MainWindow::myrfr(){    
    b4rfr=false;
    p_lineedit_packagename->setText(psys->packagename);
    p_lineedit_packagetrans->setText(psys->package_trans);
    QMap<QString,QString> strmap;
    QMap<QString,QStringList> strlistmap;
    myrfr_tablewidget_removerow(str2tw_tab(mygeneral::tabstr()),psys->getgstrlist());
    foreach(mygeneral *ip,psys->glist){
        strmap.clear();
        strlistmap.clear();
        ip->propertymap_get(strmap,strlistmap);
        int getrow=myrfr_tablewidget_getrow(str2tw_tab(mygeneral::tabstr()),ip->name);
        myrfr_tablewidget_property(str2tw_tab(mygeneral::tabstr()),getrow,strmap,strlistmap);
        //myrfr_g(ip,myrfr_tablewidget_getrow(p_tablewidget_g,ip->name));
    }
    str2tw_tab(mygeneral::tabstr())->resizeColumnsToContents();

    QList<mysk *> trslist;
    psys->getsklist(trslist,mysk::TriggerSkill);
    myrfr_tablewidget_removerow(str2tw_tab(mysk::type2str(mysk::TriggerSkill)),psys->getskstrlist());
    foreach(mysk *ip,trslist){
        strmap.clear();
        strlistmap.clear();
        ip->propertymap_get(strmap,strlistmap);
        int getrow=myrfr_tablewidget_getrow(str2tw_tab(mysk::type2str(mysk::TriggerSkill)),ip->name);
        myrfr_tablewidget_property(str2tw_tab(mysk::type2str(mysk::TriggerSkill)),getrow,strmap,strlistmap);
        //myrfr_trs(pt,myrfr_tablewidget_getrow(p_tablewidget_trs,pt->name));
    }    
    str2tw_tab(mysk::type2str(mysk::TriggerSkill))->resizeColumnsToContents();

    QList<mysk *> vslist;
    psys->getsklist(vslist,mysk::ViewAsSkill);
    myrfr_tablewidget_removerow(str2tw_tab(mysk::type2str(mysk::ViewAsSkill)),psys->getskstrlist());
    foreach(mysk *ip,vslist){
        strmap.clear();
        strlistmap.clear();
        ip->propertymap_get(strmap,strlistmap);
        int getrow=myrfr_tablewidget_getrow(str2tw_tab(mysk::type2str(mysk::ViewAsSkill)),ip->name);
        myrfr_tablewidget_property(str2tw_tab(mysk::type2str(mysk::ViewAsSkill)),getrow,strmap,strlistmap);
    }
    str2tw_tab(mysk::type2str(mysk::ViewAsSkill))->resizeColumnsToContents();

    if(psys->psk0){
        p_label_skname->setText(psys->psk0->name+" | "+psys->psk0->translation);
    }
    if(psys->pg0){
        //p_label_gname->setText(psys->pg0->name+" | "+psys->pg0->translation);
    }
    b4rfr=true;
    mytext();
    qWarning()<<"130315:rfr";
}
void MainWindow::myrfr_tablewidget_removerow(QTableWidget *ptw, QStringList getstrlist){
    for(int i=0;i<ptw->rowCount();){
        if(!getstrlist.contains(ptw->item(i,0)->text())){
            ptw->removeRow(i);
            continue;
        }
        i++;
    }
}
int MainWindow::myrfr_tablewidget_getrow(QTableWidget *ptw, QString getstr){
    for(int i=0;i<ptw->rowCount();i++){
        if(ptw->item(i,0)&&(ptw->item(i,0)->text()==getstr)){return i;}
    }
    ptw->insertRow(ptw->rowCount());
    return ptw->rowCount()-1;
}
void MainWindow::myrfr_tablewidget_str(QTableWidget *ptw,int getrow, int getcol, QString getstr){
    while(ptw->columnCount()<=getcol){
        ptw->insertColumn(ptw->columnCount());
        if(mysk::str2type(tw2str_tab(ptw))!=-1){
            ptw->setHorizontalHeaderItem(ptw->columnCount()-1,new QTableWidgetItem(mysk::property2str(mysk::Words)));
        }
    }
    if(mysk::str2type(tw2str_tab(ptw))!=-1){
        if(ptw->columnCount()-1==getcol){
            ptw->insertColumn(ptw->columnCount());
            ptw->setHorizontalHeaderItem(ptw->columnCount()-1,new QTableWidgetItem(mysk::property2str(mysk::Words)));
        }
    }
    if(ptw->item(getrow,getcol)){
        if(ptw->item(getrow,getcol)->text()!=getstr){
            ptw->setItem(getrow,getcol,new QTableWidgetItem(getstr));
        }
    }
    else{
        ptw->setItem(getrow,getcol,new QTableWidgetItem(getstr));
    }
}
void MainWindow::myrfr_tablewidget_cbb(QTableWidget *ptw,int getrow, int getcol, QString getstr,QStringList list4new){
    while(ptw->columnCount()<=getcol){ptw->insertColumn(ptw->columnCount());}
    if(ptw->cellWidget(getrow,getcol)){
        QComboBox *pt=static_cast<QComboBox *>(ptw->cellWidget(getrow,getcol));
        while(pt->count()>0){pt->removeItem(0);}
        pt->addItems(list4new);
        if(pt->currentText()!=getstr){
            pt->setCurrentIndex(pt->findText(getstr));
        }
    }
    else{
        QComboBox *pt=new QComboBox(ptw);
        //pt->setEditable(true);
        pt->addItems(list4new);
        ptw->setCellWidget(getrow,getcol,pt);
        pt->setCurrentIndex(pt->findText(getstr));
        QObject::connect(pt,SIGNAL(currentIndexChanged(QString)),this,SLOT(itemchanged_cbb()));
    }
}
void MainWindow::myrfr_tablewidget_property(QTableWidget *ptw, int getrow, QMap<QString, QString> &strmap, QMap<QString, QStringList> &strlistmap){
    for(int i=0;i<ptw->columnCount();i++){
        if(i>=strmap.values().length()){return;}
        QString hstr=ptw->horizontalHeaderItem(i)->text();
        if(strmap.contains(hstr)){
            if(strlistmap.contains(hstr)){
                myrfr_tablewidget_cbb(ptw,getrow,i,strmap.value(hstr),strlistmap.value(hstr));
                continue;
            }
            if(strmap.count(hstr)==1){
                myrfr_tablewidget_str(ptw,getrow,i,strmap.value(hstr));
                continue;
            }
            int ii;
            for(ii=i;ii>=0;ii--){
                if(ptw->horizontalHeaderItem(ii)->text()!=hstr){break;}
            }            
            if(i-ii-1<strmap.count(hstr)){
                myrfr_tablewidget_str(ptw,getrow,i,strmap.values(hstr).at(strmap.count(hstr)-i+ii));
                continue;
            }
        }
        ptw->setItem(getrow,i,new QTableWidgetItem);
        qWarning()<<"130315-2"<<hstr<<getrow<<i;
    }
}

void MainWindow::mytext(){
    //int geti=0;
    //for(int i=0;i<psys->trans().length();i++){
    //    if(!textlist.contains(psys->trans().at(i))){geti=i;break;}
    //}
    textlist_all=psys->trans();
    p_textedit_all->setText(mycode::mymdf(textlist_all,QString("\n"),false).join(""));
    if(psys->psk0){textlist_current=psys->psk0->trans();}
    else{textlist_current=QStringList();}
    p_textedit_current->setText(mycode::mymdf(textlist_current,QString("\n"),false).join(""));
    //if(!p_textedit->scrollBarWidgets(Qt::AlignRight).isEmpty()){
    //    QScrollBar *pt=static_cast<QScrollBar *>(p_textedit->scrollBarWidgets(Qt::AlignRight).first());
    //    qWarning()<<"0224:"<<geti<<pt->minimum()<<pt->maximum();
    //    pt->setValue(pt->minimum()+(pt->maximum()-pt->minimum())*geti/textlist.length());
    //}
/*
    QTextCharFormat fmt;
    QFont font=p_textedit->font();
    font.setBold(true);
    fmt.setFont(font);
    QTextCursor cursor;
    cursor=p_textedit->textCursor();
    QStringList strlist;
    cursor.select(QTextCursor::LineUnderCursor);
    cursor.mergeCharFormat(fmt);
    p_textedit->mergeCurrentCharFormat(fmt);
*/

}
void MainWindow::myadd(){
    if(!psys->psk0){return;}
    if(p_inputwidget){delete p_inputwidget;}
    qWarning()<<"myadd";
    p_inputwidget=new myinputwidget(this);
    p_inputwidget->resize(600,600);
    p_inputwidget->setWindowTitle("myplugin_add");
    p_inputwidget->show();
}
void MainWindow::myundo(){
    if(!psys->psk0){return;}
    psys->psk0->undo();
}
void MainWindow::myredo(){
    if(!psys->psk0){return;}
    psys->psk0->redo();
}
void MainWindow::mynew(){
    if(p_tabwidget1->tabText(p_tabwidget1->currentIndex())==mygeneral::tabstr()){psys->newGeneral("");}
    psys->newSkill("",mysk::str2type(p_tabwidget1->tabText(p_tabwidget1->currentIndex())));
    //if(p_tabwidget1->tabText(p_tabwidget1->currentIndex())==mysk::type2str(mysk::TriggerSkill)){psys->newTrs("");}
    //if(p_tabwidget1->tabText(p_tabwidget1->currentIndex())==mysk::type2str(mysk::ViewAsSkill)){psys->newVs("");}
}
void MainWindow::mydel(){
    if(p_tabwidget1->tabText(p_tabwidget1->currentIndex())==mygeneral::tabstr()){
        if(psys->pg0){psys->delGeneral(psys->pg0);}
    }
    if(mysk::str2type(p_tabwidget1->tabText(p_tabwidget1->currentIndex()))!=-1){
        if(psys->psk0){psys->delSkill(psys->psk0);}
    }
}

void MainWindow::changePackageName(){
    psys->packagename=p_lineedit_packagename->text();
    psys->sig_update();
}
void MainWindow::changePackageTranslation(){
    psys->package_trans=p_lineedit_packagetrans->text();
    psys->sig_update();
}
/*
void MainWindow::myrfr_g(mygeneral *getp, int getrow){
    if(!getp){return;}
    for(int i=0;i<p_tablewidget_g->columnCount();i++){
        QString hstr=p_tablewidget_g->horizontalHeaderItem(i)->text();
        if(mygeneral::str2property(hstr)==mygeneral::Name){myrfr_tablewidget_str(p_tablewidget_g,getrow,i,getp->name);}
        else if(mygeneral::str2property(hstr)==mygeneral::Translation){myrfr_tablewidget_str(p_tablewidget_g,getrow,i,getp->translation);}
        else if(mygeneral::str2property(hstr)==mygeneral::Kingdom){myrfr_tablewidget_cbb(p_tablewidget_g,getrow,i,mygeneral::kingdom2str(getp->kingdom),mygeneral::kingdomstrlist());}
        else if(mygeneral::str2property(hstr)==mygeneral::Sex){myrfr_tablewidget_cbb(p_tablewidget_g,getrow,i,mygeneral::sex2str(getp->sex),mygeneral::sexstrlist());}
        else if(mygeneral::str2property(hstr)==mygeneral::HP){myrfr_tablewidget_str(p_tablewidget_g,getrow,i,QString::number(getp->hp));}
        else if(mygeneral::str2property(hstr)==mygeneral::Title){myrfr_tablewidget_str(p_tablewidget_g,getrow,i,getp->title);}
        else if(mygeneral::str2property(hstr)==mygeneral::Word){myrfr_tablewidget_str(p_tablewidget_g,getrow,i,getp->word);}
        else if(mygeneral::str2property(hstr)==mygeneral::CV){myrfr_tablewidget_str(p_tablewidget_g,getrow,i,getp->cv);}
        else{
            qWarning()<<"myrfr_g:"<<getrow<<i<<hstr;
            p_tablewidget_g->setItem(getrow,i,new QTableWidgetItem);
        }
    }
}
void MainWindow::myrfr_trs(mytrs *getp, int getrow){
    if(!getp){return;}
    QStringList gstrlist=psys->getgstrlist();
    gstrlist.prepend("NULL");
    for(int i=0;i<p_tablewidget_trs->columnCount();i++){
        QString hstr=p_tablewidget_trs->horizontalHeaderItem(i)->text();
        if(mytrs::str2property(hstr)==mytrs::Name){myrfr_tablewidget_str(p_tablewidget_trs,getrow,i,getp->name);}
        else if(mytrs::str2property(hstr)==mytrs::Translation){myrfr_tablewidget_str(p_tablewidget_trs,getrow,i,getp->translation);}
        else if(mytrs::str2property(hstr)==mytrs::Owner){myrfr_tablewidget_cbb(p_tablewidget_trs,getrow,i,getp->owner?getp->owner->name:"NULL",gstrlist);}
        else if(mytrs::str2property(hstr)==mytrs::Subtype){myrfr_tablewidget_cbb(p_tablewidget_trs,getrow,i,mytrs::type2str(getp->subtype),mytrs::typestrlist());}
        else if(mytrs::str2property(hstr)==mytrs::Description){myrfr_tablewidget_str(p_tablewidget_trs,getrow,i,getp->description);}
        else if((mytrs::str2property(hstr)==mytrs::Words)){
            int ii;
            for(ii=i;ii>=0;ii--){
                if(mytrs::str2property(p_tablewidget_trs->horizontalHeaderItem(ii)->text())!=mytrs::Words){break;}
            }
            if(i-ii-1<getp->wordslist.length()){
                myrfr_tablewidget_str(p_tablewidget_trs,getrow,i,getp->wordslist.at(i-ii-1));
            }
        }
        else{
            qWarning()<<"myrfr_trs:"<<getrow<<i<<hstr;
            p_tablewidget_trs->setItem(getrow,i,new QTableWidgetItem);
        }
    }
}
*/
