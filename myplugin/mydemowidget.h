#ifndef MYDEMOWIDGET_H
#define MYDEMOWIDGET_H

#include <QWidget>
#include "mainwindow.h"

class mydemowidget : public QWidget
{
    Q_OBJECT
public:
    explicit mydemowidget(QWidget *parent = 0);
    mysys *psys;
    QComboBox *p_combobox_flist,*p_combobox_sklist;
    QLabel *p_label_qsv;
    QList<mydo *> dolist4show;
    QStringList flist;
    QListWidget *p_listwidget;
    QTextEdit *p_textedit_do,*p_textedit_sk;
    bool b4rfr;
    void myload(QString getpath);
signals:
    
public slots:
    void myload_btn();
    void myload_cbb();
    void myrfr();
    void myrfr_sk();
    void myrfr_do();
    void changeSkill();
};

#endif // MYDEMOWIDGET_H
