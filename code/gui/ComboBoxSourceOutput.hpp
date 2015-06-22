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

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void fill();

	void setConfiguration(Configuration);
	void setCurrentStep(std::string);
	void setCurrentSelectedSource(std::string);

private:
  std::vector<std::string> Items;

  	// the currently loaded configuration represented in the window
   	Configuration conf_;
  	// current name of a precessing step
	std::string currentStepName;
	// current selected source, set by ComboBoxSourceStep
	std::string currentSelectedSource = "process";	// TODO change it!

	ModuleManager mm_;

};

}
#endif

