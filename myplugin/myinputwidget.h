#ifndef MYINPUTWIDGET_H
#define MYINPUTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QColumnView>
#include <QTreeView>
#include <QHeaderView>
#include <QLabel>
#include <QTextEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
//#include "myinputmodel.h"


class myinputitem;
class myinputmodel;
class MainWindow;
class myinputwidget : public QWidget
{
    Q_OBJECT
public:
    explicit myinputwidget(MainWindow *getpmain,QWidget *parent=0);
    ~myinputwidget();
    //enum mytype{Sentence=0,Condition=1,Foreach=2};
    
    MainWindow *pmain;
    QColumnView *p_columnview;
    myinputmodel *p_inputmodel;
    QTextEdit *p_textedit;
    //QComboBox *p_combobox_type;
    QComboBox *p_combobox_filter;
    QList<QLabel *> p_label_list;
    QList<QLineEdit *> p_lineedit_list;
    QLineEdit *p_lineedit_filter;

    int seltype;    
    bool b4filter;

    QStringList getSel();
    QString getRemark(myinputitem *pi);
signals:
    
public slots:
    void myadd();
    //void myptg();
    //void changeType(QString);
    void changeColumnView();
    void changeFilter();
    void showRemark();
    void changeRTR();
    //void changeColumnViewWidths();
    void filterItems();
    void filterItems_obj();
    void myscroll(const QModelIndex &);
    void mytest(const QModelIndex &);
};

#endif // MYINPUTWIDGET_H
