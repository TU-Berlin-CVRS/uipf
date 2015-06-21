#include "ComboBoxSourceOutput.hpp"
 
#include <QComboBox>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>
 
#include <iostream>
 
using namespace std;
using namespace uipf; 
 
ComboBoxSourceOutput::ComboBoxSourceOutput(QObject *parent) :QItemDelegate(parent) {
}
 
void ComboBoxSourceOutput::setConfiguration(Configuration conf){
	conf_ = conf;
}

void ComboBoxSourceOutput::setCurrentStep(std::string name){
	currentStepName = name;
}  

void ComboBoxSourceOutput::setModuleManager(ModuleManager mm){
	mm_ = mm;
}  


void ComboBoxSourceOutput::fill(){
	map<string, ProcessingStep> chain = conf_.getProcessingChain();
	map<string, DataDescription> out = mm_.getAllModuleMetaData()[chain[currentSelectedSource].module].getOutputs();
	
	for (auto it = out.begin(); it!=out.end(); ++it) {
		string value = (it->first).c_str();
		Items.push_back(value);
	}
}

QWidget *ComboBoxSourceOutput::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const {
  QComboBox* editor = new QComboBox(parent);
  for(unsigned int i = 0; i < Items.size(); ++i) {
    editor->addItem(Items[i].c_str());
  }
  return editor;
}
 
void ComboBoxSourceOutput::setEditorData(QWidget *editor, const QModelIndex &index) const {
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  int value = index.model()->data(index, Qt::EditRole).toUInt();
  comboBox->setCurrentIndex(value);
}
 
void ComboBoxSourceOutput::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  model->setData(index, comboBox->currentIndex(), Qt::EditRole);
}
 
void ComboBoxSourceOutput::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const {
  editor->setGeometry(option.rect);
}
 
void ComboBoxSourceOutput::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  QStyleOptionViewItemV4 myOption = option;
  QString text = Items[index.row()].c_str();
 
  myOption.text = text;
 
  QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}

