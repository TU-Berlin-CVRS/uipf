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
		List(std::list<typename T::ptr>& l) : list_(l) {};
		// destructor
		~List(void){};

		// returns the content of the list
		std::list<typename T::ptr> getContent();

		// sets the content of the list
		void setContent(std::list<typename T::ptr>);

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


// returns the data type of this data object: in this case: LIST
template <typename T>
Type List<T>::getType(){
	return LIST;
}

} // namespace

#endif
