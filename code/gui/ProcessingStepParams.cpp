#include <sstream>
#include <iostream>

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


bool ProcessingStepParams::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        if (value.toString().isEmpty()){
			return false;
		}
		// only the values can be changed
		if(index.column() == 0){
			this->step.params[this->paramNames[index.row()]] = value.toString().toStdString();
		}
		
		QModelIndex transposedIndex = createIndex(index.column(), index.row());
		emit dataChanged(index, index);
		emit dataChanged(transposedIndex, transposedIndex);
		return true;
    }
    return false;
}

QVariant ProcessingStepParams::data(const QModelIndex &index, int role) const {
	map<string, string> para = this->step.params;
    if (role == Qt::DisplayRole) {
		return QString::fromStdString(para[this->paramNames[index.row()]]);
	} else if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    } else if(role == Qt::EditRole)	{
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

Qt::ItemFlags ProcessingStepParams::flags (const QModelIndex &index) const {

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
