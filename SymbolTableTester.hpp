#ifndef _SYMBOL_TABLE_TESTER_H
#define _SYMBOL_TABLE_TESTER_H 1

#include <map>
#include <vector>
#include <iostream>

#include "AbstractTester.hpp"
#include "SymbolTable.hpp"
#include "Utilities.h"

template <typename Key, typename Value>
class SymbolTableTester : public AbstractTester
{
protected:
    SymbolTable<Key, Value>* _st;

public:
    SymbolTableTester(SymbolTable<Key, Value>* table, const std::string& name = "Symbol Table", std::ostream& out = std::cout) :
        AbstractTester{ name, out }, _st{ table }
    {
    }

    virtual ~SymbolTableTester()
    {
        _st->clear();
    }

    virtual void runAll()
    {
        //
        // Run tests and report
        //
        _st->clear();

        _errorCountMap["constructor"] = testConstructor();
        _st->clear();

        _errorCountMap["put"] = testPut();
        _st->clear();

        _errorCountMap["get"] = testGet();
        _st->clear();

        _errorCountMap["remove"] = testRemove();
        _st->clear();

        _errorCountMap["min"] = testMin();
        _st->clear();

        _errorCountMap["max"] = testMax();
        _st->clear();

        _errorCountMap["floor"] = testFloor();
        _st->clear();

        _errorCountMap["ceiling"] = testCeiling();
        _st->clear();

        _errorCountMap["rank"] = testRank();
        _st->clear();

        _errorCountMap["select"] = testSelect();
        _st->clear();

        _errorCountMap["deleteMin"] = testDeleteMin();
        _st->clear();

        _errorCountMap["deleteMax"] = testDeleteMax();
        _st->clear();

        _errorCountMap["size"] = testSize();
        _st->clear();

        _errorCountMap["keys"] = testSelectiveKeys();
        _st->clear();

        _errorCountMap["allKeys"] = testAllKeys();
        _st->clear();
    }

    virtual int testConstructor()
    {
        _os << "\t" << "Test constructor..." << std::endl;

        int errors = 0;

        if (_st->size() != 0)
        {
            emitError("Constructor error; size not zero");
            errors++;
        }
        if (!_st->empty())
        {
            emitError("Constructor error; stack not empty");
            errors++;
        }
        check(_st, errors);

        _st->remove(Utilities::generate());

        if (_st->contains(Utilities::generate()))
        {
            emitError("Constructor error; contains returned true");
            errors++;
        }
        int key = Utilities::generate();
        int value = -1;
        if (_st->get(key, value))
        {
            emitError("Constructor error; get returned true");
            errors++;
        }
        if (_st->deleteMin())
        {
            emitError("Constructor error; deleteMin returned true");
            errors++;
        }
        if (_st->deleteMax())
        {
            emitError("Constructor error; deleteMax returned true");
            errors++;
        }
        if (!_st->keys().empty())
        {
            emitError("Constructor error; keys() returned non-empty list");
            errors++;
        }

        return errors;
    }

    virtual int testPut()
    {
        _os << "\t" << "Test put..." << std::endl;

        int errors = 0;

        //
        // Mapping [0, 10) to [100, 110) ORDERED
        //
        const int DELTA = 100;
        for (int k = 0; k < 10; k++)
        {
            if (_st->size() != k)
            {
                emitError("Put error (before): size", _st->size(), k);
                errors++;
            }

            ///////////////////
            _st->put(k, DELTA + k);
            ///////////////////

            if (_st->empty())
            {
                emitError("Put error: empty; symbol table empty");
                errors++;
            }
            if (_st->size() != k + 1)
            {
                emitError("Put error (after): size", _st->size(), k + 1);
                errors++;
            }
            if (!_st->contains(k))
            {
                emitError("Put error: contains returned false for element contained", k);
                errors++;
            }
            if (_st->contains(DELTA + k))
            {
                emitError("Put error: contains returned true for element not contained", DELTA + k);
                errors++;
            }
        }

        // next test
        _st->clear();

        //
        // Mapping [0, 10) to [100, 110) SHUFFLED
        //
        std::vector<int> keys = Utilities::rPopulate(0, 10);
        for (unsigned index = 0; index < keys.size(); index++)
        {
            if (_st->size() != index)
            {
                emitError("Shuffled put error (before): size", _st->size(), index);
                errors++;
            }

            ///////////////////
            _st->put(keys[index], DELTA + keys[index]);
            ///////////////////

            if (_st->empty())
            {
                emitError("Shuffled put error: empty; symbol table empty");
                errors++;
            }
            if (_st->size() != index + 1)
            {
                emitError("Shuffled put error (after): size", _st->size(), index + 1);
                errors++;
            }
            if (!_st->contains(keys[index]))
            {
                emitError("Shuffled put error: contains returned false for element contained", keys[index]);
                errors++;
            }
            if (_st->contains(DELTA + keys[index]))
            {
                emitError("Shuffled put error: contains returned true for element not contained", DELTA + keys[index]);
                errors++;
            }
        }

        return errors;
    }

