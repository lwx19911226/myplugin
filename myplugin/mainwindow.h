#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QClipboard>
#include <QDesktopWidget>
#include <QDir>
#include <QTime>
#include <QModelIndex>
#include <QColumnView>
#include <QTreeView>
#include <QTreeWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemModel>
#include <QTabWidget>
#include <QGridLayout>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QTextEdit>
#include <QTextCharFormat>
#include <QFont>
#include <QScrollBar>
#include <QLineEdit>
#include <QFileSystemModel>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>
#include "mysys.h"
#include "myptgwidget.h"
//#include "myinputwidget.h"

namespace Ui {
class MainWindow;
}

class myinputwidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QMap<QString,QString> tips;
    mysys *psys;
    myinputwidget *p_inputwidget;
    QTextEdit *p_textedit_all,*p_textedit_current;
    QStringList textlist_all,textlist_current;
    QLineEdit *p_lineedit_packagename,*p_lineedit_packagetrans;
    QTabWidget *p_tabwidget1,*p_tabwidget2;
    QTreeWidget *p_treewidget;
    //QTableWidget *p_tablewidget_g,*p_tablewidget_trs,*p_tablewidget_vs;
    QLabel *p_label_skname;
    QPushButton *p_pushbutton_undo,*p_pushbutton_redo;


    bool b4rfr;

    void myini_tips();
    void tipbox();

    void myrfr_g(mygeneral *getp,int getrow);
    void myrfr_trs(mytrs *getp,int getrow);
    void myrfr_tw_str(QTableWidget *ptw,int getrow,int getcol,QString getstr);
    void myrfr_tw_cbb(QTableWidget *ptw,int getrow,int getcol,QString getstr,QStringList list4new);
    void myrfr_tw_removerow(QTableWidget *ptw,QStringList getstrlist);
    int myrfr_tw_getrow(QTableWidget *ptw,QString getstr);
    void myrfr_tw_property(QTableWidget *ptw,int getrow,QMap<QString,QString> &strmap);
    void mytext_all();
    void mytext_current();
    void mytree();
    void myexport_design(QString);
    void itemsel_sk(QTableWidget *ptw);
    void itemchanged_cbb_sk(QComboBox *pcbb);
    void itemchanged_cbb_g(QComboBox *pcbb);
    bool tw_cw(QWidget *getcw,QTableWidget *&ptw,int &ii,int &jj);
    QTableWidget *str2tw_tab(QString);
    QString tw2str_tab(QTableWidget *);
private slots:
    void myexport();
    void myexport_design();
    void myimport();
    void myadd();
    void myundo();
    void myredo();
    void myptg();
    void myskn();
    void myreadme();
    void mysgs();
    void mynew();
    void mydel();
    void myrfr();
    void changePackageName();
    void changePackageTranslation();
    void itemchanged_g(QTableWidgetItem*);
    void itemchanged_sk(QTableWidgetItem *);
    void itemchanged_cbb();
    void itemsel();    
private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
