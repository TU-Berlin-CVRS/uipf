#ifndef InputsDelegate_H
#define InputsDelegate_H

#include <string>
#include <vector>

#include <QItemDelegate>

#include <../framework/Configuration.hpp>
#include <../framework/ModuleManager.hpp>
#include <../framework/MetaData.hpp>

class QModelIndex;
class QWidget;
class QVariant;

namespace uipf {

class InputsDelegate : public QItemDelegate
{
Q_OBJECT
public:
	InputsDelegate(ModuleManager&, QObject *parent = 0);

	// create editor widget
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	// set column content for edit-mode
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	// set column content for non edit-mode
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	// updates the model by setting the current configuration
	void setConfiguration(const Configuration&, const std::string&, std::vector<std::string>);

signals:
	void inputChanged(std::string, std::pair<std::string, std::string>) const;

private:
	ModuleManager& mm_;

	// available options for the step dropdown box
	std::vector<std::string> stepItems_;

	// available options for the output dropdown box
	std::vector< std::vector<std::string> > outputItems_;

	// reference to input names, row index => input name
	std::vector<std::string> inputNames_;

	std::vector<std::string> optionalInputs_;

};

} // namespace

#endif