    virtual int testGet()
    {
        _os << "\t" << "Test get..." << std::endl;

        int errors = 0;

        //
        // Mapping [0, 100) to [1000, 1100) ORDERED
        //
        const int DELTA = 1000;
        for (int k = 0; k < 100; k++)
        {
            ///////////////////
            _st->put(k, DELTA + k);
            ///////////////////
        }

        //
        // Positive Containment
        //
        for (int k = 0; k < 100; k++)
        {
            int value = -1;
            if (!_st->get(k, value))
            {
                emitError("Get error: expected to acquire a <key, value> pair; failed", k);
                errors++;
            }
            if (value != DELTA + k)
            {
                emitError("Get error: unexpected <key, value> pair", value, DELTA + k);
                errors++;
            }
        }

        //
        // Negative Containment
        //
        for (int k = 1000; k < 1100; k++)
        {
            int value = -1;
            if (_st->get(k, value))
            {
                emitError("Get error: expected NOT to acquire a <key, value> pair; failed", k);
                errors++;
            }
            if (value != -1)
            {
                emitError("Get error: get of unknown key alters input value", value, -1);
                errors++;
            }
        }

        // next test
        _st->clear();

        //
        // Mapping [0, 100) to [1000, 1100) SHUFFLED
        //
        std::vector<int> keys = Utilities::rPopulate(0, 100);
        for (unsigned index = 0; index < keys.size(); index++)
        {
            _st->put(keys[index], DELTA + keys[index]);
        }

        //
        // Positive Containment
        //
        Utilities::shuffle(keys);
        for (int key : keys)
        {
            int value;
            if (!_st->get(key, value))
            {
                emitError("Get error: expected to acquire a <key, value> pair; failed");
                errors++;
            }
            if (value != DELTA + key)
            {
                emitError("Get error: unexpected <key, value> pair", value, DELTA + key);
                errors++;
            }
        }

        //
        // Negative Containment
        //
        std::vector<int> badKeys = Utilities::rPopulate(1000, 10000);
        for (int badKey : badKeys)
        {
            int value = -1;
            if (_st->get(badKey, value))
            {
                emitError("Get error: expected NOT to acquire a <key, value> pair; failed", badKey);
                errors++;
            }
            if (value != -1)
            {
                emitError("Get error: get of unknown key alters input value", value, -1);
                errors++;
            }
        }

        return errors;
    }

