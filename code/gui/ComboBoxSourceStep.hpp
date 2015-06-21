#ifndef COMBOBOXSOURCESTEP_H
#define COMBOBOXSOURCESTEP_H
 
#include <string>
#include <vector>
#include <../framework/Configuration.hpp>
 
#include <QItemDelegate>
 
class QModelIndex;
class QWidget;
class QVariant;
 
namespace uipf { 
 
class ComboBoxSourceStep : public QItemDelegate
{
Q_OBJECT
public:
	ComboBoxSourceStep(QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void fill();

	void setConfiguration(Configuration);
	void setCurrentStep(std::string); 
 
private:
  std::vector<std::string> Items;
  
  	// the currently loaded configuration represented in the window
   	Configuration conf_;
  	// current name of a precessing step
	std::string currentStepName;
 
};

}
#endif

