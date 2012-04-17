#include <Base/IndexPool.h>
#include <Base/Assertion.h>

IndexPool::IndexPool(int size): _size(size) {
    int c;
    
    _indexState.resize(_size);
    for(c = (_size-1); c >= 0; c--) {
        _freeIndices.push(c);
        _indexState[c] = false;
    }
}

int IndexPool::allocate() {
    int index;

    if(_freeIndices.empty()) { return -1; }

    index = _freeIndices.top();
    _freeIndices.pop();

    ASSERT(!_indexState[index]);
    _indexState[index] = true;

    return index;
}

void IndexPool::free(int index) {
    ASSERT(_indexState[index]);
    _indexState[index] = false;
    _freeIndices.push(index);
}