    virtual int testRemove()
    {
        _os << "\t" << "Test remove..." << std::endl;

        int errors = 0;

        //
        // Simple test: add, remove, add, remove, check
        //
        _st->put(1, 101);
        check(_st, errors);

        _st->remove(1);
        check(_st, errors);

        if (!_st->empty())
        {
            emitError("Remove error; empty table expected");
            errors++;
        }
        check(_st, errors);


        //
        // Manageable tests
        //
        errors += testRemoveIncreasing(0, 10);
        errors += testRemoveDecreasing(0, 10);
        errors += testShuffle(0, 10);

        // Next test
        _st->clear();

        //
        // Mapping [0, 100) to [1000, 1100) ORDERED
        //
        const int DELTA = 1000;
        for (int k = 0; k < 100; k++)
        {
            _st->put(k, DELTA + k);
        }
        check(_st, errors);

        //
        // Negative Containment
        //
        for (int k = DELTA; k < DELTA + 100; k++)
        {
            //
            // Remove
            //
            _st->remove(k);
            check(_st, errors);

            //
            // Container should remain unaltered
            //
            if (_st->size() != 100)
            {
                emitError("Remove error; failed remove changed the size of table", _st->size(), 100);
                errors++;
            }
            if (_st->empty())
            {
                emitError("Remove error; failed remove emptied the table...wowzer");
                errors++;
            }

            for (int local_k = 0; local_k < 100; local_k++)
            {
                if (!_st->contains(local_k))
                {
                    emitError("Remove error; failed remove altered what should be, existing keys", local_k);
                    errors++;
                    emitError("Remove error: breaking to avoid error message bloat");
                    break;
                }
                // check(_st, errors); only use for long verification
            }
        }

        //
        // Positive Containment; remove in reverse order
        //
        int externalSize = 100;
        for (int k = 99; k >= 0; k--)
        {
            if (_st->empty())
            {
                emitError("Shuffled remove error; successful remove emptied the table...wowzer");
                errors++;
            }

            //
            // Remove
            //
            _st->remove(k);
            externalSize--;

            if (_st->contains(k))
            {
                emitError("Remove error; successful remove did not actually remove the stated <key, value>", k);
                errors++;
            }

            for (int local_k = k - 1; local_k >= 0; local_k--)
            {
                if (!_st->contains(local_k))
                {
                    emitError("Remove error; successful remove removed more keys than expected", local_k);
                    errors++;
                    emitError("Remove error: breaking to avoid error message bloat");
                    break;
                }
            }

            check(_st, errors);

            int acqSize = _st->size();
            if (_st->size() != externalSize)
            {
                emitError("Shuffled remove error; size disagreement", acqSize, externalSize);
                errors++;
            }
        }

        // Next test
        _st->clear();

        //
        // Mapping [0, 100) to [1000, 1100) 
        //
        std::vector<int> keys = Utilities::rPopulate(0, 100);
        for (int k = 0; k < 100; k++)
        {
            _st->put(keys[k], DELTA + keys[k]);
        }
        check(_st, errors);

        //
        // Negative Containment
        //
        std::vector<int> badKeys = Utilities::populateRandomComplement(0, 100, 100);
        for (int badKey : badKeys)
        {
            //
            // Remove
            //
            _st->remove(badKey);

            //
            // Container should remain unaltered
            //
            if (_st->size() != 100)
            {
                emitError("Shuffled remove error; failed remove changed the size of table", _st->size(), 100);
                errors++;
            }
            if (_st->empty())
            {
                emitError("Shuffled remove error; failed remove emptied the table...wowzer");
                errors++;
            }

            for (int k = 0; k < 100; k++)
            {
                if (!_st->contains(k))
                {
                    emitError("Shuffled remove error; failed remove altered what should be, existing keys", k);
                    errors++;
                    emitError("Remove error: breaking to avoid error message bloat");
                    break;
                }
            }
            check(_st, errors);
        }


        //
        // Positive Containment
        //
        Utilities::shuffle(keys);
        externalSize = 100;
        for (int key : keys)
        {
            if (_st->empty())
            {
                emitError("Shuffled remove error; successful remove emptied the table...wowzer");
                errors++;
            }

            //
            // Remove
            //
            _st->remove(key);
            externalSize--;

            if (_st->contains(key))
            {
                emitError("Shuffled remove error; failed remove altered what should be, existing keys", key);
                errors++;
            }

            int acqSize = _st->size();
            if (_st->size() != externalSize)
            {
                emitError("Shuffled remove error; size disagreement", acqSize, externalSize);
                errors++;
            }
            check(_st, errors);
        }

        if (!_st->empty())
        {
            emitError("Shuffled remove error; expected an empty table");
            errors++;
        }

        return errors;
    }

    virtual int testMin()
    {
        _os << "\t" << "Test min..." << std::endl;

        int errors = 0;

        //
        // Mapping [0, 100) to [1000, 1100) 
        //
        std::vector<int> keys = Utilities::rPopulate(0, 100);
        int currentMin = keys[0];
        const int DELTA = 1000;
        for (int k = 0; k < 100; k++)
        {
            // Update the current min for this container (based on what has been added thus far)
            if (currentMin > keys[k])
            {
                currentMin = keys[k];
                //std::cout << "Min is now " << currentMin << std::endl;
            }

            _st->put(keys[k], DELTA + keys[k]);

            int minAcquired = -1;
            if (!_st->min(minAcquired))
            {
                emitError("min() error: call failed");
                errors++;
            }

            if (_st->size() != k + 1)
            {
                emitError("min() error: size", _st->size(), k + 1);
                errors++;
            }

            check(_st, errors);
        }

        return errors;
    }

    virtual int testMax()
    {
        _os << "\t" << "Test max..." << std::endl;

        int errors = 0;

        //
        // Mapping [0, 100) to [1000, 1100) 
        //
        std::vector<int> keys = Utilities::rPopulate(0, 100);
        int currentMax = keys[0];
        const int DELTA = 1000;
        for (int k = 0; k < 100; k++)
        {
            // Update the current min for this container (based on what has been added thus far)
            if (currentMax < keys[k])
            {
                currentMax = keys[k];
                //std::cout << "Max is now " << currentMax << std::endl;
            }

            _st->put(keys[k], DELTA + keys[k]);

            int maxAcquired = -1;
            if (!_st->max(maxAcquired))
            {
                emitError("max() error: call failed");
                errors++;
            }
            if (maxAcquired != currentMax)
            {
                emitError("max() error: ", maxAcquired, currentMax);
                errors++;
            }
            if (_st->size() != k + 1)
            {
                emitError("max() error: size", _st->size(), k + 1);
                errors++;
            }

            check(_st, errors);
        }

        return errors;
    }

