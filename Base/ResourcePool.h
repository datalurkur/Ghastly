#ifndef RESOURCEPOOL_H
#define RESOURCEPOOL_H

#include <Base/Base.h>
#include <Base/Log.h>

template <typename T>
class ResourcePool {
public:
    ResourcePool();
    ResourcePool(const unsigned int capacity);
    ~ResourcePool();

    T* allocate();
    void free(T* t);

private:
    void reallocate(const unsigned int capacity);
    void expand();

private:
    unsigned int _capacity;
    T* _nodes;

    typedef std::stack<T*> NodeStack;
    NodeStack _freeNodes;
};

template <typename T>
ResourcePool<T>::ResourcePool():
    _capacity(0), _nodes(0)
{}

template <typename T>
ResourcePool<T>::ResourcePool(const unsigned int capacity):
    _capacity(0), _nodes(0)
{
    Info("Capacity is " << _capacity);
    reallocate(capacity);
}

template <typename T>
ResourcePool<T>::~ResourcePool() {
    while(!_freeNodes.empty()) { _freeNodes.pop(); }
    free(_nodes);
    _capacity = 0;
}

template <typename T>
T* ResourcePool<T>::allocate() {
    if(_freeNodes.empty()) { expand(); }

    T* t = _freeNodes.top();
    _freeNodes.pop();

    return t;
}

template <typename T>
void ResourcePool<T>::free(T* t) {
    //memset(t, 0, sizeof(T));
    _freeNodes.push(t);
}

template <typename T>
void ResourcePool<T>::reallocate(const unsigned int capacity) {
    ASSERT(capacity > _capacity);

    _nodes = (T*)realloc((void*)_nodes, sizeof(T) * capacity);
    //memset(&_nodes[capacity], 0, sizeof(T) * (capacity - _capacity));
    for(unsigned int i=_capacity; i<capacity; i++) {
        _freeNodes.push(&_nodes[i]);
    }
    _capacity = capacity;
}

template <typename T>
void ResourcePool<T>::expand() {
    // Double the ResourcePool in size
    reallocate(_capacity << 1);
}

#endif
