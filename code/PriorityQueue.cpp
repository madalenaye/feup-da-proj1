#include "PriorityQueue.h"

PriorityQueue::PriorityQueue() {
    Heap.push_back(nullptr);
}

/**
 * This function checks whether the priority queue is empty or not.
 *
 * @return True if the size of the heap is 1, which means that there are no elements in the queue
 * (since the 0th index of the heap is not used), false otherwise.
 *
 * @par Time complexity
 * O(1)
 */
bool PriorityQueue::empty() {
    return Heap.size() == 1;
}

/**
 * This function extracts the minimum element from the priority queue.
 *
 * @return A pointer to the minimum element of the queue.
 *
 * @par Time complexity
 * O(log n), where n is the number of elements in the queue.
 */
Vertex* PriorityQueue::extractMin() {
    auto x = Heap[1];
    Heap[1] = Heap.back();
    Heap.pop_back();
    if(Heap.size() > 1) heapifyDown(1);
    x->queueIndex = 0;
    return x;
}

/**
 * This function inserts a new element into the priority queue.
 * @param x A pointer to the element to be inserted.
 *
 * @par Time complexity
 * O(log n), where n is the number of elements in the queue.
 */
void PriorityQueue::insert(Vertex* x) {
    Heap.push_back(x);
    heapifyUp(Heap.size()-1);
}

/**
 * This function decreases the priority of an element in the queue.
 * @param x A pointer to the element whose priority is to be decreased.
 *
 * @par Time complexity
 * O(log n), where n is the number of elements in the queue.
 */
void PriorityQueue::decreaseKey(Vertex *x) {
    heapifyUp(x->queueIndex);
}

/**
 * This function maintains the heap property by swapping the element at position i with its parent,
 * as long as the parent has a higher priority than the element.
 *
 * @param i The index of the element that may violate the heap property.
 *
 * @par Time complexity
 * O(log n), where n is the number of elements in the queue.
 */
void PriorityQueue::heapifyUp(unsigned i) {
    auto x = Heap[i];
    while (i > 1 && *x < *Heap[parent(i)]) {
        set(i, Heap[parent(i)]);
        i = parent(i);
    }
    set(i, x);
}

/**
 * This function maintains the heap property by swapping the element at position i with its smallest
 * child, as long as the child has a higher priority than the element.
 *
 * @param i The index of the element that may violate the heap property.
 *
 * @par Time complexity
 * O(log n), where n is the number of elements in the queue.
 */
void PriorityQueue::heapifyDown(unsigned i) {
    auto x = Heap[i];
    while (true) {
        unsigned k = leftChild(i);
        if (k >= Heap.size())
            break;
        if (k+1 < Heap.size() && *Heap[k+1] < *Heap[k])
            ++k; // right child of i
        if ( ! (*Heap[k] < *x) )
            break;
        set(i, Heap[k]);
        i = k;
    }
    set(i, x);
}

/**
 * This function updates the value of an element in the heap and its queueIndex field.
 * @param i The new queueIndex of x.
 * @param x The element to be updated.
 *
 * @par Time complexity
 * O(1)
 */
void PriorityQueue::set(unsigned i, Vertex* x) {
    Heap[i] = x;
    x->queueIndex = i;
}
