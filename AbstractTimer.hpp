#ifndef _ABSTRACT_TIMER_HPP
#define _ABSTRACT_TIMER_HPP 1

#include <string>
#include <vector>
#include <ctime>

class AbstractTimer
{
  protected:
    std::string              _name;           // Name of this timer
    clock_t                  _start;          // Overall start time
    clock_t                  _stop;           // Overall stop time
    clock_t                  _intervalStart;  // Strictly an interval start time
    clock_t                  _intervalStop;   // Strictly an interval stop time
    std::vector<std::string> _names;          // Times for all intervals
    std::vector<clock_t>     _times;          // Times for all intervals
    std::ostream&            _os;             // Output stream

public:
    AbstractTimer(const std::string& name = "", std::ostream& out = std::cout) :
        _name{ name },
        _start{ 0 },
        _stop{ 0 },
        _intervalStart{ 0 },
        _intervalStop{ 0 },
        _names{},
        _times{},
        _os {out}
    {}

    virtual ~AbstractTimer() { _times.clear(); }

    // The abstract function for which the implementing, inheriting class must define
    virtual void timeAll() = 0;

    // Starts the (overall) timer
    virtual void start()
    {
        _start = clock();
    }

    // (1) Stop the overall timer
    // (2) Resets the start time
    // @return the accumulated time
    virtual clock_t stop()
    {
        // Compute accumulated time
        _stop = clock();
        clock_t accumulated = _stop - _start;

        return accumulated;
    }

    // Starts the (interval) timer
    virtual void startInterval()
    {
        _intervalStart = clock();
    }

    // (1) Stop the interval timer
    // (2) Resets the start interval time
    // (3) Records the interval time
    // @return the interval time
    virtual clock_t stopInterval()
    {
        // Compute accumulated time
        clock_t stop = clock();
        clock_t interval = stop - _intervalStart;

        // Reset the (interval) start time
        _intervalStart = clock();

        // Record the interval time
        _times.push_back(interval);

        return interval;
    }

    // Compute and @return the summary interval assumulated time 
    virtual clock_t accumulatedIntervalTime()
    {
        clock_t total = 0;

        for (clock_t time : _times)
        {
            total += time;
        }

        return total;
    }

    //
    // Report results of the test run
    //
    virtual void report()
    {
        _os << _name << " Timing Report: " << std::endl;

        for (unsigned index = 0; index < _names.size(); index++)
        {
            _os << "\tOperation: |" << _names[index] << "|\t" << _times[index] << "; " << _times[index] / (double)CLOCKS_PER_SEC << "s." << std::endl;
        }

        _os << "\tSum of operation times: " << accumulatedIntervalTime() / (double)CLOCKS_PER_SEC << "s." << std::endl;
        _os << "\tTotal time: " << (_stop - _start) / (double)CLOCKS_PER_SEC << "s." << std::endl;
    }
};

#endif