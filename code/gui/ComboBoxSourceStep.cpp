#include "ComboBoxSourceStep.hpp"

#include <QComboBox>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>

#include <iostream>

using namespace std;
using namespace uipf;

ComboBoxSourceStep::ComboBoxSourceStep(QObject *parent) :QItemDelegate(parent) {
}

void ComboBoxSourceStep::setConfiguration(Configuration conf){
	conf_ = conf;
}

void ComboBoxSourceStep::setCurrentStep(std::string name){
	currentStepName = name;
}


void ComboBoxSourceStep::fill(){
	map<string, ProcessingStep> chain = conf_.getProcessingChain();
	for (auto it = chain.begin(); it!=chain.end(); ++it) {
		string value = it->first.c_str();
		Items.push_back(value);
	}
}

QWidget *ComboBoxSourceStep::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const {
  QComboBox* editor = new QComboBox(parent);
  for(unsigned int i = 0; i < Items.size(); ++i) {
    editor->addItem(Items[i].c_str());
  }
  return editor;
}

void ComboBoxSourceStep::setEditorData(QWidget *editor, const QModelIndex &index) const {
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  int value = index.model()->data(index, Qt::EditRole).toUInt();
  comboBox->setCurrentIndex(value);
}

void ComboBoxSourceStep::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  model->setData(index, comboBox->currentIndex(), Qt::EditRole);
}

void ComboBoxSourceStep::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const {
  editor->setGeometry(option.rect);
}

void ComboBoxSourceStep::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  QStyleOptionViewItemV4 myOption = option;
  QString text = Items[index.row()].c_str();

  myOption.text = text;

  QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}

