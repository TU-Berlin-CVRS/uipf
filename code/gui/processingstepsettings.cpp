#include "processingstepsettings.h"

ProcessingStepSettings::ProcessingStepSettings(QObject *parent)
    :QAbstractTableModel(parent)
{
}

int ProcessingStepSettings::rowCount(const QModelIndex & /*parent*/) const
{
   return 4;
}

int ProcessingStepSettings::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant ProcessingStepSettings::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
       return QString("Row%1, Column%2")
                   .arg(index.row() + 1)
                   .arg(index.column() +1);
    }
    return QVariant();
}
