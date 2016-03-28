/*
 * StaticList.h
 *
 *  Created on: 24 mar 2016
 *      Author: witek
 */

#ifndef LIST_STATICLIST_H_
#define LIST_STATICLIST_H_

#include <stdint.h>
#include <string.h>

template<class T>
class StaticList
{
private:
	T *internalTable;
	uint8_t maxSize;

	uint8_t size;
public:
	StaticList(T* internalTable, uint8_t maxSize);

	uint8_t getSize();
	uint8_t getMaxSize();
	bool add(T* object);
	T* get(uint8_t index);
	void remove(uint8_t index);
};

template <class T>
StaticList<T>::StaticList(T* internalTable, uint8_t maxSize) {
	this->internalTable = internalTable;
	this->maxSize = maxSize;
	this->size = 0;
}

template <class T>
uint8_t StaticList<T>::getSize() {
	return this->size;
}

template <class T>
uint8_t StaticList<T>::getMaxSize() {
	return this->maxSize;
}

template <class T>
bool StaticList<T>::add(T* object) {
	if(size == maxSize) {
		return false;
	}

	memcpy(internalTable + size, object, sizeof(T));
	size ++;

	return true;
}

template <class T>
T* StaticList<T>::get(uint8_t index) {
	return (internalTable + index);
}

template <class T>
void StaticList<T>::remove(uint8_t index) {
	if(size == 0) {
		return;
	}

	if(index > size -1) {
		return;
	}

	if(index == size -1) {
		size --;

		return;
	}

	// need to shift left
	for(uint8_t q = index ; q < size - 1 ; q ++) {
		memcpy(internalTable + q, internalTable + q + 1, sizeof(T));
	}

	size --;
}

#endif /* LIST_STATICLIST_H_ */
