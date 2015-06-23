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

void ComboBoxSourceStep::setConfiguration(Configuration conf, std::string currentStep) {
	conf_ = conf;
	currentStepName = currentStep;

	items_.clear();
	selected_.clear();

	// fill vector of possible reference steps
	map<string, ProcessingStep> chain = conf_.getProcessingChain();
	for (auto it = chain.begin(); it!=chain.end(); ++it) {
		// the current step can not depend on itself
		if (it->first.compare(currentStepName) != 0) {
			items_.push_back(it->first.c_str());
		}
	}

	// fill vector of selected reference steps
	ProcessingStep step = conf_.getProcessingStep(currentStepName);
	for(auto it = step.inputs.cbegin(); it != step.inputs.end(); ++it) {
		selected_.push_back(it->second.first);
	}
}

// create editor widget
QWidget *ComboBoxSourceStep::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const {
	QComboBox* editor = new QComboBox(parent); // will be deleted by Qt automatically http://doc.qt.io/qt-5/qabstractitemdelegate.html#destroyEditor
	for(unsigned int i = 0; i < items_.size(); ++i) {
		editor->insertItem(i, QString(items_[i].c_str()), QString(items_[i].c_str()));
	}
	return editor;
}

// set column content for edit-mode
void ComboBoxSourceStep::setEditorData(QWidget *editor, const QModelIndex &index) const {
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	int value = comboBox->findData(QString(selected_[index.row()].c_str()));
	comboBox->setCurrentIndex(value);
}

// set column content for non edit-mode
void ComboBoxSourceStep::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	model->setData(index, QString(items_[comboBox->currentIndex()].c_str()), Qt::EditRole);
}
