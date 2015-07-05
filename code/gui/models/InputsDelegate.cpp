#include "InputsDelegate.hpp"

#include <QComboBox>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>

#include <iostream>

using namespace std;
using namespace uipf;

InputsDelegate::InputsDelegate(ModuleManager& mm, QObject *parent) : QItemDelegate(parent) , mm_(mm) {

}

void InputsDelegate::setConfiguration(const Configuration& conf, const std::string& currentStepName, std::vector<std::string> inputNames) {

	inputNames_ = inputNames;

	stepItems_.clear();
	outputItems_.clear();

	map<string, MetaData> moduleMetaData = mm_.getAllModuleMetaData();

	// fill vector of possible reference steps
	map<string, ProcessingStep> chain = conf.getProcessingChain();
	for (auto it = chain.begin(); it!=chain.end(); ++it) {
		// the current step can not depend on itself
		if (it->first.compare(currentStepName) == 0) {
			continue;
		}
		// also only steps are listed that have at least one output
		auto metaIt = moduleMetaData.find(it->second.module);
		if (metaIt != moduleMetaData.end() && metaIt->second.getOutputs().empty()) {
			continue;
		}

		stepItems_.push_back(it->first.c_str());
	}

	ProcessingStep step = conf.getProcessingStep(currentStepName);
	for(auto it = step.inputs.cbegin(); it != step.inputs.end(); ++it) {

		// fill optional inputs
		auto ownMetaIt = moduleMetaData.find(step.module);
		if (ownMetaIt != moduleMetaData.end()) {
			map<string, DataDescription> in = ownMetaIt->second.getInputs();
			for (auto iit = in.cbegin(); iit!=in.end(); ++iit) {
				if (iit->second.getIsOptional()) {
					optionalInputs_.push_back(iit->first);
				}
			}
		}

		// fill vector of possible outputs for each referenced module
		vector<string> subItems;
		if (conf.hasProcessingStep(it->second.first)) {
			// only possible to add items if the referenced step exists and we can get its module
			ProcessingStep referencedStep = conf.getProcessingStep(it->second.first);

			// fill vector of possible output selections if module exists
			auto metaIt = moduleMetaData.find(referencedStep.module);
			if (metaIt != moduleMetaData.end()) {
				map<string, DataDescription> out = metaIt->second.getOutputs();
				for (auto oit = out.cbegin(); oit!=out.end(); ++oit) {
					subItems.push_back(oit->first);
				}
			}
		}
		outputItems_.push_back(subItems);
	}

}

// create editor widget
QWidget *InputsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex & index) const {
	QComboBox* editor = new QComboBox(parent); // will be deleted by Qt automatically http://doc.qt.io/qt-5/qabstractitemdelegate.html#destroyEditor

	// first column is the processing step
	if (index.column() == 0) {
		string inputName = inputNames_[index.row()];
		bool isOptional = false;
		for (unsigned int i = 0; i < optionalInputs_.size(); ++i) {
			if (inputName.compare(optionalInputs_[i]) == 0) {
				isOptional = true;
				break;
			}
		}
		int k = 0;
		// add a selection that is empty for optional inputs
		if (isOptional) {
			editor->insertItem(k++, QString(""), QString(""));
		}
		for(unsigned int i = 0; i < stepItems_.size(); ++i) {
			editor->insertItem(k++, QString(stepItems_[i].c_str()), QString(stepItems_[i].c_str()));
		}
	}
	// second column is the output of the source module
	else if (index.column() == 1) {
		vector<string> items = outputItems_[index.row()];
		for(unsigned int i = 0; i < items.size(); ++i) {
			editor->insertItem(i, QString(items[i].c_str()), QString(items[i].c_str()));
		}
	}
	return editor;
}

// set column content for edit-mode
void InputsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	int value = comboBox->findData(index.data());
	comboBox->setCurrentIndex(value);
}

// set column content for non edit-mode
void InputsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	string selectedItem = comboBox->currentData().toString().toStdString();
	model->setData(index, QString(selectedItem.c_str()));

	//cout << "set model data " << index.row() << "," << index.column() << " : " << selectedItem << endl;

	pair<string, string> newInput(
		model->data(model->index(index.row(), 0)).toString().toStdString(),
		model->data(model->index(index.row(), 1)).toString().toStdString()
	);
	emit inputChanged(inputNames_[index.row()], newInput);
}
