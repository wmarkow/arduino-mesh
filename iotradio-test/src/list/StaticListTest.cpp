/*
 * StaticVectorTest.cpp
 *
 *  Created on: 27 mar 2016
 *      Author: witek
 */

#include <src/list/StaticList.h>
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#define MAX_COUNT 3

void testConstructor() {
	uint8_t internalTable[MAX_COUNT];
	StaticList<uint8_t> list = StaticList<uint8_t>(internalTable, MAX_COUNT);

	ASSERT(list.getMaxSize() == MAX_COUNT);
	ASSERT(list.getSize() == 0);
}

void testAddUntilFull() {
	uint8_t internalTable[3];
	StaticList<uint8_t> list = StaticList<uint8_t>(internalTable, MAX_COUNT);

	uint8_t value = 123;

	ASSERT(list.add(&value));
	ASSERT(list.getMaxSize() == MAX_COUNT);
	ASSERT(list.getSize() == 1);

	ASSERT(list.add(&value));
	ASSERT(list.getMaxSize() == MAX_COUNT);
	ASSERT(list.getSize() == 2);

	ASSERT(list.add(&value));
	ASSERT(list.getMaxSize() == MAX_COUNT);
	ASSERT(list.getSize() == 3);

	ASSERT(!list.add(&value));
	ASSERT(list.getMaxSize() == MAX_COUNT);
	ASSERT(list.getSize() == 3);
}

void testGet() {
	uint8_t internalTable[3];
	StaticList<uint8_t> list = StaticList<uint8_t>(internalTable, MAX_COUNT);

	uint8_t value1 = 123;
	uint8_t value2 = 23;
	uint8_t value3 = 254;

	list.add(&value1);
	list.add(&value2);
	list.add(&value3);

	ASSERT((*list.get(0)) == 123);
	ASSERT((*list.get(1)) == 23);
	ASSERT((*list.get(2)) == 254);
}

void testRemoveWhenZeroSize() {
	uint8_t internalTable[3];
	StaticList<uint8_t> list = StaticList<uint8_t>(internalTable, MAX_COUNT);

	list.remove(0);

	ASSERT(list.getSize() == 0);
}

void testRemoveWhenIndexOutOfArray() {
	uint8_t internalTable[3];
	StaticList<uint8_t> list = StaticList<uint8_t>(internalTable, MAX_COUNT);

	uint8_t value1 = 123;
	uint8_t value2 = 23;
	uint8_t value3 = 254;

	list.add(&value1);
	list.add(&value2);
	list.add(&value3);

	list.remove(3);

	ASSERT(list.getSize() == 3);
	ASSERT((*list.get(0)) == 123);
	ASSERT((*list.get(1)) == 23);
	ASSERT((*list.get(2)) == 254);
}

void testRemoveLast() {
	uint8_t internalTable[3];
	StaticList<uint8_t> list = StaticList<uint8_t>(internalTable, MAX_COUNT);

	uint8_t value1 = 123;
	uint8_t value2 = 23;
	uint8_t value3 = 254;

	list.add(&value1);
	list.add(&value2);
	list.add(&value3);

	list.remove(2);

	ASSERT(list.getSize() == 2);
	ASSERT((*list.get(0)) == 123);
	ASSERT((*list.get(1)) == 23);
}

void testRemoveInside() {
	uint8_t internalTable[3];
	StaticList<uint8_t> list = StaticList<uint8_t>(internalTable, MAX_COUNT);

	uint8_t value1 = 123;
	uint8_t value2 = 23;
	uint8_t value3 = 254;

	list.add(&value1);
	list.add(&value2);
	list.add(&value3);

	list.remove(1);

	ASSERT(list.getSize() == 2);
	ASSERT((*list.get(0)) == 123);
	ASSERT((*list.get(1)) == 254);
}

void testRemoveFirst() {
	uint8_t internalTable[3];
	StaticList<uint8_t> list = StaticList<uint8_t>(internalTable, MAX_COUNT);

	uint8_t value1 = 123;
	uint8_t value2 = 23;
	uint8_t value3 = 254;

	list.add(&value1);
	list.add(&value2);
	list.add(&value3);

	list.remove(0);

	ASSERT(list.getSize() == 2);
	ASSERT((*list.get(0)) == 23);
	ASSERT((*list.get(1)) == 254);
}

void runAllTests(int argc, char const *argv[]){
	cute::suite s;
	//TODO add your test here
	s.push_back(CUTE(testConstructor));
	s.push_back(CUTE(testAddUntilFull));
	s.push_back(CUTE(testGet));
	s.push_back(CUTE(testRemoveWhenZeroSize));
	s.push_back(CUTE(testRemoveWhenIndexOutOfArray));
	s.push_back(CUTE(testRemoveLast));
	s.push_back(CUTE(testRemoveInside));
	s.push_back(CUTE(testRemoveFirst));
	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<> >  lis(xmlfile.out);
	cute::makeRunner(lis,argc,argv)(s, "AllTests");
}

int main(int argc, char const *argv[]){
    runAllTests(argc,argv);
    return 0;
}
