#include <Base/Base.h>
#include <Base/Debug.h>
#include <Base/Log.h>
#include <Base/Assertion.h>

void CheckHeap() {
#if SYS_PLATFORM == PLATFORM_WIN32
    int heapStatus = _heapchk();

    switch(heapStatus) {
    case _HEAPBADBEGIN:
        Error("Bad start of heap");
        break;
    case _HEAPBADNODE:
        Error("Bad node in heap");
        break;
    };

    if(heapStatus != _HEAPOK && heapStatus != _HEAPEMPTY) {
        Error("Heap check failed with errno " << errno);
        ASSERT(0);
    }
#endif
}
