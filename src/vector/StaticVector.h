/*
 * StaticList.h
 *
 *  Created on: 24 mar 2016
 *      Author: witek
 */

#ifndef VECTOR_STATICVECTOR_H_
#define VECTOR_STATICVECTOR_H_

#include <stdint.h>

template<class T>
class StaticVector
{
private:
	T *internalTable;
	uint8_t maxCount;

	uint8_t count;
public:
	StaticVector(T* internalTable, uint8_t maxCount);

	uint8_t getCount();
	uint8_t getMaxCount();
	bool pushBack(T* object);
	T* peek(uint8_t index);
};

template <class T>
StaticVector<T>::StaticVector(T* internalTable, uint8_t maxCount) {
	this->internalTable = internalTable;
	this->maxCount = maxCount;
	this->count = 0;
}

template <class T>
uint8_t StaticVector<T>::getCount() {
	return this->count;
}

template <class T>
uint8_t StaticVector<T>::getMaxCount() {
	return this->maxCount;
}

template <class T>
bool StaticVector<T>::pushBack(T* object) {
	if(count == maxCount) {
		return false;
	}

	memcpy(internalTable + count, object, sizeof(T));
	count ++;

	return true;
}

template <class T>
T* StaticVector<T>::peek(uint8_t index) {
	return (internalTable + index);
}

#endif /* VECTOR_STATICVECTOR_H_ */
