#ifndef INDEXPOOL_H
#define INDEXPOOL_H

#include <Base/Base.h>

class IndexPool {
public:
    IndexPool(int size);

    int allocate();
    void free(int index);

private:
    std::stack<int> _freeIndices;
    std::vector<bool> _indexState;
    int _size;
};

#endif