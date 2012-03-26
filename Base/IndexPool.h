#ifndef INDEXPOOL_H
#define INDEXPOOL_H

#define DEBUG_INDEXPOOL 1

class IndexPool {
public:
	IndexPool(unsigned int size);

	unsigned int allocate();
	void free(unsigned int index);

private:
	std::stack<unsigned int> _freeIndices;
	std::vector<bool> _indexState;

	unsigned int _size;
};

#endif