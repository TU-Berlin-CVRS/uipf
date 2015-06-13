#include <sstream>

#include "processingstepsettings.h"

ProcessingStepSettings::ProcessingStepSettings(QObject *parent)
    :QAbstractTableModel(parent)
{
}

int ProcessingStepSettings::rowCount(const QModelIndex & /*parent*/) const {
	return paramNames.size();

}

int ProcessingStepSettings::columnCount(const QModelIndex & /*parent*/) const {
    return 1;
}



QVariant ProcessingStepSettings::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {	
		std::map<std::string, std::string> para = this->step.params;
		return QString::fromStdString(para[this->paramNames[index.row()]]); 
    } 
    return QVariant();
}


QVariant ProcessingStepSettings::headerData(int section, Qt::Orientation orientation, int role) const {
 
	if(role == Qt::DisplayRole) {
		std::stringstream ss;
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


void ProcessingStepSettings::setProcessingStep(uipf::ProcessingStep proSt){
	
	beginResetModel();
	
	step = proSt;
	
	paramNames.clear();
	std::map<std::string, std::string> par = proSt.params;
	for (auto it = par.begin(); it!=par.end(); ++it) {
		paramNames.push_back(it->first);
	}
	
	endResetModel();
}

