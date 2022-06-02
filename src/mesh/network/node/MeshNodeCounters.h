/*
 * MeshNodeCounters.h
 *
 *  Created on: 23 lut 2016
 *      Author: witek
 */

#ifndef MESH_NETWORK_NODE_MESHNODECOUNTERS_H_
#define MESH_NETWORK_NODE_MESHNODECOUNTERS_H_

class MeshNodeCounters
{
private:
	unsigned int droppedCount = 0;
	unsigned int floodedCount = 0;

public:
	void incDroppedCount();
	void incFloodedCount();

	unsigned int getDroppedCount();
	unsigned int getFloodedCount();
};



#endif /* MESH_NETWORK_NODE_MESHNODECOUNTERS_H_ */
