#ifndef MAP_H
#define MAP_H

#include <map>

namespace uipf{

template <typename K, typename V>
// Map of Data to Data, which is a specification of Data
class Map : public Data {
	public:
		typedef SMARTPOINTER<Map<K,V> > ptr;
		typedef const SMARTPOINTER<Map<K,V> > c_ptr;

	public:
		// constructor
		Map(void){};
		// destructor
		~Map(void){};

		// get content
		std::map<K,V> getContent();

		// set content
		void setContent(std::map<K,V>);

		// returns the data type of this data object
		Type getType() override;

	private:
		// content
		std::map<K,V> mp;
};

// returns the Data
/*
*/
template <typename K, typename V>
map<K,V> Map<K,V>::getContent(){
	return mp;
}

// sets the Data
/*
m	map<Key,Value> content
*/
template <typename K, typename V>
void Map<K,V>::setContent(std::map<K,V> m){
	mp = m;
}


// returns the data type of this data object
template <typename K, typename V>
Type Map<K,V>::getType(){
	return MAP;
}

}
#endif
