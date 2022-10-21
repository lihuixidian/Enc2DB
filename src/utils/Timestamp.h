#pragma once

#include <stdint.h>
#include <string>
#include <time.h>
#include <chrono>

using namespace std;

template <class T>
struct less_than_comparable
{
    friend bool operator>(const T &x, const T &y) { return y < x; }
    friend bool operator<=(const T &x, const T &y) { return !static_cast<bool>(y < x); }
    friend bool operator>=(const T &x, const T &y) { return !static_cast<bool>(x < y); }
};



///
/// Time stamp in UTC, in microseconds resolution.
///
/// This class is immutable.
/// It's recommended to pass it by value, since it's passed in register on x64.
///
class Timestamp : public less_than_comparable<Timestamp>
{
public:
    ///
    /// Constucts an invalid Timestamp.
    ///
    Timestamp()
        : _microSecondsPassed(0)
    {
    }

    ///
    /// Constucts a Timestamp at specific time
    ///
    /// @param microSecondsPassed
    explicit Timestamp(int64_t microSecondsPassed);

    void swap(Timestamp &that)
    {
        std::swap(_microSecondsPassed, that._microSecondsPassed);
    }

    // default copy/assignment/dtor are Okay

    string toString() const;
    string toFormattedString() const;

    bool valid() const { return _microSecondsPassed > 0; }

    // for internal usage.
    int64_t microSecondsPassed() const { return _microSecondsPassed; }
    time_t secondsPassed() const
    {
        return static_cast<time_t>(_microSecondsPassed / kMicroSecondsPerSecond);
    }

    ///
    /// Get time of now.
    ///
    static Timestamp now();
    static Timestamp invalid();

    static constexpr int kMicroSecondsPerSecond = 1000 * 1000;

private:
    int64_t _microSecondsPassed;
};

inline bool operator<(const Timestamp &lhs, const Timestamp &rhs)
{
    return lhs.microSecondsPassed() < rhs.microSecondsPassed();
}

inline bool operator==(const Timestamp &lhs, const Timestamp &rhs)
{
    return lhs.microSecondsPassed() == rhs.microSecondsPassed();
}

///
/// Gets time difference of two timestamps, result in seconds.
///
/// @param high, low
/// @return (high-low) in seconds
/// @c double has 52-bit precision, enough for one-microseciond
/// resolution for next 100 years.
inline double timeDifference(const Timestamp &high, const Timestamp &low)
{
    int64_t diff = high.microSecondsPassed() - low.microSecondsPassed();
    return static_cast<double>(diff);
}

///
/// Add @c seconds to given timestamp.
///
/// @return timestamp+seconds as Timestamp
///
inline Timestamp addTime(Timestamp timestamp, double seconds)
{
    int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
    return Timestamp(timestamp.microSecondsPassed() + delta);
}