    virtual int testFloor()
    {
        _os << "\t" << "Test floor..." << std::endl;

        int errors = 0;

        // floor on an empty table
        int floorAcquired = -1;
        if (_st->floor(12, floorAcquired))
        {
            emitError("floor() error: returned true on empty table");
            errors++;
        }
        if (floorAcquired != -1)
        {
            emitError("floor() error: failure overwrote input value");
            errors++;
        }

        //
        // Mapping [0, 100) to [1000, 1100) every 3rd key
        //
        const int DELTA = 1000;
        std::vector<int> keys;
        for (int k = 0; k < 100; k += 3)
        {
            _st->put(k, DELTA + k);
        }


        //
        // Negative tests
        //
        for (int k = -5; k < 0; k++)
        {
            floorAcquired = -1;
            if (_st->floor(k, floorAcquired))
            {
                emitError("floor() error: returned true non empty table with key smaller than all values", k);
                errors++;
            }
            if (floorAcquired != -1)
            {
                emitError("floor() error: failure (small key) overwrote input value");
                errors++;
            }
        }

        //
        // Positive tests
        //
        int currentFloor = 0;
        for (int k = 0; k < 100; k++)
        {
            // Update the current min for this container (based on what has been added thus far)
            if (k % 3 == 0)
            {
                currentFloor = k;
                //std::cout << "Floor is now " << currentFloor << std::endl;
            }

            floorAcquired = -1;
            if (!_st->floor(k, floorAcquired))
            {
                emitError("floor() error: returned false on full table");
                errors++;
            }
            if (floorAcquired != currentFloor)
            {
                emitError("floor() error:", floorAcquired, currentFloor);
                errors++;
            }
        }

        // Larger values than in the table
        for (int k = 100; k < 150; k++)
        {
            floorAcquired = -1;
            if (!_st->floor(k, floorAcquired))
            {
                emitError("floor() error: returned false (key larger) on full table");
                errors++;
            }
            if (floorAcquired != currentFloor)
            {
                emitError("floor() error:", floorAcquired, currentFloor);
                errors++;
            }
        }

        return errors;
    }

    virtual int testCeiling()
    {
        _os << "\t" << "Test ceiling..." << std::endl;

        int errors = 0;

        // floor on an empty table
        int ceilingAcquired = -1;
        if (_st->ceiling(12, ceilingAcquired))
        {
            emitError("floor() error: returned true on empty table");
            errors++;
        }
        if (ceilingAcquired != -1)
        {
            emitError("ceiling() error: failure overwrote input value");
            errors++;
        }

        //
        // Mapping [0, 100) to [1000, 1100) every 3rd key
        //
        const int DELTA = 1000;
        for (int k = 0; k < 100; k += 3)
        {
            _st->put(k, DELTA + k);
        }

        //
        // Negative tests
        //
        for (int k = 100; k < 105; k++)
        {
            ceilingAcquired = -1;
            if (_st->ceiling(k, ceilingAcquired))
            {
                emitError("ceiling() error: returned true non empty table with key larger than all values", k);
                errors++;
            }
            if (ceilingAcquired != -1)
            {
                emitError("ceiling() error: failure (large key) overwrote input value");
                errors++;
            }
        }

        //
        // Positive tests
        //
        int currentceiling = 0;
        for (int k = 0; k < 100; k++)
        {
            // Update the current min for this container (based on what has been added thus far)
            if (k % 3 == 1)
            {
                currentceiling = k + 2;
                //std::cout << "ceiling is now " << currentceiling << std::endl;
            }

            ceilingAcquired = -1;
            if (!_st->ceiling(k, ceilingAcquired))
            {
                emitError("ceiling() error: returned false on full table");
                errors++;
            }
            if (ceilingAcquired != currentceiling)
            {
                emitError("ceiling() error:", ceilingAcquired, currentceiling);
                errors++;
            }
        }

        // Smaller values than in the table
        for (int k = -50; k < 0; k++)
        {
            ceilingAcquired = -1;
            if (!_st->ceiling(k, ceilingAcquired))
            {
                emitError("ceiling() error: returned false (key larger) on full table");
                errors++;
            }
            if (ceilingAcquired != 0)
            {
                emitError("ceiling() error:", ceilingAcquired, 0);
                errors++;
            }
        }

        return errors;
    }

