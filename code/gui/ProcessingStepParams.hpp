#ifndef PROCESSINGSTEPPARAMS_H
#define PROCESSINGSTEPPARAMS_H

#include <QAbstractTableModel>
#include <QStyleOptionViewItem>

#include "../framework/ProcessingStep.hpp"

namespace uipf {

class ProcessingStepParams : public QAbstractTableModel
{
    Q_OBJECT

public:
    ProcessingStepParams(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

	bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setProcessingStep(uipf::ProcessingStep);

    Qt::ItemFlags flags (const QModelIndex &index) const;

private:
	ProcessingStep step;
	std::vector<std::string> paramNames;
};

}; // namespace

#endif // PROCESSINGSTEPPARAMS_H
