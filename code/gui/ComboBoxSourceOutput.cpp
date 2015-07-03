#include "ComboBoxSourceOutput.hpp"

#include <QComboBox>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>

#include <iostream>

using namespace std;
using namespace uipf;

ComboBoxSourceOutput::ComboBoxSourceOutput(ModuleManager& mm, QObject *parent) : QItemDelegate(parent) , mm_(mm) {
}

void ComboBoxSourceOutput::setConfiguration(Configuration conf, std::string currentStep) {
	conf_ = conf;
	currentStepName = currentStep;

	items_.clear();
	selected_.clear();

	// fill vector of possible outputs for each referenced module
	ProcessingStep step = conf_.getProcessingStep(currentStepName);
	for(auto it = step.inputs.cbegin(); it != step.inputs.end(); ++it) {

		vector<string> subItems;
		if (conf_.hasProcessingStep(it->second.first)) {
			// only possible to add items if the referenced step exists and we can get its module
			ProcessingStep referencedStep = conf_.getProcessingStep(it->second.first);

			// fill vector of possible output selections
			map<string, DataDescription> out = mm_.getModuleMetaData(referencedStep.module).getOutputs();
			for (auto oit = out.cbegin(); oit!=out.end(); ++oit) {
				subItems.push_back(oit->first);
			}
		}
		items_.push_back(subItems);

		// fill vector of selected outputs
		selected_.push_back(it->second.second);
	}
}

// create editor widget
QWidget *ComboBoxSourceOutput::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex & index) const {
	QComboBox* editor = new QComboBox(parent); // will be deleted by Qt automatically http://doc.qt.io/qt-5/qabstractitemdelegate.html#destroyEditor
	for(unsigned int i = 0; i < items_[index.row()].size(); ++i) {
		QString item = QString(items_[index.row()][i].c_str());
		editor->insertItem(i, item, item);
	}
	return editor;
}

// set column content for edit-mode
void ComboBoxSourceOutput::setEditorData(QWidget *editor, const QModelIndex &index) const {
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	int value = comboBox->findData(QString(selected_[index.row()].c_str()));
	comboBox->setCurrentIndex(value);
}

// set column content for non edit-mode
void ComboBoxSourceOutput::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
	//~ QComboBox *comboBox = static_cast<QComboBox*>(editor);
	model->setData(index, QString(selected_[index.row()].c_str()), Qt::EditRole);
}