    virtual int testRank()
    {
        _os << "\t" << "Test rank..." << std::endl;

        int errors = 0;

        //
        // Empty
        //
        for (int k = 100; k < 105; k++)
        {
            if (_st->rank(k) != 0)
            {
                emitError("rank() error: empty return non-zero rank");
                errors++;
            }
        }

        //
        // Non-Empty
        //
        // Mapping [0, 100) to [1000, 1100) every 3rd key
        //
        const int DELTA = 1000;
        std::vector<int> keys;
        for (int k = 0; k < 100; k += 3)
        {
            _st->put(k, DELTA + k);
            keys.push_back(k);
        }

        //
        // Ends
        //
        if (_st->rank(-1) != 0)
        {
            emitError("rank() error: non-empty return non-zero rank");
            errors++;
        }
        if (_st->rank(105) != keys.size())
        {
            emitError("rank() error: non-empty", _st->rank(105), keys.size());
            errors++;
        }

        //
        // Inmiddle of 0, 3, 6, 9, ..., 99
        //
        int externalRank = 0;
        for (int k = 0; k < 101; k++)
        {
            if (k % 3 == 1) externalRank++;

            int rank = _st->rank(k);
            if (rank != externalRank)
            {
                emitError("rank() error: non-empty return non-zero rank", rank, externalRank);
                errors++;
            }
        }

        return errors;
    }

    virtual int testSelect()
    {
        _os << "\t" << "Test select..." << std::endl;

        int errors = 0;

        const int DELTA = 1000;
        std::vector<int> keys = Utilities::populateRandomComplement(-1000, 0, 15);
        for (int key : keys)
        {
            _st->put(key, key + DELTA);
        }

        std::sort(keys.begin(), keys.end());
        for (unsigned index = 0; index < keys.size(); index++)
        {
            int acquiredKey = -1;
            if (!_st->select(index, acquiredKey))
            {
                emitError("select() error: select return false");
                errors++;
            }
            if (acquiredKey != keys[index])
            {
                emitError("select() error: select", acquiredKey, keys[index]);
                errors++;
            }
        }


        // CTA: Need test for selecting (in)valid values: < 0, >= size == size

        return errors;
    }

    virtual int testDeleteMin()
    {
        _os << "\t" << "Test deleteMin()..." << std::endl;

        int errors = 0;

        //
        // Empty
        //
        if (_st->deleteMin())
        {
            emitError("deleteMin() error: empty deletion");
            errors++;
        }

        //
        // Filled
        //
        const int DELTA = 1000;
        std::vector<int> keys = Utilities::populateRandomComplement(-1000, 0, 15);
        for (int key : keys)
        {
            _st->put(key, DELTA + key);
        }

        std::sort(keys.begin(), keys.end());
        for (unsigned index = 0; index < keys.size(); index++)
        {
            if (!_st->deleteMin())
            {
                emitError("deleteMin() error: non-empty");
                errors++;
            }
            if (_st->size() != keys.size() - (index + 1))
            {
                emitError("deleteMin() error: size", _st->size(), keys.size() - (index + 1));
                errors++;
            }
            if (_st->contains(keys[index]))
            {
                emitError("deleteMin() error: did not remove smallest elements");
                errors++;
            }

            check(_st, errors);
        }

        //
        // Empty
        //
        if (_st->deleteMin())
        {
            emitError("deleteMin() error: empty deletion");
            errors++;
        }

        return errors;
    }

    virtual int testDeleteMax()
    {
        _os << "\t" << "Test deleteMax()..." << std::endl;

        int errors = 0;

        //
        // Empty
        //
        if (_st->deleteMax())
        {
            emitError("deleteMax() error: empty deletion");
            errors++;
        }

        //
        // Filled
        //
        const int DELTA = 1000;
        std::vector<int> keys = Utilities::populateRandomComplement(-1000, 0, 15);
        for (int key : keys)
        {
            _st->put(key, DELTA + key);
        }

        std::sort(keys.begin(), keys.end());
        for (int index = keys.size() - 1; index >= 0; index--)
        {
            if (!_st->deleteMax())
            {
                emitError("deleteMax() error: non-empty");
                errors++;
            }
            if (_st->size() != index)
            {
                emitError("deleteMax() error: size", _st->size(), index);
                errors++;
            }
            if (_st->contains(keys[index]))
            {
                emitError("deleteMax() error: did not remove largest elements");
                errors++;
            }
            check(_st, errors);
        }

        //
        // Empty
        //
        if (_st->deleteMax())
        {
            emitError("deleteMax() error: empty deletion");
            errors++;
        }

        return errors;
    }

