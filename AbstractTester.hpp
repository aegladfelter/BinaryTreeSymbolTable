#ifndef _ABSTRACT_TESTER_H
#define _ABSTRACT_TESTER_H 1

#include <iostream>
#include <algorithm>
#include <map>
#include <random>
#include <string>

class AbstractTester
{
protected:
    std::map<std::string, int>         _errorCountMap;
    std::string                        _name;
    std::ostream&                      _os;

public:
    AbstractTester(const std::string& name = "", std::ostream& out = std::cout)
        : _errorCountMap{}, _name{ name }, _os{ out }
    {
    }

    virtual ~AbstractTester()
    {
        _errorCountMap.clear();
    }


    // Pure virtual "run every test" function
    virtual void runAll() = 0;

    //
    // Report results of the test run
    //
    virtual void report()
    {
        _os << _name << " Error Report: " << std::endl;

        int sum = 0;
        for (auto const& entry : _errorCountMap)
        {
            _os << "\tTest " << entry.first << ": " << entry.second << " errors." << std::endl;
            sum += entry.second;
        }

        _os << "\tTotal number of checks: " << _checkCount << std::endl;
        _os << "\tTotal number of errors: " << sum << std::endl;
    }

    virtual void emitError(const std::string& error) const
    {
        _os << error << std::endl;
    }

    virtual void emitError(const std::string& error, const std::string& acquired, const std::string& expected) const
    {
        emitError(error + ": acquired (" + acquired + "); expected (" + expected + ")");
    }

    virtual void emitError(const std::string& error, int value) const
    {
        emitError(error + " with value " + std::to_string(value));
    }

    virtual void emitError(const std::string& error, int acquired, int expected) const
    {
        emitError(error, std::to_string(acquired), std::to_string(expected));
    }

protected:
    static bool doubleEquals(double a, double b)
    {
        return (a - b) < 0.0001;
    }

    mutable unsigned _checkCount = 0;
    virtual void check() const { _checkCount++; }
};

#endif