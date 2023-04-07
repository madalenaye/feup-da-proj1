#ifndef DA_PRIORITYQUEUE_H
#define DA_PRIORITYQUEUE_H

#include <vector>
#include "VertexEdge.h"

class PriorityQueue {
	std::vector<Vertex *> Heap;
	void heapifyUp(unsigned i);
	void heapifyDown(unsigned i);
	inline void set(unsigned i, Vertex* x);
public:
	PriorityQueue();
	void insert(Vertex * x);
    Vertex * extractMin();
	void decreaseKey(Vertex* x);
	bool empty();
};

#define parent(i) ((i) / 2)
#define leftChild(i) ((i) * 2)

#endif //DA_PRIORITYQUEUE_H