    virtual int testSize()
    {
        _os << "\t" << "Test size(int, int)..." << std::endl;

        int errors = 0;

        //
        // Empty
        //
        for (int i = 0; i < 5; i++)
        {
            int left = Utilities::generate();
            int right = Utilities::generate();
            int sz = _st->size(left, right);
            if (sz != 0)
            {
                emitError("size(int, int) error: empty returns non-zero size");
                errors++;
            }

            // Same bounds test
            right = left;
            sz = _st->size(left, right);
            if (sz != 0)
            {
                emitError("size(int, int) error: empty returns non-zero size");
                errors++;
            }
        }

        //
        // Filled
        //
        const int DELTA = 1000;
        std::vector<int> keys;
        for (unsigned key = 0; key < 100; key += 2)
        {
            keys.push_back(key);
            _st->put(key, DELTA + key);
        }

        //
        // None in interval to left of values
        //
        int left = -10;
        int right = -5;
        int sz = _st->size(left, right);
        if (sz != 0)
        {
            emitError("size(int, int) error: filled returns non-zero size (left)", sz, 0);
            errors++;
        }

        //
        // None in interval to left of values
        //
        left = 110;
        right = 115;
        sz = _st->size(left, right);
        if (sz != 0)
        {
            emitError("size(int, int) error: filled returns non-zero size (right)", sz, 0);
            errors++;
        }

        //
        // Singletons
        //
        for (int key : keys)
        {
            sz = _st->size(key, key);
            if (sz != 1)
            {
                emitError("size(int, int) error: filled returns non-zero size (right)", sz, 1);
                errors++;
            }
        }
        for (int key : keys)
        {
            sz = _st->size(key + 1, key + 1);
            if (sz != 0)
            {
                emitError("size(int, int) error: filled returns non-zero size (right)", sz, 0);
                errors++;
            }

            sz = _st->size(key - 1, key + 1);
            if (sz != 1)
            {
                emitError("size(int, int) error: filled returns non-zero size (right)", sz, 1);
                errors++;
            }
        }

        //
        // InMiddle
        //
        int currentSize = keys.size();
        std::vector<int>::const_iterator left_it = keys.begin();
        std::vector<int>::const_iterator right_it = --keys.end();
        while (*left_it < *right_it)
        {
            sz = _st->size(*left_it, *right_it);
            if (sz != currentSize)
            {
                emitError("size(int, int) error: found endpoints", sz, currentSize);
                errors++;
            }
            currentSize -= 2;
            left_it++;
            right_it--;
        }

        currentSize = keys.size();
        left_it = keys.begin();
        right_it = --keys.end();
        while (*left_it < *right_it)
        {
            sz = _st->size(*left_it - 1, *right_it + 1);
            if (sz != currentSize)
            {
                emitError("size(int, int) error: non-found endpoints", sz, currentSize);
                errors++;
            }
            currentSize -= 2;
            left_it++;
            right_it--;
        }

        // Left non-bound, right bound on
        currentSize = keys.size();
        left_it = keys.begin();
        right_it = --keys.end();
        while (*left_it < *right_it)
        {
            sz = _st->size(*left_it - 1, *right_it);
            if (sz != currentSize)
            {
                emitError("size(int, int) error: non-found endpoints", sz, currentSize);
                errors++;
            }
            currentSize -= 2;
            left_it++;
            right_it--;
        }

        // Left bound on, right non-bound
        currentSize = keys.size();
        left_it = keys.begin();
        right_it = --keys.end();
        while (*left_it < *right_it)
        {
            sz = _st->size(*left_it, *right_it + 1);
            if (sz != currentSize)
            {
                emitError("size(int, int) error: non-found endpoints", sz, currentSize);
                errors++;
            }
            currentSize -= 2;
            left_it++;
            right_it--;
        }

        //
        // Large middle gap
        //
        _st->put(150, 1150);
        left = 102;
        right = 148;
        while (left < right)
        {
            sz = _st->size(left, right);
            if (sz != 0)
            {
                emitError("size(int, int) error: filled returns non-zero size (right)", sz, 0);
                errors++;
            }
            left++;
            right--;
        }

        return errors;
    }

