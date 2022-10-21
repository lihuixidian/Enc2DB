
#include "Timestamp.h"

#include <chrono>
#include <time.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS



static_assert(sizeof(Timestamp) == sizeof(int64_t));

Timestamp::Timestamp(int64_t microseconds)
    : _microSecondsPassed(microseconds)
{
}

string Timestamp::toString() const
{
    char buf[32] = {0};
    int64_t seconds = _microSecondsPassed / kMicroSecondsPerSecond;
    int64_t microseconds = _microSecondsPassed % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf) - 1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
}

string Timestamp::toFormattedString() const
{
    char buf[32] = {0};
    time_t seconds = static_cast<time_t>(_microSecondsPassed / kMicroSecondsPerSecond);
    int microseconds = static_cast<int>(_microSecondsPassed % kMicroSecondsPerSecond);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);

    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
                tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
                microseconds);
    return buf;
}

Timestamp Timestamp::now()
{
    using namespace std;
    using namespace chrono;
    time_point<system_clock> tp(system_clock::now());
    int64_t ms = std::chrono::duration_cast<microseconds>(tp.time_since_epoch()).count();
    return Timestamp(ms);
    // struct timeval tv;
    // gettimeofday(&tv, NULL);
    // int64_t seconds = tv.tv_sec;
    // return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

Timestamp Timestamp::invalid()
{
    return Timestamp();
}

