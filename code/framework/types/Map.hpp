#ifndef MAP_H
#define MAP_H

#include <map>

namespace uipf{

// Map of Data to Data, which is a specification of Data
// The keys of the map are also Data, but are all of the same Type!
// The values of the map are also Data, but are all of the same Type!
// This class is a template, for this reason, there is no .cpp file and the implementation is done inside the .hpp file
template <typename K, typename V>
class Map : public Data {
	public:
		typedef SMARTPOINTER<Map<K,V> > ptr;
		typedef const SMARTPOINTER<Map<K,V> > c_ptr;

	public:
		// constructor
		Map(void){};
		// destructor
		~Map(void){};

		// returns the content of the map
		std::map<K,V> getContent();

		// sets the content of the map
		void setContent(std::map<K,V>);

		// returns the data type of this data object: in this case: MAP
		Type getType() override;

	private:
		// content of the map
		std::map<K,V> map_;
};


/*----------------------implementation----------------------------------*/

// returns the content of the list
template <typename K, typename V>
map<K,V> Map<K,V>::getContent(){
	return map_;
}

// sets the content of the map
/*
map		map<Keys,Values> content
*/
template <typename K, typename V>
void Map<K,V>::setContent(std::map<K,V> map){
	map_ = map;
}


// returns the data type of this data object: in this case: MAP
template <typename K, typename V>
Type Map<K,V>::getType(){
	return MAP;
}

}
#endif