    virtual int testSelectiveKeys()
    {
        _os << "\t" << "Test selectKeys()..." << std::endl;

        int errors = 0;

        //
        // Empty
        //
        if (!_st->keys().empty())
        {
            emitError("keys() error: non-empty key set");
            errors++;
        }

        //
        // Filled
        //
        const int DELTA = 1000;
        std::vector<int> keys = Utilities::populateRandomComplement(-1000, 0, 15);
        for (int key : keys)
        {
            _st->put(key, DELTA + key);
        }

        std::sort(keys.begin(), keys.end());
        for (unsigned index = 0; index < keys.size(); index++)
        {
            Key key;
            if (!_st->select(index, key))
            {
                emitError("select(index) error: returned false");
                errors++;
                emitError("break for brevity");
                break;
            }
            if (keys[index] != key)
            {
                emitError("select(index) error: unexpected key values", key, keys[index]);
                errors++;
                emitError("break for brevity");
                break;
            }
        }

        //
        // Empty
        //
        _st->clear();
        if (!_st->keys().empty())
        {
            emitError("keys() error: non-empty key set");
            errors++;
        }

        return errors;
    }

    virtual int testAllKeys()
    {
        _os << "\t" << "Test allKeys()..." << std::endl;

        int errors = 0;

        //
        // Empty
        //
        if (!_st->keys().empty())
        {
            emitError("keys() error: non-empty key set");
            errors++;
        }

        //
        // Filled
        //
        const int DELTA = 1000;
        std::vector<int> keys = Utilities::populateRandomComplement(-1000, 0, 15);
        for (int key : keys)
        {
            _st->put(key, DELTA + key);
        }

        std::sort(keys.begin(), keys.end());
        std::vector<int> st_keys = _st->keys();

        if (keys.size() != st_keys.size())
        {
            emitError("keys() error: key sizes differ");
            errors++;
        }
        for (unsigned index = 0; index < keys.size(); index++)
        {
            if (keys[index] != st_keys[index])
            {
                emitError("keys() error: unexpected key values", st_keys[index], keys[index]);
                errors++;
                emitError("break for brevity");
                break;
            }
        }

        //
        // Empty
        //
        _st->clear();
        if (!_st->keys().empty())
        {
            emitError("keys() error: non-empty key set");
            errors++;
        }

        return errors;
    }

    virtual void check(const SymbolTable<int, int>* s, int& errors) const
    {
        AbstractTester::check();

        if (!s->check())
        {
            emitError("Verification check failed");
            errors++;
        }
        return;
    }

private:
    //
    // Mapping [low, high) to [1000 + low, 1000 + high) ORDERED: Small to large
    //
    int testRemoveIncreasing(int low, int high)
    {
        int errors = 0;
        //
        ////////////////////////////////////////////////////
        // Test 1:               Remove in same order
        ////////////////////////////////////////////////////
        //
        _st->clear(); // Prepare the test

        //
        // Add all elements from low to high: linked list
        //
        const int DELTA = 1000;
        for (int k = low; k < high; k++)
        {
            _st->put(k, DELTA + k);
        }
        check(_st, errors);

        //
        // Positive Containment; remove in same order
        //
        int externalSize = 10;
        for (int k = low; k < high; k++)
        {
            ////////////////
            _st->remove(k);
            externalSize--;
            ////////////////

            if (_st->contains(k))
            {
                emitError("Remove error; successful remove did not actually remove the stated <key, value>", k);
                errors++;
            }

            for (int local_k = k + 1; local_k < high; local_k++)
            {
                if (!_st->contains(local_k))
                {
                    emitError("Remove error; successful remove removed more keys than expected", local_k);
                    errors++;
                    emitError("Remove error: breaking to avoid error message bloat");
                    break;
                }
            }

            int acqSize = _st->size();
            if (acqSize != externalSize)
            {
                emitError("Shuffled remove error; size disagreement", acqSize, externalSize);
                errors++;
            }
            check(_st, errors);
        }

        //
        ////////////////////////////////////////////////////
        // Test 2:               Remove in reverse order
        ////////////////////////////////////////////////////
        //
        _st->clear(); // Prepare the test

        //
        // Add all elements from low to high: linked list
        //
        for (int k = low; k < high; k++)
        {
            _st->put(k, DELTA + k);
        }
        check(_st, errors);

        //
        // Positive Containment; remove in reverse order
        //
        externalSize = 10;
        for (int k = high - 1; k >= low; k--)
        {
            ////////////////
            _st->remove(k);
            externalSize--;
            ////////////////

            if (_st->contains(k))
            {
                emitError("Remove error; successful remove did not actually remove the stated <key, value>", k);
                errors++;
            }

            for (int local_k = k - 1; local_k >= low; local_k--)
            {
                if (!_st->contains(local_k))
                {
                    emitError("Remove error; successful remove removed more keys than expected", local_k);
                    errors++;
                    emitError("Remove error: breaking to avoid error message bloat");
                    break;
                }
            }

            int acqSize = _st->size();
            if (acqSize != externalSize)
            {
                emitError("Shuffled remove error; size disagreement", acqSize, externalSize);
                errors++;
            }
            check(_st, errors);
        }

        return errors;
    }

