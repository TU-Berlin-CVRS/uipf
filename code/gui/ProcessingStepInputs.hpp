#ifndef PROCESSINGSTEPINPUTS_H
#define PROCESSINGSTEPINPUTS_H

#include <QAbstractTableModel>
#include <QStyleOptionViewItem>

#include "../framework/ProcessingStep.hpp"

namespace uipf {

class ProcessingStepInputs : public QAbstractTableModel
{
    Q_OBJECT

public:
    ProcessingStepInputs(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;


    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setProcessingStep(uipf::ProcessingStep);

private:
	ProcessingStep step;
	std::vector<std::string> inputNames;
};

} // namespace

#endif // PROCESSINGSTEPINPUTS_H
