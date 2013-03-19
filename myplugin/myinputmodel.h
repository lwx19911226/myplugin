#ifndef MYINPUTMODEL_H
#define MYINPUTMODEL_H

#include <QAbstractItemModel>
//#include <QStandardItemModel>
//#include <QSortFilterProxyModel>
//#include <QFileSystemModel>
//#include <QMimeData>
#include <QDebug>
#include "myinputitem.h"
#include "mainwindow.h"
#include "myinputwidget.h"

class MainWindow;
class myinputmodel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit myinputmodel(QObject *parent = 0);
    ~myinputmodel();

    QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    void fetchMore(const QModelIndex &parent);
    bool canFetchMore(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section,Qt::Orientation orientation,int role = Qt::DisplayRole) const;
    //bool dropMimeData(const QMimeData *data,Qt::DropAction action,int row,int column,const QModelIndex &parent);
    //Qt::DropActions supportedDragActions() const;
    //QStringList mimeTypes() const;
    //QMimeData *mimeData(const QModelIndexList &indexes) const;

    myinputitem *getItem(const QModelIndex &index) const;
    myinputitem *getItem0(const QModelIndex &index) const;

    void myini();
    MainWindow *getmain(){return static_cast<MainWindow *>(QObject::parent());}
    mysys *getsys(){return getmain()->psys;}
    mysk *getsk0(){return getsys()->psk0;}
    //int getType(){return getmain()->p_inputwidget->type;}
    QModelIndex getIndex(myinputitem *getp,bool visibility=false){
        if(!getp->pf){return QModelIndex();}
        return createIndex(getp->getRank(visibility),0,getp);
    }

    myinputitem *rootitem0,*rootitem;
    //int sktype;
signals:

public slots:

};

#endif // MYINPUTMODEL_H