    //
    // Mapping [low, high) to [1000 + low, 1000 + high) ORDERED: Small to large
    //
    int testRemoveDecreasing(int low, int high)
    {
        int errors = 0;
        //
        ////////////////////////////////////////////////////
        // Test 1:               Remove in same order
        ////////////////////////////////////////////////////
        //
        _st->clear(); // Prepare the test

        //
        // Add all elements from low to high: linked list
        //
        const int DELTA = 1000;
        for (int k = high - 1; k >= low; k--)
        {
            _st->put(k, DELTA + k);
        }
        check(_st, errors);

        //
        // Positive Containment; remove in same order
        //
        int externalSize = 10;
        for (int k = high - 1; k >= low; k--)
        {
            ////////////////
            _st->remove(k);
            externalSize--;
            ////////////////

            if (_st->contains(k))
            {
                emitError("Remove error; successful remove did not actually remove the stated <key, value>", k);
                errors++;
            }

            for (int local_k = low; local_k < k; local_k++)
            {
                if (!_st->contains(local_k))
                {
                    emitError("Remove error; successful remove removed more keys than expected", local_k);
                    errors++;
                    emitError("Remove error: breaking to avoid error message bloat");
                    break;
                }
            }

            int acqSize = _st->size();
            if (acqSize != externalSize)
            {
                emitError("Shuffled remove error; size disagreement", acqSize, externalSize);
                errors++;
            }
            check(_st, errors);
        }

        //
        ////////////////////////////////////////////////////
        // Test 2:               Remove in reverse order
        ////////////////////////////////////////////////////
        //
        _st->clear(); // Prepare the test

        //
        // Add all elements from high to low: linked list
        //
        for (int k = high - 1; k >= low; k--)
        {
            _st->put(k, DELTA + k);
        }
        check(_st, errors);

        //
        // Positive Containment; remove in reverse order
        //
        externalSize = 10;
        for (int k = low; k < high; k++)
        {
            ////////////////
            _st->remove(k);
            externalSize--;
            ////////////////

            if (_st->contains(k))
            {
                emitError("Remove error; successful remove did not actually remove the stated <key, value>", k);
                errors++;
            }

            for (int local_k = k + 1; local_k < high; local_k++)
            {
                if (!_st->contains(local_k))
                {
                    emitError("Remove error; successful remove removed more keys than expected", local_k);
                    errors++;
                    emitError("Remove error: breaking to avoid error message bloat");
                    break;
                }
            }

            int acqSize = _st->size();
            if (acqSize != externalSize)
            {
                emitError("Shuffled remove error; size disagreement", acqSize, externalSize);
                errors++;
            }
            check(_st, errors);
        }

        return errors;
    }

    //
    // Mapping [low, high) to [1000 + low, 1000 + high) Shuffled add / remove
    //
    int testShuffle(int low, int high)
    {
        int errors = 0;

        for (int testCount = 0; testCount < 10; testCount++)
        {
            _st->clear(); // Prepare the test

            const int DELTA = 1000;
            std::vector<int> keys = Utilities::rPopulate(low, high);
            Utilities::shuffle(keys);
            for (int key : keys)
            {
                _st->put(key, DELTA + key);
            }
            check(_st, errors);

            //
            // Positive Containment; remove in same order
            //
            int externalSize = keys.size();
            keys = std::vector<int>{ 7, 8, 9, 3, 2, 1, 0, 4, 5, 6 };
            for (unsigned index = 0; index < keys.size(); index++)
            {
                if (_st->empty())
                {
                    emitError("Remove error; failed remove emptied the table...wowzer");
                    errors++;
                }

                ////////////////
                _st->remove(keys[index]);
                externalSize--;
                ////////////////

                if (_st->contains(keys[index]))
                {
                    emitError("Remove error; successful remove did not actually remove the stated <key, value>", keys[index]);
                    errors++;
                }

                for (unsigned local_index = index + 1; local_index < keys.size(); local_index++)
                {
                    if (!_st->contains(keys[local_index]))
                    {
                        emitError("Remove error; successful remove removed more keys than expected", keys[local_index]);
                        errors++;
                        emitError("Remove error: breaking to avoid error message bloat");
                        break;
                    }
                }

                int acqSize = _st->size();
                if (acqSize != externalSize)
                {
                    emitError("Shuffled remove error; size disagreement", acqSize, externalSize);
                    errors++;
                }

                check(_st, errors);
            }
        }

        return errors;
    }
};

#endif