#ifndef MODELCOLUMNRO_H
#define MODELCOLUMNRO_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QBrush>


#include <iostream>
using namespace std;

class ModelColumnRO : public QAbstractTableModel
{
    Q_OBJECT
public:
    ModelColumnRO(QObject *parent);
    int             rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int             columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant        data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void            setStringList(QStringList *list);
    void            clear();
    QStringList*    getContent();
    void            setN(int n);
    int             getN();

public slots:
    void            updateData(const QModelIndex &, const QModelIndex &, const QVector<int> &);

private:
    QStringList     *content;
    int             n;
};

#endif // MODELCOLUMNRO_H
