#include "ModelColumnRO.h"
#include <QtSql/QSqlQueryModel>

ModelColumnRO::ModelColumnRO(QObject *parent)
    :QAbstractTableModel(parent)
{
    this->content = NULL;
    this->n = 0;
    connect(this,
        SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
        this, SLOT(updateData(const QModelIndex &, const QModelIndex &, const QVector<int> &)));

}

int ModelColumnRO::rowCount(const QModelIndex & /*parent*/) const
{
    if (!this->content)
        return (0);
   return (this->content->size());
}

int ModelColumnRO::columnCount(const QModelIndex & /*parent*/) const
{
    return (1);
}


QVariant    ModelColumnRO::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return (QString(this->content->at(index.row())));
    }
    return QVariant();
}

void    ModelColumnRO::setStringList(QStringList  *list) {
    this->content = list;
    emit this->dataChanged(QModelIndex(), QModelIndex(), QVector<int>());
}

void    ModelColumnRO::clear()
{
    if (this->content)
        this->content->clear();
}

void    ModelColumnRO::updateData(const QModelIndex &, const QModelIndex &, const QVector<int> &) {
    cout << "Data changed" << endl;
}

QStringList*    ModelColumnRO::getContent()                 {   return (this->content);     }
void            ModelColumnRO::setN(int n)                  {   this->n = n;                }
int             ModelColumnRO::getN()                       {   return (this->n);           }

