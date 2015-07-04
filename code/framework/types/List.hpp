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
		List(void){};
		// destructor
		~List(void){};

		// returns the content of the list
		std::list<T> getContent();

		// sets the content of the list
		void setContent(std::list<T>);

		// returns the data type of this data object: in this case: LIST
		Type getType() override;

	private:
		// content of the list
		std::list<T> list_;
};


/*----------------------implementation----------------------------------*/

// returns the content of the list
template <typename T>
std::list<T> List<T>::getContent(){
	return list_;
}

// sets the content of the list
/*
list	list<Data> content
*/
template <typename T>
void List<T>::setContent(std::list<T> list){
	list_ = list;
}


// returns the data type of this data object: in this case: LIST
template <typename T>
Type List<T>::getType(){
	return LIST;
}

} // namespace

#endif
