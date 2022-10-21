

#include "TimeCounter.h"
#include "Singleton.h"
void c_count(const char* name, double time) {
    TimeCounter *counter = Singleton<TimeCounter>::instance();
    counter->count(name, time);
}
