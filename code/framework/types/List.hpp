#ifndef LIST_H
#define LIST_H

#include <list>

namespace uipf{

template <typename T>
// List of Data which is a specification of Data
class List : public Data {
	public:
		typedef SMARTPOINTER<List<T> > ptr;
		typedef const SMARTPOINTER<List<T> > c_ptr;

	public:
		// constructor
		List(void){};
		// destructor
		~List(void){};

		// get content
		std::list<T> getContent();

		// set content
		void setContent(std::list<T>);

		// returns the data type of this data object
		Type getType() override;

	private:
		// content
		std::list<T> lst;
};

// returns the Data
/*
*/
template <typename T>
std::list<T> List<T>::getContent(){
	return lst;
}

// sets the Data
/*
d	list<Data> content
*/
template <typename T>
void List<T>::setContent(std::list<T> l){
	lst = l;
}


// returns the data type of this data object
template <typename T>
Type List<T>::getType(){
	return LIST;
}

}

#endif
