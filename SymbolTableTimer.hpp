#ifndef _SYMBOL_TABLE_TIMER_HPP
#define _SYMBOL_TABLE_TIMER_HPP 1

#include "AbstractTimer.hpp"
#include "SymbolTable.hpp"

template <typename Key, typename Value>
class SymbolTableTimer : public AbstractTimer
{
protected:
    SymbolTable<Key, Value>* _st;
    const long LOWER_SIZE = 64;
    const long UPPER_SIZE = pow(2, 24);

public:
    SymbolTableTimer(SymbolTable<Key, Value>* table = nullptr,
        const std::string& name = "Symbol Table",
        std::ostream& out = std::cout) :
        AbstractTimer{ name, out }, _st{ table }
    {
    }

    virtual ~SymbolTableTimer()
    {
        _st->clear();
    }

    virtual void timeAll()
    {
        _os << "Timing " << _name << "..." << std::endl;

        ///////////////////////
        start();     // Overall
        ///////////////////////

        timePut();
        timeRemove();
        //timeContains();
        //timeDeleteMin();
        //timeDeleteMax();

        ///////////////////////
        stop();     // Overall
        ///////////////////////

    }

private:
    void timePut()
    {
        _os << "\t" << "Time put...";

        for (long size = LOWER_SIZE; size <= UPPER_SIZE; size *= 2)
        {
            _os << " " << size;

            _st->clear();
            _names.push_back("put " + std::to_string(size));

            std::vector<int> values = Utilities::rPopulate(0, size);

            ////////////////////////////////
            startInterval();
            ////////////////////////////////

            for (auto value : values)
            {
                _st->put(value, size + value);
            }

            ////////////////////////////////
            stopInterval();
            ////////////////////////////////

            _st->clear();
        }

        _os << std::endl;
    }

    void timeRemove()
    {
        _os << "\t" << "Time remove...";

        for (long size = LOWER_SIZE; size <= UPPER_SIZE; size *= 2)
        {
            _os << " " << size;

            _st->clear();
            _names.push_back("remove " + std::to_string(size));

            std::vector<int> values = Utilities::rPopulate(0, size);

            for (auto value : values)
            {
                _st->put(value, size + value);
            }

            ////////////////////////////////
            startInterval();
            ////////////////////////////////

            for (auto value : values)
            {
                _st->remove(value);
            }

            ////////////////////////////////
            stopInterval();
            ////////////////////////////////

            _st->clear();
        }

        _os << std::endl;
    }

    //void timeContains()
    //{
    //    _os << "\t" << "Time contains...";

    //    for (long size = LOWER_SIZE; size <= UPPER_SIZE; size *= 2)
    //    {
    //        _os << " " << size;

    //        _st->clear();
    //        _names.push_back("contains " + std::to_string(size));

    //        std::vector<int> values = Utilities::rPopulate(0, size);

    //        ////////////////////////////////
    //        startInterval();
    //        ////////////////////////////////

    //        for (auto value : values)
    //        {
    //            _st->contains(value);
    //        }

    //        ////////////////////////////////
    //        stopInterval();
    //        ////////////////////////////////

    //        _st->clear();
    //    }

    //    _os << std::endl;
    //}

    //void timeDeleteMin()
    //{
    //    _os << "\t" << "Time deleteMin...";

    //    for (long size = LOWER_SIZE; size <= UPPER_SIZE; size *= 2)
    //    {
    //        _os << " " << size;

    //        _st->clear();
    //        _names.push_back("deleteMin " + std::to_string(size));

    //        std::vector<int> values = Utilities::rPopulate(0, size);

    //        ////////////////////////////////
    //        startInterval();
    //        ////////////////////////////////

    //        for (auto value : values)
    //        {
    //            _st->deleteMin();
    //        }

    //        ////////////////////////////////
    //        stopInterval();
    //        ////////////////////////////////

    //        _st->clear();
    //    }

    //    _os << std::endl;
    //}

    //void timeDeleteMax()
    //{
    //    _os << "\t" << "Time deleteMax...";

    //    for (long size = LOWER_SIZE; size <= UPPER_SIZE; size *= 2)
    //    {
    //        _os << " " << size;

    //        _st->clear();
    //        _names.push_back("deleteMax " + std::to_string(size));

    //        std::vector<int> values = Utilities::rPopulate(0, size);

    //        ////////////////////////////////
    //        startInterval();
    //        ////////////////////////////////

    //        for (auto value : values)
    //        {
    //            _st->deleteMax();
    //        }

    //        ////////////////////////////////
    //        stopInterval();
    //        ////////////////////////////////

    //        _st->clear();
    //    }

    //    _os << std::endl;
    //}
};

#endif