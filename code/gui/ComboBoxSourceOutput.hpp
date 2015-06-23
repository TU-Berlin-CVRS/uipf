#ifndef COMBOBOXSOURCEOUTPUT_H
#define COMBOBOXSOURCEOUTPUT_H

#include <string>
#include <vector>
#include <../framework/Configuration.hpp>
#include <../framework/ModuleManager.hpp>
#include <../framework/MetaData.hpp>

#include <QItemDelegate>

class QModelIndex;
class QWidget;
class QVariant;

namespace uipf {

class ComboBoxSourceOutput : public QItemDelegate
{
Q_OBJECT
public:
	ComboBoxSourceOutput(ModuleManager&, QObject *parent = 0);

	// create editor widget
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	// set column content for edit-mode
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	// set column content for non edit-mode
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	// updates the model by setting the current configuration
	void setConfiguration(Configuration, std::string);

private:
	ModuleManager& mm_;

	// available options for the dropdown box
	std::vector< std::vector<std::string> > items_;

	// currently selected reference steps
	std::vector<std::string> selected_;

  	// the currently loaded configuration represented in the window
   	Configuration conf_;
  	// current name of a precessing step
	std::string currentStepName;
};

}
#endif

