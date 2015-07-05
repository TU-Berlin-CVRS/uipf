#ifndef LIST_H
#define LIST_H

#include <list>
#include "Data.hpp"

namespace uipf{

// List of Data which is a specification of Data.
// The elements of the list are also Data, but are all of the same Type!
// This class is a template, for this reason, there is no .cpp file and the implementation is done inside the .hpp file
template <typename T>
class List : public Data {
	public:
		typedef SMARTPOINTER<List<T> > ptr;
		typedef const SMARTPOINTER<List<T> > c_ptr;

	public:
		// constructor
		List() {};
		List(std::list<typename T::ptr>& l) : list_(l) {};
		// destructor
		~List(void){};

		// returns the content of the list
		std::list<typename T::ptr> getContent();

		// sets the content of the list
		void setContent(std::list<typename T::ptr>);

		// check whether item with index exists
		bool hasItem(int index);

		// returns an item in the list or nullptr if it does not exist, check with hasItem() before doing this.
		typename T::ptr getItem(int index);

		// add an item to the list by either passing a new element or an existing pointer
		void addItem(T* item);
		void addItem(typename T::ptr item);

		// returns the data type of this data object: in this case: LIST
		Type getType() override;

	private:
		// content of the list
		std::list<typename T::ptr> list_;
};


/*----------------------implementation----------------------------------*/

// returns the content of the list
template <typename T>
std::list<typename T::ptr> List<T>::getContent(){
	return list_;
}

// sets the content of the list
/*
list	list<Data> content
*/
template <typename T>
void List<T>::setContent(std::list<typename T::ptr> list){
	list_ = list;
}

template<typename T>
bool List<T>::hasItem(int index) {

	int i = 0;
	for(auto it = list_.begin(); it != list_.end(); ++it) {
		if (i++ == index) {
			return true;
		}
	}
	return false;
}

template<typename T>
typename T::ptr List<T>::getItem(int index) {

	int i = 0;
	for(auto it = list_.begin(); it != list_.end(); ++it) {
		if (i++ == index) {
			return *it;
		}
	}
	return nullptr;
}

template<typename T>
void List<T>::addItem(T* item) {

	list_.push_back(typename T::ptr(item));
}

template<typename T>
void List<T>::addItem(typename T::ptr item) {

	list_.push_back(item);
}


// returns the data type of this data object: in this case: LIST
template <>
inline Type List<String>::getType() { return STRING_LIST; }

template <>
inline Type List<Integer>::getType() { return INTEGER_LIST; }

template <>
inline Type List<Float>::getType() { return FLOAT_LIST; }

template <>
inline Type List<Bool>::getType() { return BOOL_LIST; }

template <>
inline Type List<Matrix>::getType() { return MATRIX_LIST; }



} // namespace

#endif
