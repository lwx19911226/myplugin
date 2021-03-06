#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myinputwidget.h"
#include "mydemowidget.h"
#include "myptgwidget.h"
#include "mysknwidget.h"

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
    QFileInfo fi("../QSanguosha.exe");
    int getqsv=mysys::VersionUnknown;
    if(fi.exists()){
        int tm=fi.lastModified().date().month();
        int td=fi.lastModified().date().day();
        QString qsvstr=QString("V%1%2").arg(QString::number(tm),2,'0').arg(QString::number(td),2,'0');
        getqsv=mysys::str2qsv(qsvstr);
    }

    psys=new mysys(this);
    QFile ft("tmp4design.txt");
    if(ft.exists()&&(QMessageBox::question(this,"DESIGN FILE",
                                           "There exists temp design files left before. Do you want to import it?",
                                          QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)){
        path4design="tmp4design.txt";
    }
    else{
        path4design=QFileDialog::getOpenFileName(NULL,"DESIGN FILE",".","design files (*.txt)");
    }
    psys->myini(path4design,getqsv);

    p_lineedit_packagename=new QLineEdit(ui->centralWidget);
    QObject::connect(p_lineedit_packagename,SIGNAL(textEdited(QString)),this,SLOT(changePackageName()));
    QFormLayout *p_formlayout4=new QFormLayout;    
    p_formlayout4->addRow(tr("Package Name")+tr(": "),p_lineedit_packagename);
    p_lineedit_packagetrans=new QLineEdit(ui->centralWidget);
    QObject::connect(p_lineedit_packagetrans,SIGNAL(textEdited(QString)),this,SLOT(changePackageTranslation()));
    QFormLayout *p_formlayout5=new QFormLayout;
    p_formlayout5->addRow(tr("Package Translation")+tr(": "),p_lineedit_packagetrans);
    p_combobox_qsv=new QComboBox(ui->centralWidget);
    p_combobox_qsv->addItems(mysys::qsvstrlist());
    p_combobox_qsv->setEnabled(path4design=="");
    QObject::connect(p_combobox_qsv,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeQSV()));
    QFormLayout *p_formlayout_qsv=new QFormLayout;
    p_formlayout_qsv->addRow(tr("QSanguosha Version")+tr(": "),p_combobox_qsv);
    QHBoxLayout *p_hboxlayout1=new QHBoxLayout;
    p_hboxlayout1->addLayout(p_formlayout4);
    p_hboxlayout1->addLayout(p_formlayout5);
    p_hboxlayout1->addLayout(p_formlayout_qsv);

    QTableWidget *p_tablewidget_g=new QTableWidget;
    p_tablewidget_g->setColumnCount(mygeneral::propertystrlist().length());
    p_tablewidget_g->setSelectionMode(QTableWidget::SingleSelection);    
    p_tablewidget_g->setHorizontalHeaderLabels(mygeneral::propertystrlist());
    QObject::connect(p_tablewidget_g,SIGNAL(itemSelectionChanged()),this,SLOT(itemsel()));
    QObject::connect(p_tablewidget_g,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(itemchanged_g(QTableWidgetItem*)));
    //QObject::connect(p_tablewidget_g,SIGNAL(cellChanged(int,int)),this,SLOT(cellchanged_g(int,int)));

    p_tabwidget1=new QTabWidget(ui->centralWidget);
    //p_tabwidget1->setTabPosition(QTabWidget::West);
    p_tabwidget1->addTab(p_tablewidget_g,mygeneral::tabstr());
    //p_tabwidget1->addTab(p_tablewidget_trs,mysk::type2str(mysk::TriggerSkill));
    //p_tabwidget1->addTab(p_tablewidget_vs,mysk::type2str(mysk::ViewAsSkill));

    QList<int> sktypelist;
    mysk::typelist(sktypelist);
    foreach(int gettype,sktypelist){
        QTableWidget *ptw=new QTableWidget;
        mysk *psk=psys->newsk(gettype);
        QStringList tstrlist=psk->propertystrlist(true);
        ptw->setColumnCount(tstrlist.length());
        ptw->setSelectionMode(QTableWidget::SingleSelection);
        ptw->setHorizontalHeaderLabels(tstrlist);
        QObject::connect(ptw,SIGNAL(itemSelectionChanged()),this,SLOT(itemsel()));
        QObject::connect(ptw,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(itemchanged_sk(QTableWidgetItem*)));
        p_tabwidget1->addTab(ptw,mysk::type2str(gettype));
        delete psk;
    }

    p_label_skname=new QLabel(ui->centralWidget);
    QFormLayout *p_formlayout1=new QFormLayout;
    p_formlayout1->addRow(tr("Current Skill")+tr(": "),p_label_skname);

    QPushButton *p_pushbutton_new=new QPushButton(tr("New"),ui->centralWidget);
    //p_pushbutton_new->setText(tr("New"));
    QObject::connect(p_pushbutton_new,SIGNAL(clicked()),this,SLOT(mynew()));
    QPushButton *p_pushbutton_del=new QPushButton(tr("Delete"),ui->centralWidget);
    //p_pushbutton_del->setText(tr("Delete"));
    QObject::connect(p_pushbutton_del,SIGNAL(clicked()),this,SLOT(mydel()));
    QPushButton *p_pushbutton_add=new QPushButton(tr("Skill_Add"),ui->centralWidget);
    //p_pushbutton_add->setText(tr("Skill_Add"));
    QObject::connect(p_pushbutton_add,SIGNAL(clicked()),this,SLOT(myadd()));
    p_pushbutton_undo=new QPushButton(tr("Skill_Undo"),ui->centralWidget);
    //p_pushbutton_undo->setText(tr("Skill_Undo"));
    p_pushbutton_undo->setEnabled(false);
    QObject::connect(p_pushbutton_undo,SIGNAL(clicked()),this,SLOT(myundo()));
    p_pushbutton_redo=new QPushButton(tr("Skill_Redo"),ui->centralWidget);
    //p_pushbutton_redo->setText(tr("Skill_Redo"));
    p_pushbutton_redo->setEnabled(false);
    QObject::connect(p_pushbutton_redo,SIGNAL(clicked()),this,SLOT(myredo()));    
    QPushButton *p_pushbutton_import=new QPushButton(tr("Import"),ui->centralWidget);
    //p_pushbutton_import->setText(tr("Import"));
    QObject::connect(p_pushbutton_import,SIGNAL(clicked()),this,SLOT(myimport()));
    QPushButton *p_pushbutton_export=new QPushButton(tr("Export"),ui->centralWidget);
    //p_pushbutton_export->setText(tr("Export"));
    QObject::connect(p_pushbutton_export,SIGNAL(clicked()),this,SLOT(myexport()));
    QHBoxLayout *p_hboxlayout2=new QHBoxLayout;
    p_hboxlayout2->addWidget(p_pushbutton_new);
    p_hboxlayout2->addWidget(p_pushbutton_del);
    p_hboxlayout2->addWidget(p_pushbutton_add);
    p_hboxlayout2->addWidget(p_pushbutton_undo);
    p_hboxlayout2->addWidget(p_pushbutton_redo);
    p_hboxlayout2->addWidget(p_pushbutton_import);
    p_hboxlayout2->addWidget(p_pushbutton_export);

    QPushButton *p_pushbutton_demo=new QPushButton(tr("Demo"),ui->centralWidget);
    QObject::connect(p_pushbutton_demo,SIGNAL(clicked()),this,SLOT(mydemo()));
    QPushButton *p_pushbutton_ptg=new QPushButton(tr("Pattern Generator"),ui->centralWidget);
    //p_pushbutton_ptg->setText(tr("Pattern Generator"));
    QObject::connect(p_pushbutton_ptg,SIGNAL(clicked()),this,SLOT(myptg()));
    QPushButton *p_pushbutton_skn=new QPushButton(tr("Skill Name List"),ui->centralWidget);
    //p_pushbutton_skn->setText(tr("Skill Name List"));
    QObject::connect(p_pushbutton_skn,SIGNAL(clicked()),this,SLOT(myskn()));
    QPushButton *p_pushbutton_readme=new QPushButton(tr("Readme"),ui->centralWidget);
    //p_pushbutton_readme->setText(tr("Readme"));
    QObject::connect(p_pushbutton_readme,SIGNAL(clicked()),this,SLOT(myreadme()));
    QPushButton *p_pushbutton_sgs=new QPushButton(ui->centralWidget);    
    if(fi.exists()){
        p_pushbutton_sgs->setText(tr("QSanguosha")+mysys::qsv2str(psys->qsv));
        QObject::connect(p_pushbutton_sgs,SIGNAL(clicked()),this,SLOT(mysgs()));
        if(p_combobox_qsv->isEnabled()){p_combobox_qsv->setEnabled(false);}
    }
    else{
        p_pushbutton_sgs->setText(tr("No QSanguosha"));
        p_pushbutton_sgs->setEnabled(false);
    }
    QHBoxLayout *p_hboxlayout3=new QHBoxLayout;
    p_hboxlayout3->addWidget(p_pushbutton_demo);
    p_hboxlayout3->addWidget(p_pushbutton_ptg);
    p_hboxlayout3->addWidget(p_pushbutton_skn);
    p_hboxlayout3->addWidget(p_pushbutton_readme);
    p_hboxlayout3->addWidget(p_pushbutton_sgs);

    p_textedit_all=new QTextEdit;
    p_textedit_all->setReadOnly(true);
    p_textedit_all->setLineWrapMode(QTextEdit::NoWrap);
    p_textedit_current=new QTextEdit;
    p_textedit_current->setReadOnly(true);
    p_textedit_current->setLineWrapMode(QTextEdit::NoWrap);
    p_treewidget=new QTreeWidget;
    p_treewidget->setColumnCount(3);
    QStringList tstrlist;
    tstrlist<<tr("Name")<<tr("Type")<<tr("Description");
    p_treewidget->setHeaderLabels(tstrlist);
    p_listwidget_do=new QListWidget;
    //mytext();
    p_tabwidget2=new QTabWidget(ui->centralWidget);
    //p_tabwidget2->setTabPosition(QTabWidget::West);    

    p_tabwidget2->addTab(p_listwidget_do,tr("Procedure"));
    p_tabwidget2->addTab(p_treewidget,tr("Block Tree"));
    p_tabwidget2->addTab(p_textedit_current,tr("LUA(Current Skill)"));
    p_tabwidget2->addTab(p_textedit_all,tr("LUA(Full)"));

    QVBoxLayout *p_vboxlayout=new QVBoxLayout;
    p_vboxlayout->addLayout(p_hboxlayout1);
    p_vboxlayout->addWidget(p_tabwidget1);
    p_vboxlayout->addLayout(p_formlayout1);
    p_vboxlayout->addLayout(p_hboxlayout2);
    p_vboxlayout->addLayout(p_hboxlayout3);

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
    p_demowidget=NULL;
    //qWarning()<<"130325~~";
    myrfr();
    //qWarning()<<"130325~";
    QObject::connect(psys,SIGNAL(update()),this,SLOT(myrfr()));
    QObject::connect(p_tabwidget1,SIGNAL(currentChanged(int)),this,SLOT(myrfr()));
    QObject::connect(p_tabwidget2,SIGNAL(currentChanged(int)),this,SLOT(myrfr()));
    resize(QApplication::desktop()->screenGeometry().width()*0.75,QApplication::desktop()->screenGeometry().height()*0.75);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(p_inputwidget){delete p_inputwidget;}
    if(p_demowidget){delete p_demowidget;}
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
    int ret=QMessageBox::question(this,"DESIGN FILE","Before importing a new design file, do you want to save the current design file?",
                                  QMessageBox::Yes,QMessageBox::No);
    if(ret==QMessageBox::Yes){myexport_design();}
    QString path=QFileDialog::getOpenFileName(this,"DESIGN FILE",".","design files (*.txt)");
    if(path!=""){
        path4design=path;
        int getqsv=psys->qsv;
        delete psys;
        psys=new mysys(this);
        psys->myini(path,getqsv);
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
    QFile fsys("myplugin_sysfunc.lua");
    if(fsys.exists()){
        QString patht="../lua/"+fsys.fileName();
        QFile fsyst(patht);
        if(fsyst.exists()){
            if(!fsyst.remove()){
                qWarning()<<"save copy remove";return;
            }
        }
        if(!fsys.copy(patht)){
            QMessageBox::warning(this,"SAVE","Fail to copy "+fsys.fileName()+" to "+patht+". Please manually copy it.");
            qWarning()<<"save copy";return;
        }
    }
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
    if(path2!=""){myexport_design(path2);}
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
    QTableWidgetItem *pitem=ptw->item(ptw->currentRow(),0);
    if(pitem){
        QString getname=pitem->text();
        if(psys->psk0&&(psys->psk0->name==getname)){}
        else{
            mysk *pt=psys->findSkillByName(getname);
            if(pt){psys->psk0=pt;}
            else{}
        }
    }
}
void MainWindow::itemsel(){
    if(!b4rfr){return;}
    QString twtab=p_tabwidget1->tabText(p_tabwidget1->currentIndex());
    QTableWidget *ptw=str2tw_tab(twtab);
    if(twtab==mygeneral::tabstr()){
        QString getname=ptw->item(ptw->currentRow(),0)->text();
        if(psys->pg0&&(psys->pg0->name==getname)){}
        else{
            mygeneral *pg=psys->findGeneralByName(getname);
            if(pg){psys->pg0=pg;}
            else{}
        }
    }
    if(mysk::typestrlist().contains(twtab)){
        itemsel_sk(ptw);
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
    if(getp->column()>0){b4rfr=false;getp->setText("");b4rfr=true;}
    QMap<QString,QString> strmap;
    strmap.insert(getp->tableWidget()->horizontalHeaderItem(getp->column())->text(),gettext);
    psys->pg0->propertymap_set(strmap,true);
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
    QStringList strlist;
    int ii;
    for(ii=getp->column();ii>=0;ii--){
        QString hhstr=getp->tableWidget()->horizontalHeaderItem(ii)->text();
        if(psys->psk0->str2property(hhstr)!=psys->psk0->str2property(hstr)){break;}
        QTableWidgetItem *getpp=getp->tableWidget()->item(getp->row(),ii);
        if(getpp){
            QString gettext=getpp->text();
            if(getpp->column()>0){b4rfr=false;getpp->setText("");b4rfr=true;}
            if(gettext!=""){strlist.prepend(gettext);}
        }
    }
    strmap.insert(hstr,strlist.join(",,"));
    psys->psk0->propertymap_set(strmap,true);
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
bool MainWindow::tw_cw(QWidget *getcw, QTableWidget *&ptw, int &ii, int &jj){
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
    if(tw_cw(pcbb,ptw,ii,jj)){
        mysk *psk=psys->findSkillByName(ptw->item(ii,0)->text());
        if(!psk){}
        else{
            QMap<QString,QString> strmap;
            strmap.insert(ptw->horizontalHeaderItem(jj)->text(),pcbb->currentText());
            psk->propertymap_set(strmap,true);
        }
    }
}
void MainWindow::itemchanged_cbb_g(QComboBox *pcbb){
    QTableWidget *ptw;
    int ii,jj;
    if(tw_cw(pcbb,ptw,ii,jj)){
        mygeneral *pg=psys->findGeneralByName(ptw->item(ii,0)->text());
        if(!pg){}
        else{
            QMap<QString,QString> strmap;
            strmap.insert(ptw->horizontalHeaderItem(jj)->text(),pcbb->currentText());
            pg->propertymap_set(strmap,true);
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
    p_combobox_qsv->setCurrentIndex(p_combobox_qsv->findText(mysys::qsv2str(psys->qsv)));
    QMap<QString,QString> strmap;    
    QString tstr=p_tabwidget1->tabText(p_tabwidget1->currentIndex());
    QTableWidget *pcurrent=str2tw_tab(tstr);
    if(tstr==mygeneral::tabstr()){
        myrfr_tw_removerow(pcurrent,psys->getgstrlist());
        foreach(mygeneral *ip,psys->glist){
            strmap.clear();
            ip->propertymap_get(strmap,true);
            int getrow=myrfr_tw_getrow(pcurrent,ip->name);
            myrfr_tw_property(pcurrent,getrow,strmap);
            //myrfr_g(ip,myrfr_tablewidget_getrow(p_tablewidget_g,ip->name));
        }
        pcurrent->resizeColumnsToContents();
    }
    else{
        QList<int> sktypelist;
        mysk::typelist(sktypelist);
        foreach(int gettype,sktypelist){
            if(tstr==mysk::type2str(gettype)){
                QList<mysk *> sklist;
                psys->getsklist(sklist,gettype);
                myrfr_tw_removerow(pcurrent,psys->getskstrlist(gettype));
                foreach(mysk *ip,sklist){
                    strmap.clear();
                    ip->propertymap_get(strmap,true);
                    int getrow=myrfr_tw_getrow(pcurrent,ip->name);
                    myrfr_tw_property(pcurrent,getrow,strmap);
                }
                pcurrent->resizeColumnsToContents();
                break;
            }
        }
    }
    QString sk0str;
    if(psys->psk0){
        sk0str=psys->psk0->name+" | "+psys->psk0->translation;
    }
    p_label_skname->setText(sk0str);
    if(psys->pg0){
        //p_label_gname->setText(psys->pg0->name+" | "+psys->pg0->translation);
    }
    p_pushbutton_undo->setEnabled(!psys->dolist.isEmpty());
    p_pushbutton_redo->setEnabled(!psys->undostrlist.isEmpty());
    b4rfr=true;
    if(p_tabwidget2->currentWidget()==p_treewidget){mytree();}
    else if(p_tabwidget2->currentWidget()==p_listwidget_do){mylist_do();}
    else if(p_tabwidget2->currentWidget()==p_textedit_all){mytext_all();}
    else if(p_tabwidget2->currentWidget()==p_textedit_current){mytext_current();}

    //qWarning()<<"myrfr";
}
void MainWindow::myrfr_tw_removerow(QTableWidget *ptw, QStringList getstrlist){
    for(int i=0;i<ptw->rowCount();){
        if(!getstrlist.contains(ptw->item(i,0)->text())){
            ptw->removeRow(i);
            continue;
        }
        i++;
    }
}
int MainWindow::myrfr_tw_getrow(QTableWidget *ptw, QString getstr){
    for(int i=0;i<ptw->rowCount();i++){
        if(ptw->item(i,0)&&(ptw->item(i,0)->text()==getstr)){return i;}
    }
    ptw->insertRow(ptw->rowCount());
    return ptw->rowCount()-1;
}
void MainWindow::myrfr_tw_str(QTableWidget *ptw,int getrow, int getcol, QString getstr){
    bool b4sk=mysk::typestrlist().contains(tw2str_tab(ptw));
    if(b4sk){
        mysk *tsk=psys->newsk(mysk::str2type(tw2str_tab(ptw)));
        while(ptw->columnCount()<=getcol){
            ptw->insertColumn(ptw->columnCount());
            ptw->setHorizontalHeaderItem(ptw->columnCount()-1,new QTableWidgetItem(tsk->property2str(mysk::Words)));
        }
        if(ptw->columnCount()-1==getcol){
            ptw->insertColumn(ptw->columnCount());
            ptw->setHorizontalHeaderItem(ptw->columnCount()-1,new QTableWidgetItem(tsk->property2str(mysk::Words)));
        }
        delete tsk;
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
void MainWindow::myrfr_tw_cbb(QTableWidget *ptw,int getrow, int getcol, QString getstr,QStringList list4new){
    while(ptw->columnCount()<=getcol){ptw->insertColumn(ptw->columnCount());}
    bool b4edit=false;
    if((list4new.length()>1)&&(list4new.at(1)=="")){b4edit=true;list4new.removeOne("");}
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
        pt->setEditable(b4edit);
        pt->addItems(list4new);
        ptw->setCellWidget(getrow,getcol,pt);
        pt->setCurrentIndex(pt->findText(getstr));
        QObject::connect(pt,SIGNAL(currentIndexChanged(QString)),this,SLOT(itemchanged_cbb()));
        QObject::connect(pt,SIGNAL(editTextChanged(QString)),this,SLOT(itemchanged_cbb()));
    }
}
void MainWindow::myrfr_tw_property(QTableWidget *ptw, int getrow, QMap<QString, QString> &strmap){
    for(int i=0;i<ptw->columnCount();i++){
        if(i>=strmap.values().length()){return;}
        QString hstr=ptw->horizontalHeaderItem(i)->text();
        if(strmap.contains(hstr)){
            if(strmap.count(hstr)==1){
                QString getstr=strmap.value(hstr);
                QStringList tstrlist=mycode::mysplit(getstr,"|");
                if(tstrlist.length()>1){myrfr_tw_cbb(ptw,getrow,i,tstrlist.first(),tstrlist);}
                else{myrfr_tw_str(ptw,getrow,i,tstrlist.first());}
                continue;
            }
            int ii;
            for(ii=i;ii>=0;ii--){
                if(ptw->horizontalHeaderItem(ii)->text()!=hstr){break;}
            }            
            if(i-ii-1<strmap.count(hstr)){
                myrfr_tw_str(ptw,getrow,i,strmap.values(hstr).at(strmap.count(hstr)-i+ii));
                continue;
            }
        }
        ptw->setItem(getrow,i,new QTableWidgetItem);
        qWarning()<<"myrfr_tw_property"<<hstr<<getrow<<i<<tw2str_tab(ptw);
    }
}

void MainWindow::mytext_all(){
    //int geti=0;
    //for(int i=0;i<psys->trans().length();i++){
    //    if(!textlist.contains(psys->trans().at(i))){geti=i;break;}
    //}
    textlist_all=psys->trans();
    p_textedit_all->setText(mycode::mymdf(textlist_all,QString("\n"),false).join(""));

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
void MainWindow::mytext_current(){
    if(psys->psk0){
        QStringList tback;
        textlist_current=psys->psk0->trans(tback);
        textlist_current<<tback;
    }
    else{textlist_current=QStringList();}
    p_textedit_current->setText(mycode::mymdf(textlist_current,QString("\n"),false).join(""));
}

void MainWindow::mytree(){
    p_treewidget->clear();
    if(!psys->psk0){return;}
    foreach(myblock *ip,psys->psk0->blocklist){
        QTreeWidgetItem *pitem=ip->mytreeitem();
        p_treewidget->addTopLevelItem(pitem);
    }
    p_treewidget->expandAll();
    for(int i=0;i<p_treewidget->columnCount();i++){
        p_treewidget->resizeColumnToContents(i);
    }
}

void MainWindow::mylist_do(){
    p_listwidget_do->clear();
    if(!psys->psk0){return;}
    foreach(mydo *ip,psys->dolist){
        if(ip->psktgt==psys->psk0){
            p_listwidget_do->addItem(ip->getfunc()->getRemark());
        }
    }
}

void MainWindow::myadd(){
    if(!psys->psk0){return;}
    if(psys->psk0->getType()==mysk::ExistingSkill){return;}
    if(p_inputwidget){delete p_inputwidget;}
    qWarning()<<"myadd";
    p_inputwidget=new myinputwidget(this);
    p_inputwidget->resize(QApplication::desktop()->screenGeometry().width()*0.75,QApplication::desktop()->screenGeometry().height()*0.75);
    p_inputwidget->setWindowTitle(tr("Skill_Add"));
    p_inputwidget->show();
}
void MainWindow::myundo(){
    //if(!psys->psk0){return;}
    psys->undo();
}
void MainWindow::myredo(){
    //if(!psys->psk0){return;}
    psys->redo();
}
void MainWindow::mynew(){
    if(p_tabwidget1->tabText(p_tabwidget1->currentIndex())==mygeneral::tabstr()){psys->newGeneral("");}
    else{psys->newSkill("",mysk::str2type(p_tabwidget1->tabText(p_tabwidget1->currentIndex())));}
    //if(p_tabwidget1->tabText(p_tabwidget1->currentIndex())==mysk::type2str(mysk::TriggerSkill)){psys->newTrs("");}
    //if(p_tabwidget1->tabText(p_tabwidget1->currentIndex())==mysk::type2str(mysk::ViewAsSkill)){psys->newVs("");}
}
void MainWindow::mydel(){
    if(p_tabwidget1->tabText(p_tabwidget1->currentIndex())==mygeneral::tabstr()){
        if(psys->pg0){psys->delGeneral(psys->pg0);}
    }
    if(mysk::typestrlist().contains(p_tabwidget1->tabText(p_tabwidget1->currentIndex()))){
        if(psys->psk0){psys->delSkill(psys->psk0);}
    }
}
void MainWindow::mydemo(){
    if(!p_demowidget){p_demowidget=new mydemowidget;}
    p_demowidget->show();
    p_demowidget->activateWindow();
}
void MainWindow::myptg(){
    myptgwidget *p_ptgwidget=new myptgwidget(this);
    p_ptgwidget->resize(QApplication::desktop()->screenGeometry().width()*0.5,QApplication::desktop()->screenGeometry().height()*0.5);
    p_ptgwidget->show();
}
void MainWindow::myskn(){
    mysknwidget *p_sknwidget=new mysknwidget(this);
    p_sknwidget->resize(QApplication::desktop()->screenGeometry().width()*0.5,QApplication::desktop()->screenGeometry().height()*0.5);
    p_sknwidget->show();
}
void MainWindow::myreadme(){
    QDesktopServices ds;
    ds.openUrl(QUrl("readme.txt"));
    //QProcess::startDetached("readme.txt");
}
void MainWindow::mysgs(){
    //QDesktopServices ds;
    //ds.openUrl(QUrl("../QSanguosha.exe"));
    QString tstr=QDir::currentPath();
    QDir::setCurrent("..");
    QProcess::startDetached("QSanguosha.exe");
    QDir::setCurrent(tstr);
}

void MainWindow::changePackageName(){
    psys->packagename=p_lineedit_packagename->text();
    psys->sig_update();
}
void MainWindow::changePackageTranslation(){
    psys->package_trans=p_lineedit_packagetrans->text();
    psys->sig_update();
}
void MainWindow::changeQSV(){
    if(!b4rfr){return;}
    psys->qsv=mysys::str2qsv(p_combobox_qsv->currentText());
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
    gstrlist.prepend(mygeneral::nullname());
    for(int i=0;i<p_tablewidget_trs->columnCount();i++){
        QString hstr=p_tablewidget_trs->horizontalHeaderItem(i)->text();
        if(mytrs::str2property(hstr)==mytrs::Name){myrfr_tablewidget_str(p_tablewidget_trs,getrow,i,getp->name);}
        else if(mytrs::str2property(hstr)==mytrs::Translation){myrfr_tablewidget_str(p_tablewidget_trs,getrow,i,getp->translation);}
        else if(mytrs::str2property(hstr)==mytrs::Owner){myrfr_tablewidget_cbb(p_tablewidget_trs,getrow,i,getp->owner?getp->owner->name:mygeneral::nullname(),gstrlist);}
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
