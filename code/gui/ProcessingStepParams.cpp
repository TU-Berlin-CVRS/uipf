#include <sstream>

#include "ProcessingStepParams.hpp"

using namespace std;
using namespace uipf;

ProcessingStepParams::ProcessingStepParams(QObject *parent)
    :QAbstractTableModel(parent)
{
}

int ProcessingStepParams::rowCount(const QModelIndex & /*parent*/) const {
	return paramNames.size();

}

int ProcessingStepParams::columnCount(const QModelIndex & /*parent*/) const {
    return 1;
}



QVariant ProcessingStepParams::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
		map<string, string> para = this->step.params;
		return QString::fromStdString(para[this->paramNames[index.row()]]);
    }
    return QVariant();
}


QVariant ProcessingStepParams::headerData(int section, Qt::Orientation orientation, int role) const {

	if(role == Qt::DisplayRole) {
		stringstream ss;
		if(orientation == Qt::Horizontal) {
			  ss << "Value";
			  return QString(ss.str().c_str());
		}
		else if(orientation == Qt::Vertical) {
			ss << this->paramNames[section];
			return QString(ss.str().c_str());
		}
	}
	QSize size(100, 50);
	if (section == 0) {
		size.setWidth(200);
	}

	return QVariant(size);
}


void ProcessingStepParams::setProcessingStep(ProcessingStep proSt){

	beginResetModel();

	step = proSt;

	paramNames.clear();
	map<string, string> par = proSt.params;
	for (auto it = par.begin(); it!=par.end(); ++it) {
		paramNames.push_back(it->first);
	}

	endResetModel();
}

