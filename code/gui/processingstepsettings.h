#ifndef PROCESSINGSTEPSETTINGS_H
#define PROCESSINGSTEPSETTINGS_H

#include <QAbstractTableModel>

class ProcessingStepSettings : public QAbstractTableModel
{
    Q_OBJECT
public:
    ProcessingStepSettings(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

#endif // PROCESSINGSTEPSETTINGS_H
