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


    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setProcessingStep(uipf::ProcessingStep);


	QSize sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

private:
	ProcessingStep step;
	std::vector<std::string> paramNames;
};

}; // namespace

#endif // PROCESSINGSTEPPARAMS_H
