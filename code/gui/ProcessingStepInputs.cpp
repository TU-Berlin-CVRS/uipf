#include <sstream>

#include "ProcessingStepInputs.hpp"

using namespace std;
using namespace uipf;

ProcessingStepInputs::ProcessingStepInputs(QObject *parent)
    :QAbstractTableModel(parent)
{
}

int ProcessingStepInputs::rowCount(const QModelIndex & /*parent*/) const {
	return inputNames.size();

}

int ProcessingStepInputs::columnCount(const QModelIndex & /*parent*/) const {
    return 2;
}



QVariant ProcessingStepInputs::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
		map<string, pair<string, string> > inp = this->step.inputs;
		if(index.column() == 0){
			return QString::fromStdString(inp[this->inputNames[index.row()]].first);
		} else if(index.column() == 1){
			return QString::fromStdString(inp[this->inputNames[index.row()]].second);
		}
    }
    return QVariant();
}


QVariant ProcessingStepInputs::headerData(int section, Qt::Orientation orientation, int role) const {

	if(role == Qt::DisplayRole) {
		stringstream ss;
		if(orientation == Qt::Horizontal) {
			if(section == 0){
			  ss << "From Step:";
			  return QString(ss.str().c_str());
			}
			if(section == 1){
			  ss << "Output name:";
			  return QString(ss.str().c_str());
			}
		}
		else if(orientation == Qt::Vertical) {
			ss << this->inputNames[section];
			return QString(ss.str().c_str());
		}
	}
	QSize size(100, 50);
	if (section == 0) {
		size.setWidth(200);
	}

	return QVariant(size);
}


void ProcessingStepInputs::setProcessingStep(ProcessingStep proSt){

	beginResetModel();

	step = proSt;

	inputNames.clear();
	map<string, pair<string, string> > inp = proSt.inputs;
	for (auto it = inp.begin(); it!=inp.end(); ++it) {
		inputNames.push_back(it->first);
	}

	endResetModel();
}

