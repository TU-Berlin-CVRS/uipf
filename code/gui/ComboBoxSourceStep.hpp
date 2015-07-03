#ifndef COMBOBOXSOURCESTEP_H
#define COMBOBOXSOURCESTEP_H

#include <string>
#include <vector>

#include <QItemDelegate>

#include <../framework/Configuration.hpp>

class QModelIndex;
class QWidget;
class QVariant;

namespace uipf {

class ComboBoxSourceStep : public QItemDelegate
{
Q_OBJECT
public:
	ComboBoxSourceStep(QObject *parent = 0);

	// create editor widget
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	// set column content for edit-mode
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	// set column content for non edit-mode
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

	// updates the model by setting the current configuration
	void setConfiguration(Configuration, std::string);

signals:
	void inputChanged(std::string, std::pair<std::string, std::string>);

private:
	// available options for the dropdown box
	std::vector<std::string> items_;

	// currently selected reference steps
	std::vector<std::string> selected_;

  	// the currently loaded configuration represented in the window
   	Configuration conf_;
  	// current name of a precessing step
	std::string currentStepName;

};

} // namespace

#endif
