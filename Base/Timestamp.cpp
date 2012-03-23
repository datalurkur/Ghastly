#include <Base/Timestamp.h>

time_t GetTimestamp() {
	return time(NULL);
}

clock_t GetClock() {
    return clock();
}
