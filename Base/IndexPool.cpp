#include <Base/IndexPool.h>

IndexPool::IndexPool(unsigned int size): _size(size) {
	unsigned int c;
	
	_indexState.resize(_size);
	for(c = (_size-1); c >= 0; c--) {
		_freeIndices.push(c);
		_indexState[c] = 0;
	}
}

unsigned int IndexPool::allocate() {
	unsigned int ret;
	ret = _freeIndices.top();
	_freeIndices.pop();
	return ret;
}

void IndexPool::free(unsigned int index) {
	_freeIndices.push(index);
}