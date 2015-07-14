#include "ParamsModel.hpp"

using namespace std;
using namespace uipf;

// constructor
ParamsModel::ParamsModel(ModuleManager& mm, QObject *parent) : QAbstractTableModel(parent),  mm_(mm) {

}

// sets the number of rows for the widget
int ParamsModel::rowCount(const QModelIndex & /*parent*/) const
{
	return paramNames_.size();
}

// sets the number of columns for the widget
int ParamsModel::columnCount(const QModelIndex & /*parent*/) const
{
	return 1;
}

// this method is triggered when a parameter has been edited
bool ParamsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
		// only the values can be changed
		if (index.column() == 0) {
			step_.params[paramNames_[index.row()]] = value.toString().toStdString();
			emit paramChanged(paramNames_[index.row()], value.toString().toStdString());
		}

		QModelIndex transposedIndex = createIndex(index.column(), index.row());
		emit dataChanged(index, index);
		emit dataChanged(transposedIndex, transposedIndex);
		return true;
    }
    return false;
}

// describes the data that is displayed in the widget
QVariant ParamsModel::data(const QModelIndex &index, int role) const
{
	map<string, string> para = step_.params;
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
		return QString::fromStdString(para[paramNames_[index.row()]]);
	}
	else if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
	} else if (role == Qt::ToolTipRole && mm_.hasModule(step_.module) && mm_.getModuleMetaData(step_.module).getParams().count(paramNames_[index.row()]) > 0) {
		// show param description as tooltip info
		string descr = mm_.getModuleMetaData(step_.module).getParam(paramNames_[index.row()]).getDescription();
		return QString::fromStdString(descr);
	} else
    return QVariant();
}

// describes what is displayed in the headers of the table
QVariant ParamsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			return QString("Value");
		}
		else if(orientation == Qt::Vertical) {
			return QString(paramNames_[section].c_str());
		}
	} else if (role == Qt::ToolTipRole && mm_.hasModule(step_.module)) {
		// show param description as tooltip info
		MetaData meta = mm_.getModuleMetaData(step_.module);
		if(orientation == Qt::Vertical && meta.getParams().count(paramNames_[section]) > 0) {
			string descr = meta.getParam(paramNames_[section]).getDescription();
			return QString::fromStdString(descr);
		}
	}
	QSize size(100, 30);
	if (section == 0) {
		size.setWidth(200);
	}

	return QVariant(size);
}

// sets the processing step this widget represents and updates the content
void ParamsModel::setProcessingStep(ProcessingStep proSt)
{
	beginResetModel();

	step_ = proSt;

	paramNames_.clear();
	map<string, string> params = step_.params;
	for (auto it = params.begin(); it != params.end(); ++it) {
		paramNames_.push_back(it->first);
	}

	endResetModel();
}

// set flags for the fields, allows them to be editable
Qt::ItemFlags ParamsModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
